#include <gtest/gtest.h>

#include <algorithm>
#include <cstdlib>
#include <string>

#include "line_test_helpers.h"
#include "lines.h"

namespace {

struct LineCase {
	std::string name;
	glm::ivec2 start;
	glm::ivec2 end;
	std::set<PixelPosition> expected;
};

class BresenhamLineContractTest : public testing::TestWithParam<LineCase> {};

TEST_P(BresenhamLineContractTest, WritesExpectedPixels) {
	// Each case describes the complete set of pixels a Bresenham line should write.
	const Pixel background{};
	const Pixel lineColor{ 255, 20, 40, 255 };
	Framebuffer framebuffer{ 8, 8, background };

	const LineCase& lineCase{ GetParam() };
	SCOPED_TRACE("Line case: " + lineCase.name);
	drawLine(framebuffer, lineCase.start, lineCase.end, lineColor);

	EXPECT_EQ(nonBackgroundPositions(framebuffer, background), lineCase.expected)
		<< "The set of non-background pixels does not match the expected line shape.";

	for (const PixelPosition position : lineCase.expected) {
		SCOPED_TRACE(
			"Checking expected line pixel at ("
			+ std::to_string(position.first) + ", "
			+ std::to_string(position.second) + ")");

		EXPECT_EQ(
			framebuffer.getPixel(glm::ivec2{ position.first, position.second }),
			lineColor)
			<< "Every expected line pixel should have the requested line color.";
	}
}

// These cases exercise positive/negative low slopes, horizontal lines, and
// endpoint reversal while keeping the expected pixel set explicit.
INSTANTIATE_TEST_SUITE_P(
	LowAndHighSlope,
	BresenhamLineContractTest,
	testing::Values(
		LineCase{
			"PositiveLowSlope",
			glm::ivec2{ 1, 1 },
			glm::ivec2{ 6, 3 },
			{{ 1, 1 }, { 2, 1 }, { 3, 2 }, { 4, 2 }, { 5, 3 }, { 6, 3 }}
		},
		LineCase{
			"NegativeLowSlope",
			glm::ivec2{ 1, 5 },
			glm::ivec2{ 6, 2 },
			{{ 1, 5 }, { 2, 4 }, { 3, 4 }, { 4, 3 }, { 5, 3 }, { 6, 2 }}
		},
		LineCase{
			"Horizontal",
			glm::ivec2{ 1, 4 },
			glm::ivec2{ 6, 4 },
			{{ 1, 4 }, { 2, 4 }, { 3, 4 }, { 4, 4 }, { 5, 4 }, { 6, 4 }}
		},
		LineCase{
			"ReversedEndpoints",
			glm::ivec2{ 6, 3 },
			glm::ivec2{ 1, 1 },
			{{ 1, 1 }, { 2, 1 }, { 3, 2 }, { 4, 2 }, { 5, 3 }, { 6, 3 }}
		}
	),
	[](const testing::TestParamInfo<LineCase>& info) {
		return info.param.name;
	});

TEST(BresenhamLineContract, ClipsOutOfBoundsPixels) {
	// A line may extend outside the framebuffer, but visible pixels must remain correct.
	const Pixel background{};
	const Pixel lineColor{ 255, 20, 40, 255 };
	Framebuffer framebuffer{ 4, 4, background };

	drawLine(
		framebuffer,
		glm::ivec2{ -2, 1 },
		glm::ivec2{ 3, 3 },
		lineColor);

	const std::set<PixelPosition> expectedPositions{
		{ 0, 2 }, { 1, 2 }, { 2, 3 }, { 3, 3 }
	};
	EXPECT_EQ(nonBackgroundPositions(framebuffer, background), expectedPositions)
		<< "Only in-bounds pixels should be present after clipping.";
}

TEST(BresenhamLineContract, HighSlopeIncludesEveryDrivenCoordinate) {
	// For a high-slope line, y is the driven coordinate, so every y endpoint
	// value must receive exactly one pixel.
	const Pixel background{};
	const Pixel lineColor{ 255, 20, 40, 255 };
	Framebuffer framebuffer{ 8, 8, background };

	drawLine(
		framebuffer,
		glm::ivec2{ 2, 1 },
		glm::ivec2{ 4, 7 },
		lineColor);

	const std::set<PixelPosition> expectedPositions{
		{ 2, 1 }, { 2, 2 }, { 3, 3 }, { 3, 4 },
		{ 3, 5 }, { 4, 6 }, { 4, 7 }
	};
	EXPECT_EQ(nonBackgroundPositions(framebuffer, background), expectedPositions)
		<< "A high-slope line must advance the y coordinate for each pixel.";
}

TEST(BresenhamLineContract, VerticalLineIncludesBothEndpoints) {
	// A vertical line is the high-slope boundary case with dx == 0.
	const Pixel background{};
	const Pixel lineColor{ 255, 20, 40, 255 };
	Framebuffer framebuffer{ 8, 8, background };

	drawLine(
		framebuffer,
		glm::ivec2{ 3, 1 },
		glm::ivec2{ 3, 5 },
		lineColor);

	const std::set<PixelPosition> expectedPositions{
		{ 3, 1 }, { 3, 2 }, { 3, 3 }, { 3, 4 }, { 3, 5 }
	};
	EXPECT_EQ(nonBackgroundPositions(framebuffer, background), expectedPositions)
		<< "Both endpoints and every intervening y coordinate must be drawn.";
}

TEST(BresenhamLineContract, PixelCountMatchesDrivenCoordinateCount) {
	// A line must contain max(dx, dy) + 1 unique pixels, including both endpoints.
	const Pixel background{};
	const Pixel lineColor{ 255, 20, 40, 255 };
	Framebuffer framebuffer{ 8, 8, background };
	const glm::ivec2 start{ 1, 1 };
	const glm::ivec2 end{ 6, 3 };

	drawLine(framebuffer, start, end, lineColor);

	const int dx{ std::abs(end.x - start.x) };
	const int dy{ std::abs(end.y - start.y) };
	const std::size_t expectedCount{ static_cast<std::size_t>(std::max(dx, dy) + 1) };
	EXPECT_EQ(nonBackgroundPositions(framebuffer, background).size(), expectedCount)
		<< "The line should contain one pixel for each driven coordinate.";
}

} // namespace
