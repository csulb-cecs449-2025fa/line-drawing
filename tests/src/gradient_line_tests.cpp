#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

#include "line_test_helpers.h"
#include "lines.h"

namespace {

struct ExpectedPixel {
	PixelPosition position;
	Pixel color;
};

void expectPixels(
	const Framebuffer& framebuffer,
	const std::vector<ExpectedPixel>& expected) {
	for (const ExpectedPixel& expectedPixel : expected) {
		SCOPED_TRACE(
			"Checking gradient color at ("
			+ std::to_string(expectedPixel.position.first) + ", "
			+ std::to_string(expectedPixel.position.second) + ")");
		EXPECT_EQ(
			framebuffer.getPixel(glm::ivec2{
				expectedPixel.position.first,
				expectedPixel.position.second
			}),
			expectedPixel.color);
	}
}

} // namespace

TEST(GradientLineLow, UsesBresenhamPixelsAndInterpolatesRGBA) {
	// The low-slope gradient must use the same pixel geometry as Bresenham,
	// while interpolating every RGBA channel from startColor to endColor.
	const Pixel background{};
	const Pixel startColor{ 0, 10, 20, 30 };
	const Pixel endColor{ 255, 210, 200, 230 };
	Framebuffer framebuffer{ 8, 8, background };

	drawGradientLineLow(
		framebuffer,
		glm::ivec2{ 1, 1 },
		glm::ivec2{ 6, 3 },
		startColor,
		endColor);

	const std::vector<ExpectedPixel> expected{
		{ { 1, 1 }, { 0, 10, 20, 30 } },
		{ { 2, 1 }, { 51, 50, 56, 70 } },
		{ { 3, 2 }, { 102, 90, 92, 110 } },
		{ { 4, 2 }, { 153, 130, 128, 150 } },
		{ { 5, 3 }, { 204, 170, 164, 190 } },
		{ { 6, 3 }, { 255, 210, 200, 230 } }
	};
	const std::set<PixelPosition> expectedPositions{
		{ 1, 1 }, { 2, 1 }, { 3, 2 },
		{ 4, 2 }, { 5, 3 }, { 6, 3 }
	};

	expectPixels(framebuffer, expected);
	EXPECT_EQ(nonBackgroundPositions(framebuffer, background), expectedPositions)
		<< "The gradient must visit the same low-slope pixels as Bresenham.";
}

TEST(GradientLineLow, ReversedEndpointsKeepColorsAttachedToTheirEndpoints) {
	// Reversing traversal changes the order of writes, but each supplied color
	// must remain attached to its corresponding geometric endpoint.
	const Pixel background{};
	const Pixel startColor{ 255, 0, 0, 255 };
	const Pixel endColor{ 0, 0, 255, 255 };
	Framebuffer framebuffer{ 8, 8, background };

		drawGradientLineLow(
		framebuffer,
		glm::ivec2{ 6, 4 },
		glm::ivec2{ 1, 4 },
		startColor,
		endColor);

	EXPECT_EQ(framebuffer.getPixel(glm::ivec2{ 6, 4 }), startColor);
	EXPECT_EQ(framebuffer.getPixel(glm::ivec2{ 1, 4 }), endColor);
	EXPECT_EQ(
		framebuffer.getPixel(glm::ivec2{ 3, 4 }),
		(Pixel{ 102, 0, 153, 255 }));
}

TEST(GradientLineLow, SinglePointUsesStartColor) {
	// With no distance to interpolate, the one pixel receives startColor and
	// the implementation must avoid dividing by zero.
	const Pixel background{};
	const Pixel startColor{ 10, 20, 30, 40 };
	const Pixel endColor{ 200, 210, 220, 230 };
	Framebuffer framebuffer{ 4, 4, background };

	drawGradientLineLow(
		framebuffer,
		glm::ivec2{ 2, 1 },
		glm::ivec2{ 2, 1 },
		startColor,
		endColor);

	const std::set<PixelPosition> expectedPositions{ { 2, 1 } };
	EXPECT_EQ(framebuffer.getPixel(glm::ivec2{ 2, 1 }), startColor);
	EXPECT_EQ(nonBackgroundPositions(framebuffer, background), expectedPositions);
}
