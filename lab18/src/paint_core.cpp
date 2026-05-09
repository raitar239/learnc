#include "paint_core.hpp"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <cmath>
#include <stack>
#include <iostream>

extern std::stack<Canvas> undoStack;
void saveToUndo(const Canvas& canvas) {
    undoStack.push(canvas);
}

// ========== 1. Clear Canvas ==========
void clearCanvas(Canvas& canvas) {
    for (auto& row : canvas) {
        std::fill(row.begin(), row.end(), sf::Color::White);
    }
}

// ========== Draw Pixel ==========
void drawPixel(Canvas& canvas, int x, int y, const sf::Color& color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        canvas[y][x] = color;
    }
}

// ========== Draw Brush ==========
void drawBrush(Canvas& canvas, int x, int y, const sf::Color& color, int radius) {
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy * dy <= radius * radius) {
                drawPixel(canvas, x + dx, y + dy, color);
            }
        }
    }
}

// ========== 2. Draw Rectangle ==========
void drawRectangle(Canvas& canvas, int x1, int y1, int x2, int y2, const sf::Color& color) {
    int left = std::min(x1, x2);
    int right = std::max(x1, x2);
    int top = std::min(y1, y2);
    int bottom = std::max(y1, y2);

    for (int y = top; y <= bottom; ++y) {
        for (int x = left; x <= right; ++x) {
            drawPixel(canvas, x, y, color);
        }
    }
}

// ========== Draw Line (Bresenham) ==========
void drawLine(Canvas& canvas, int x1, int y1, int x2, int y2, const sf::Color& color) {
    int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        drawPixel(canvas, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

// ========== Undo ==========
void undo(Canvas& canvas) {
    if (!undoStack.empty()) {
        canvas = undoStack.top();
        undoStack.pop();
    }
}

// ========== 3. Shades of gray ==========
void applyGrayscale(Canvas& canvas) {
    for (auto& row : canvas) {
        for (auto& c : row) {
            uint8_t gray = static_cast<uint8_t>((c.r + c.g + c.b) / 3);
            c = sf::Color(gray, gray, gray);
        }
    }
}

// ========== Negative ==========
void applyNegative(Canvas& canvas) {
    for (auto& row : canvas) {
        for (auto& c : row) {
            c = sf::Color(255 - c.r, 255 - c.g, 255 - c.b);
        }
    }
}

// ========== Blur ==========
void applyBlur(Canvas& canvas) {
    Canvas original = canvas;
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            int r = 0, g = 0, b = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    sf::Color c = original[y + dy][x + dx];
                    r += c.r; g += c.g; b += c.b;
                }
            }
            canvas[y][x] = sf::Color(r / 9, g / 9, b / 9);
        }
    }
}

// ========== Вспомогательная отрисовка (SFML 3.1 compatible) ==========
void renderToWindow(sf::RenderWindow& window, const Canvas& canvas) {
    static sf::Image image;
    static sf::Texture texture;
    static bool initialized = false;
    
    if (!initialized) {
        image.resize({static_cast<unsigned int>(WIDTH), static_cast<unsigned int>(HEIGHT)});
        (void)texture.loadFromImage(image);
        initialized = true;
    }
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            image.setPixel({static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, canvas[y][x]);
        }
    }
    texture.update(image);
    sf::Sprite sprite(texture);
    
    float s = static_cast<float>(PIXEL_SIZE);
    sprite.setScale({s, s});
    
    window.draw(sprite);
}


// ========== 4. Pipette ==========
sf::Color pickColor(const Canvas& canvas, int x, int y, sf::Color defaultColor) {
    if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
        return canvas[y][x];
    }
    return defaultColor;
}

// ========== 5. Files ==========
void saveToFile(const Canvas& canvas, const std::string& filename) {
    sf::Image image;
    image.resize({static_cast<unsigned int>(WIDTH), static_cast<unsigned int>(HEIGHT)});

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            image.setPixel({static_cast<unsigned int>(x), static_cast<unsigned int>(y)}, canvas[y][x]);
        }
    }
    if (image.saveToFile(filename)) {
        std::cout << "Canvas saved to " << filename << std::endl;
    } else {
        std::cerr << "Failed to save image!" << std::endl;
    }
}

void loadFromFile(Canvas& canvas, const std::string& filename) {
    sf::Texture tempTexture;
    if (!tempTexture.loadFromFile(filename)) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return;
    }
    sf::Image tempImage = tempTexture.copyToImage();
    sf::Vector2u imgSize = tempImage.getSize();

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            unsigned int srcX = static_cast<unsigned int>(x * imgSize.x / WIDTH);
            unsigned int srcY = static_cast<unsigned int>(y * imgSize.y / HEIGHT);
            if (srcX < imgSize.x && srcY < imgSize.y) {
                canvas[y][x] = tempImage.getPixel({srcX, srcY});
            } else {
                canvas[y][x] = sf::Color::White;
            }
        }
    }
    std::cout << "Canvas loaded and resized: " << filename << std::endl;
}

std::string openFile() {
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "PNG Files\0*.png\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "png";

    if (GetOpenFileNameA(&ofn)) {
        return std::string(filename);
    }
    return "";
}