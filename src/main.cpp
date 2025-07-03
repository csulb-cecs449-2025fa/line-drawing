#include <SFML/Graphics.hpp>
#include <iostream>
#include "lines.h"

void drawPixel(sf::RenderWindow& window, sf::Vector2i position, sf::Color color) {
	float pX{ static_cast<float>(position.x) };
	float pY{ static_cast<float>(position.y) };

	sf::Vertex pixel[1]{
		sf::Vector2f{pX, pY},
		color
	};
	window.draw(pixel, 1, sf::PrimitiveType::Points);
}

int main() {
	sf::RenderWindow window{ sf::VideoMode::getFullscreenModes().at(0), "SFML Demo" };
	sf::Clock c;
	auto last{ c.getElapsedTime() };

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear();

		auto now{ c.getElapsedTime() };
		auto diff{ now - last };
		last = now;
		std::cout << 1 / diff.asSeconds() << " FPS " << std::endl;
		
		drawLineNaive(window, sf::Vector2i{ 100, 300 }, sf::Vector2i{ 700, 613 }, sf::Color::Red);

		window.display();
	}

	return 0;
}


