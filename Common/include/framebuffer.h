#pragma once

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <vector>

#include <glm/vec2.hpp>

/**
 * @brief A small RGBA pixel value used by the CPU framebuffer.
 */
struct Pixel {
	std::uint8_t r{ 0 };
	std::uint8_t g{ 0 };
	std::uint8_t b{ 0 };
	std::uint8_t a{ 255 };

	constexpr bool operator==(const Pixel&) const = default;
};

inline std::ostream& operator<<(std::ostream& stream, Pixel pixel) {
	return stream
		<< "Pixel{r=" << static_cast<unsigned int>(pixel.r)
		<< ", g=" << static_cast<unsigned int>(pixel.g)
		<< ", b=" << static_cast<unsigned int>(pixel.b)
		<< ", a=" << static_cast<unsigned int>(pixel.a)
		<< "}";
}

/**
 * @brief A CPU-side, row-major pixel buffer for headless rendering tests.
 *
 * Coordinates use the same convention as SFML: (0, 0) is the upper-left
 * corner, x increases to the right, and y increases downward.
 */
class Framebuffer {
public:
	Framebuffer(int width, int height, Pixel clearColor = {});

	[[nodiscard]] int width() const noexcept;
	[[nodiscard]] int height() const noexcept;
	[[nodiscard]] bool contains(glm::ivec2 position) const noexcept;

	/**
	 * @brief Fills every pixel with the supplied color.
	 */
	void clear(Pixel color);

	/**
	 * @brief Writes a pixel when position is in bounds.
	 * @return true when the pixel was written; false for an out-of-bounds position.
	 */
	bool setPixel(glm::ivec2 position, Pixel color) noexcept;

	/**
	 * @brief Reads a pixel.
	 * @throws std::out_of_range when position is outside the framebuffer.
	 */
	[[nodiscard]] Pixel getPixel(glm::ivec2 position) const;

	/**
	 * @brief Returns the row-major pixel storage for inspection or presentation.
	 */
	[[nodiscard]] const std::vector<Pixel>& data() const noexcept;

private:
	[[nodiscard]] std::size_t index(glm::ivec2 position) const noexcept;

	int m_width;
	int m_height;
	std::vector<Pixel> m_pixels;
};
