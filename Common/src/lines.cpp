#include "lines.h"

#include <cmath>
#include <cstdint>
#include <utility>

/**
 * @brief Draws a low-slope line using a naive algorithm.
 * @param framebuffer the CPU framebuffer to draw to.
 * @param start the starting point of the line.
 * @param end the ending point of the line.
 * @param color the color of the line.
 */
void drawLineNaive(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color) {
	if (start.x > end.x) {
		std::swap(start, end);
	}

	double m{ static_cast<double>(end.y - start.y) / (end.x - start.x) };

	// The first point on the line is (start.x, start.y).
	std::int32_t x{ start.x };

	// We separately track where y *should be*, and also which integer y value is closest to that.
	double yReal{ static_cast<double>(start.y) };
	std::int32_t yPixel{ start.y };

	while (x <= end.x) {
		framebuffer.setPixel(glm::ivec2{ x, yPixel }, color);
		// x always goes up by 1.
		++x;
		// yReal increases by m.
		yReal += m;
		// yPixel is the rounded yReal.
		yPixel = static_cast<std::int32_t>(std::round(yReal));
	}
}

void drawLinePointSlope(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color) {
	if (start.x > end.x) {
		std::swap(start, end);
	}

	// Put in point-slope form and solve for b.
	// y - k = m(x - h) --> y = mx - mh + k --> b = -mh + k

	// m = rise over run
	// b = -mh + k --> -m * (start.x) + (start.y)
	double m{ static_cast<double>(end.y - start.y) / (end.x - start.x) };
	double b{ static_cast<double>(-m * start.x + start.y) };

	std::int32_t x{ start.x };
	while (x <= end.x) {
		// For each value of x, calculate yPixel using y = mx + b
		std::int32_t yPixel{ static_cast<std::int32_t>(std::round(m * x + b)) };
		framebuffer.setPixel(glm::ivec2{ x, yPixel }, color);
		++x;
	}
}

void drawLineLow(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color) {
	// TODO: Students implement the low-slope Bresenham algorithm here.
}

void drawLineHigh(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color) {
	// TODO: Students implement the high-slope Bresenham algorithm here.
}

void drawLine(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color) {
	const int dx{ std::abs(end.x - start.x) };
	const int dy{ std::abs(end.y - start.y) };

	if (dy <= dx) {
		drawLineLow(framebuffer, start, end, color);
	}
	else {
		drawLineHigh(framebuffer, start, end, color);
	}
}

void drawGradientLineLow(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end,
	Pixel startColor, Pixel endColor) {
	// TODO: Students implement the low-slope gradient Bresenham algorithm here.
}
