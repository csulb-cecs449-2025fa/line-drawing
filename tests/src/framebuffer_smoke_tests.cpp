#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

#include "framebuffer.h"

TEST(FramebufferSmoke, StoresAndReadsPixels) {
	// Smoke test: verify the basic read/write contract before testing lines.
	const Pixel background{ 1, 2, 3, 255 };
	const Pixel foreground{ 10, 20, 30, 255 };
	Framebuffer framebuffer{ 3, 2, background };

	EXPECT_EQ(framebuffer.width(), 3);
	EXPECT_EQ(framebuffer.height(), 2);
	EXPECT_EQ(framebuffer.getPixel(glm::ivec2{ 0, 0 }), background);

	EXPECT_TRUE(framebuffer.setPixel(glm::ivec2{ 2, 1 }, foreground));
	EXPECT_EQ(framebuffer.getPixel(glm::ivec2{ 2, 1 }), foreground);
	EXPECT_FALSE(framebuffer.setPixel(glm::ivec2{ 3, 1 }, foreground));
	EXPECT_THROW(
		static_cast<void>(framebuffer.getPixel(glm::ivec2{ -1, 0 })),
		std::out_of_range);
}

TEST(Framebuffer, RejectsInvalidDimensions) {
	// A framebuffer with no usable width or height should fail immediately.
	EXPECT_THROW(Framebuffer(0, 2), std::invalid_argument);
	EXPECT_THROW(Framebuffer(2, 0), std::invalid_argument);
	EXPECT_THROW(Framebuffer(-1, 2), std::invalid_argument);
}

TEST(Framebuffer, ContainsReportsBounds) {
	// contains() is the bounds check used by safe pixel writes.
	Framebuffer framebuffer{ 3, 2 };

	EXPECT_TRUE(framebuffer.contains(glm::ivec2{ 0, 0 }));
	EXPECT_TRUE(framebuffer.contains(glm::ivec2{ 2, 1 }));
	EXPECT_FALSE(framebuffer.contains(glm::ivec2{ -1, 0 }));
	EXPECT_FALSE(framebuffer.contains(glm::ivec2{ 3, 1 }));
	EXPECT_FALSE(framebuffer.contains(glm::ivec2{ 2, 2 }));
}

TEST(Framebuffer, ClearReplacesEveryPixel) {
	// clear() must replace old drawing everywhere, not only at previously written pixels.
	const Pixel background{ 1, 2, 3, 255 };
	const Pixel replacement{ 10, 20, 30, 128 };
	Framebuffer framebuffer{ 3, 2, background };

	framebuffer.setPixel(glm::ivec2{ 1, 0 }, Pixel{ 90, 80, 70, 255 });
	framebuffer.clear(replacement);

	for (const Pixel pixel : framebuffer.data()) {
		EXPECT_EQ(pixel, replacement);
	}
}

TEST(Framebuffer, DataUsesRowMajorOrder) {
	// The texture upload path depends on storage being [row 0, row 1, ...].
	Framebuffer framebuffer{ 2, 2 };
	const Pixel topLeft{ 1, 0, 0, 255 };
	const Pixel topRight{ 2, 0, 0, 255 };
	const Pixel bottomLeft{ 3, 0, 0, 255 };
	const Pixel bottomRight{ 4, 0, 0, 255 };

	framebuffer.setPixel(glm::ivec2{ 0, 0 }, topLeft);
	framebuffer.setPixel(glm::ivec2{ 1, 0 }, topRight);
	framebuffer.setPixel(glm::ivec2{ 0, 1 }, bottomLeft);
	framebuffer.setPixel(glm::ivec2{ 1, 1 }, bottomRight);

	const std::vector<Pixel> expected{
		topLeft, topRight, bottomLeft, bottomRight
	};
	EXPECT_EQ(framebuffer.data(), expected);
}

TEST(Framebuffer, OutOfBoundsWritesDoNotModifyStorage) {
	// Invalid writes are ignored and must not corrupt any valid pixel.
	const Pixel background{ 1, 2, 3, 255 };
	const Pixel foreground{ 10, 20, 30, 255 };
	Framebuffer framebuffer{ 2, 2, background };

	EXPECT_FALSE(framebuffer.setPixel(glm::ivec2{ -1, 0 }, foreground));
	EXPECT_FALSE(framebuffer.setPixel(glm::ivec2{ 2, 0 }, foreground));
	EXPECT_FALSE(framebuffer.setPixel(glm::ivec2{ 0, 2 }, foreground));

	for (const Pixel pixel : framebuffer.data()) {
		EXPECT_EQ(pixel, background);
	}
}
