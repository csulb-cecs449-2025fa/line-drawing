#include "lines.h"
#include <SFML/Graphics.hpp>

/**
 * @brief Draws a low-slope line using a naive algorithm.
 * @param window the SFML window to draw to.
 * @param start the starting point of the line.
 * @param end the ending point of the line.
 * @param color the color of the line.
 */
void drawLineNaive(sf::RenderWindow& window, sf::Vector2i start, sf::Vector2i end, sf::Color color) {
	if (start.x > end.x) {
		std::swap(start, end);
	}

	double m{ static_cast<double>(end.y - start.y) / (end.x - start.x) };
	double y{ start.y };
	for (auto x{ start.x }; x < end.x + 1; ++x) {
		auto yCoord{ static_cast<int32_t>(std::round(y)) };
		drawPixel(window, sf::Vector2i{ x, yCoord }, color);
		y += m;
	}
}
