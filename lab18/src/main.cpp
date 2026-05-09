#include "paint_core.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

// Tools
enum class Tool { BRUSH, RECTANGLE, LINE, ERASER };
std::stack<Canvas> undoStack;

int main() {
    const int UI_HEIGHT = 120; 
    
    // Create window
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(WIDTH * PIXEL_SIZE), 
                       static_cast<unsigned int>(HEIGHT * PIXEL_SIZE + UI_HEIGHT)}),
        "Paint++"
    );

    // Centring
    auto desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        static_cast<int>((desktop.size.x - window.getSize().x) / 2),
        static_cast<int>((desktop.size.y - window.getSize().y) / 2)
    ));

    // Init canvas
    Canvas canvas(HEIGHT, std::vector<sf::Color>(WIDTH, sf::Color::White));
    clearCanvas(canvas);

    // Tools
    Tool currentTool = Tool::BRUSH;
    sf::Color currentColor = sf::Color::Black;
    int brushRadius = 3;
    bool isDrawing = false;
    int startX = 0, startY = 0, lastX = 0, lastY = 0;

    // Palette
    std::vector<sf::Color> palette = {
        sf::Color::Black, sf::Color::White, sf::Color::Red,
        sf::Color::Green, sf::Color::Blue, sf::Color::Yellow,
        sf::Color::Cyan, sf::Color::Magenta, sf::Color(255, 128, 0),
        sf::Color(128, 0, 128)
    };
    int selectedPaletteIndex = 0;
    auto syncPaletteIndex = [&](sf::Color color) {
        for (size_t i = 0; i < palette.size(); ++i) {
            if (palette[i] == color) {
                selectedPaletteIndex = static_cast<int>(i);
                return;
            }
        }
        selectedPaletteIndex = -1;
    };

    // Text
    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        std::cerr << "Error: Could not load arial.ttf\n";
    }
    
    sf::Text status(font);
    status.setCharacterSize(14);
    status.setFillColor(sf::Color::Black);
    status.setPosition({10.f, static_cast<float>(HEIGHT * PIXEL_SIZE + 10)});

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            // --- Keys ---
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::B: currentTool = Tool::BRUSH; break;
                    case sf::Keyboard::Key::R: currentTool = Tool::RECTANGLE; break;
                    case sf::Keyboard::Key::L: currentTool = Tool::LINE; break;
                    case sf::Keyboard::Key::E: currentTool = Tool::ERASER; break;
                    case sf::Keyboard::Key::C: saveToUndo(canvas); clearCanvas(canvas); break;
                    
                    case sf::Keyboard::Key::Z:
                        if (keyPressed->control) undo(canvas);
                        break;

                    case sf::Keyboard::Key::Add:
                    case sf::Keyboard::Key::Equal:
                        if (brushRadius < 20) brushRadius++;
                        break;

                    case sf::Keyboard::Key::Hyphen:
                    case sf::Keyboard::Key::Subtract:
                        if (brushRadius > 1) brushRadius--;
                        break;

                    // Filtres
                    case sf::Keyboard::Key::G: saveToUndo(canvas); applyGrayscale(canvas); break;
                    case sf::Keyboard::Key::N: saveToUndo(canvas); applyNegative(canvas); break;
                    case sf::Keyboard::Key::M: saveToUndo(canvas); applyBlur(canvas); break;

                    // Pipette
                    case sf::Keyboard::Key::P: {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        currentColor = pickColor(canvas, mousePos.x / PIXEL_SIZE, mousePos.y / PIXEL_SIZE, currentColor);
                        syncPaletteIndex(currentColor);
                        break;
                    }

                    // Files 
                    case sf::Keyboard::Key::S: {
                        if (keyPressed->control) {
                            saveToFile(canvas, "drawing.png");
                        }
                        break;
                    }
                    case sf::Keyboard::Key::O: {
                        if (keyPressed->control) {
                            std::string path = openFile();
                            if (!path.empty()) {
                                saveToUndo(canvas);
                                loadFromFile(canvas, path);
                            }
                        }
                        break;
                    }


                    // Colors
                    case sf::Keyboard::Key::Num0:
                    case sf::Keyboard::Key::Num1:
                    case sf::Keyboard::Key::Num2:
                    case sf::Keyboard::Key::Num3:
                    case sf::Keyboard::Key::Num4:
                    case sf::Keyboard::Key::Num5:
                    case sf::Keyboard::Key::Num6:
                    case sf::Keyboard::Key::Num7:
                    case sf::Keyboard::Key::Num8:
                    case sf::Keyboard::Key::Num9: {
                        int index = static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Num0);
                        if (index >= 0 && index < (int)palette.size()) {
                            selectedPaletteIndex = index;
                            currentColor = palette[index];
                        }
                        break;
                    }
                    default: break;
                }
            }

            // --- Mouse (press) ---
            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = mousePress->position;

                if (mousePress->button == sf::Mouse::Button::Right) {
                    currentColor = pickColor(canvas, mousePos.x / PIXEL_SIZE, mousePos.y / PIXEL_SIZE, currentColor);
                    syncPaletteIndex(currentColor);
                } 
                else if (mousePress->button == sf::Mouse::Button::Left) {
                    int paletteY = HEIGHT * PIXEL_SIZE + 50;
                    if (mousePos.y >= paletteY && mousePos.y <= paletteY + 30) {
                        int index = (mousePos.x - 10) / 35;
                        if (index >= 0 && index < (int)palette.size()) {
                            selectedPaletteIndex = index;
                            currentColor = palette[index];
                        }
                    } 
                    else if (mousePos.y < HEIGHT * PIXEL_SIZE) {
                        isDrawing = true;
                        startX = mousePos.x / PIXEL_SIZE;
                        startY = mousePos.y / PIXEL_SIZE;
                        lastX = startX; lastY = startY;

                        saveToUndo(canvas);
                        if (currentTool == Tool::BRUSH || currentTool == Tool::ERASER) {
                            sf::Color drawColor = (currentTool == Tool::ERASER) ? sf::Color::White : currentColor;
                            drawBrush(canvas, startX, startY, drawColor, brushRadius);
                        }
                    }
                }
            }

            // --- Mouse (move) ---
            if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                if (isDrawing && (currentTool == Tool::BRUSH || currentTool == Tool::ERASER)) {
                    int x = mouseMove->position.x / PIXEL_SIZE;
                    int y = mouseMove->position.y / PIXEL_SIZE;
                    if (x != lastX || y != lastY) {
                        sf::Color drawColor = (currentTool == Tool::ERASER) ? sf::Color::White : currentColor;
                        drawBrush(canvas, x, y, drawColor, brushRadius);
                        lastX = x; lastY = y;
                    }
                }
            }

            // --- Mouse (release) ---
            if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseRelease->button == sf::Mouse::Button::Left && isDrawing) {
                    if (currentTool == Tool::RECTANGLE || currentTool == Tool::LINE) {
                        int endX = mouseRelease->position.x / PIXEL_SIZE;
                        int endY = mouseRelease->position.y / PIXEL_SIZE;
                        sf::Color drawColor = currentColor;
                        
                        if (currentTool == Tool::RECTANGLE)
                            drawRectangle(canvas, startX, startY, endX, endY, drawColor);
                        else
                            drawLine(canvas, startX, startY, endX, endY, drawColor);
                    }
                    isDrawing = false;
                }
            }
        }

        // --- Render all ---
        window.clear(sf::Color(200, 200, 200));
        renderToWindow(window, canvas);

        // Palette
        for (size_t i = 0; i < palette.size(); ++i) {
            sf::RectangleShape box(sf::Vector2f{30.f, 30.f});
            box.setFillColor(palette[i]);
            box.setPosition({10.f + i * 35.f, static_cast<float>(HEIGHT * PIXEL_SIZE + 50)});
            box.setOutlineThickness(2);
            box.setOutlineColor((i == (size_t)selectedPaletteIndex) ? sf::Color::Red : sf::Color::Black);
            window.draw(box);
        }

        // Status bar
        std::string toolNames[] = {"BRUSH", "RECTANGLE", "LINE", "ERASER"};
        status.setString(toolNames[(int)currentTool] + 
                        " | Radius: " + std::to_string(brushRadius) +
                        " | Undo steps: " + std::to_string(undoStack.size()));    
        window.draw(status);
        window.display();
    }

    return 0;
}