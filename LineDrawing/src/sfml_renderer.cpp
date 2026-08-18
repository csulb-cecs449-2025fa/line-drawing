#include "sfml_renderer.h"

#include <cstdint>

void drawFramebuffer(
	sf::RenderWindow& window,
	sf::Texture& texture,
	const sf::Sprite& sprite,
	const Framebuffer& framebuffer) {
	texture.update(reinterpret_cast<const std::uint8_t*>(framebuffer.data().data()));
	window.draw(sprite);
}	
