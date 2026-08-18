#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "framebuffer.h"
#include "lines.h"
#include "sfml_renderer.h"

int main() {
	sf::RenderWindow window{ sf::VideoMode::getFullscreenModes().at(0), "SFML Demo" };
	const auto windowSize{ window.getSize() };
	const Pixel backgroundColor{};
	Framebuffer framebuffer{
		static_cast<int>(windowSize.x),
		static_cast<int>(windowSize.y),
		backgroundColor
	};
	sf::Texture framebufferTexture{ windowSize };
	framebufferTexture.setSmooth(false);
	sf::Sprite framebufferSprite{ framebufferTexture };
	sf::Clock c;
	auto last{ c.getElapsedTime() };

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);
		framebuffer.clear(backgroundColor);

		auto now{ c.getElapsedTime() };
		auto diff{ now - last };
		last = now;
		std::cout << 1 / diff.asSeconds() << " FPS " << std::endl;

		drawLineNaive(
			framebuffer,
			glm::ivec2{ 100, 300 },
			glm::ivec2{ 700, 613 },
			Pixel{ 255, 0, 0, 255 });
		drawFramebuffer(window, framebufferTexture, framebufferSprite, framebuffer);

		window.display();
	}

	return 0;
}
