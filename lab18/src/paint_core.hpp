#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

inline constexpr int WIDTH      = 960;
inline constexpr int HEIGHT     = 540;
inline constexpr int PIXEL_SIZE = 1;

using Canvas = std::vector<std::vector<sf::Color>>;

// ========== Base art ==========
void clearCanvas(Canvas& canvas);
void drawPixel(Canvas& canvas, int x, int y, const sf::Color& color);
void drawBrush(Canvas& canvas, int x, int y, const sf::Color& color, int radius);

// ========== Figures and Undo ==========
void drawRectangle(Canvas& canvas, int x1, int y1, int x2, int y2, const sf::Color& color);
void drawLine(Canvas& canvas, int x1, int y1, int x2, int y2, const sf::Color& color);
void undo(Canvas& canvas);
void saveToUndo(const Canvas& canvas);

// ========== Filters ==========
void applyGrayscale(Canvas& canvas);
void applyNegative(Canvas& canvas);
void applyBlur(Canvas& canvas);

// ========== render ==========
void renderToWindow(sf::RenderWindow& window, const Canvas& canvas);


// ========== Pipette ==========
sf::Color pickColor(const Canvas& canvas, int x, int y, sf::Color defaultColor);

// ========== Files ==========
void saveToFile(const Canvas& canvas, const std::string& filename);
void loadFromFile(Canvas& canvas, const std::string& filename);
std::string openFile();