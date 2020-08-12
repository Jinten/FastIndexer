#include "pch.h"

using namespace ao;

u64 reserveTest(u32 nReserves)
{
	FastIndexer finder(3);

	for (u32 i = 0; i < nReserves; ++i)
	{
		finder.reserve();
	}

	return finder.reserve();
}

TEST(BasicUseCaseTest, ReserveTest0) {
  EXPECT_EQ(0, reserveTest(0));
  EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, ReserveTest1) {
	EXPECT_EQ(1, reserveTest(1));
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, ReserveTest2) {
	EXPECT_EQ(2, reserveTest(2));
	EXPECT_TRUE(true);
}