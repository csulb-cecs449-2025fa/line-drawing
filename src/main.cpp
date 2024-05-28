#include <SFML/Graphics.hpp>
#include <iostream>
#include "lines.h"

void drawPixel(sf::RenderWindow& window, sf::Vector2i position, sf::Color color) {
	sf::Vertex pixel[1] = {
		sf::Vertex(sf::Vector2f(position.x, position.y), color)
	};
	window.draw(pixel, 1, sf::Points);
}

int main() {
	sf::RenderWindow window{ sf::VideoMode{800, 800}, "SFML Demo" };
	sf::Clock c;
	auto last = c.getElapsedTime();
	uint32_t mx = 400, my = 300;

	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed)
				window.close();
			else if (ev.type == sf::Event::MouseMoved) {
				mx = ev.mouseMove.x;
				my = ev.mouseMove.y;
			}
		}
		window.clear();

		auto now = c.getElapsedTime();
		auto diff = now - last;
		last = now;
		std::cout << 1 / diff.asSeconds() << " FPS " << std::endl;
		
		drawLineNaive(window, sf::Vector2i(100, 300), sf::Vector2i(700, 613), sf::Color::Red);

		window.display();
	}

	return 0;
}


