#include "pch.h"

using namespace ao;

u64 reserveTest()
{
	FastIndexer finder(3);

	return finder.reserve();
}

TEST(BasicUseCaseTest, SmallIndexTest) {
  EXPECT_EQ(0, reserveTest());
  EXPECT_TRUE(true);
}