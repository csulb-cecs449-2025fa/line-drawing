#pragma once

#include <SFML/Graphics.hpp>

#include "framebuffer.h"

/**
 * @brief Uploads the CPU framebuffer to an SFML texture and draws it.
 */
void drawFramebuffer(
	sf::RenderWindow& window,
	sf::Texture& texture,
	const sf::Sprite& sprite,
	const Framebuffer& framebuffer);
