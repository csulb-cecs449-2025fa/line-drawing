#pragma once

#include <glm/vec2.hpp>

#include "framebuffer.h"

void drawLineNaive(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color);
void drawLinePointSlope(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color);

/**
 * @brief Draws the low-slope portion of the student Bresenham implementation.
 */
void drawLineLow(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color);

/**
 * @brief Draws the high-slope portion of the student Bresenham implementation.
 */
void drawLineHigh(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color);

/**
 * @brief Draws a line using the student's Bresenham implementation.
 */
void drawLine(Framebuffer& framebuffer, glm::ivec2 start, glm::ivec2 end, Pixel color);

/**
 * @brief Draws a low-slope Bresenham line with an RGBA color gradient.
 */
void drawGradientLineLow(
	Framebuffer& framebuffer,
	glm::ivec2 start,
	glm::ivec2 end,
	Pixel startColor,
	Pixel endColor);
