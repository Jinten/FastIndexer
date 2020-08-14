#include "pch.h"

using namespace ao;

static constexpr u32 MaxExponent = 5;

bool reserveTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		for (u32 j = 0; j < n; ++j)
		{
			const u32 index = indexer.reserve();
			if (index != j)
			{
				std::cout << "reserved index is " << index << ", " << "correct index is " << j << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool reserve64Test()
{
	return reserveTestInternal(64);
}

bool reserve4096Test()
{
	return reserveTestInternal(4096);
}

bool reserve262144Test()
{
	return reserveTestInternal(262144);
}

bool reserve16777216Test()
{
	return reserveTestInternal(16777216);
}

bool reserve1073741824Test()
{
	return reserveTestInternal(1073741824);
}

bool releaseSimpleTest()
{
	for (u32 i = 0; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		const u32 index = indexer.reserve();
		if (index != 0)
		{
			std::cout << "reserved index is " << index << ", " << "correct index is " << 0 << std::endl;
			return false;
		}

		indexer.release(0);
	}

	return true;
}

bool releaseTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		for (u32 j = 0; j < n; ++j)
		{
			indexer.reserve();
		}

		for (u32 j = 0; j < n; ++j)
		{
			indexer.release(j);
		}

		const u32 index = indexer.reserve();
		if (index != 0)
		{
			std::cout << "reserved index is " << index << ", " << "correct index is " << 0 << std::endl;
			return false;
		}
	}

	return true;
}

bool release64Test()
{
	return releaseTestInternal(64);
}

bool release4096Test()
{
	return releaseTestInternal(4096);
}

bool release262144Test()
{
	return releaseTestInternal(262144);
}

bool release16777216Test()
{
	return releaseTestInternal(16777216);
}

bool release1073741824Test()
{
	return releaseTestInternal(1073741824);
}

bool evenReserveTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		for (u32 j = 0; j < n; ++j)
		{
			indexer.reserve();
		}

		for (u32 j = 0; j < n; ++j)
		{
			if (j % 2 == 0)
			{
				indexer.release(j);
			}
		}

		const u32 half_n = n >> 1;
		for (u32 j = 0; j < half_n; ++j)
		{
			const u32 index = indexer.reserve();
			if (index != (j << 1))
			{
				std::cout << "reserved index is " << index << ", " << "correct index is " << (j << 1) << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool evenReserve64Test()
{
	return evenReserveTestInternal(64);
}

bool evenReserve4096Test()
{
	return evenReserveTestInternal(4096);
}

bool evenReserve262144Test()
{
	return evenReserveTestInternal(262144);
}

bool evenReserve16777216Test()
{
	return evenReserveTestInternal(16777216);
}

bool evenReserve1073741824Test()
{
	return evenReserveTestInternal(1073741824);
}

bool oddReserveTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		for (u32 j = 0; j < n; ++j)
		{
			indexer.reserve();
		}

		for (u32 j = 0; j < n; ++j)
		{
			if (j % 2 == 1)
			{
				indexer.release(j);
			}
		}

		const u32 half_n = n >> 1;
		for (u32 j = 0; j < half_n; ++j)
		{
			const u32 index = indexer.reserve();
			if (index != ((j << 1) + 1))
			{
				std::cout << "reserved index is " << index << ", " << "correct index is " << ((j << 1) + 1) << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool oddReserve64Test()
{
	return oddReserveTestInternal(64);
}

bool oddReserve4096Test()
{
	return oddReserveTestInternal(4096);
}

bool oddReserve262144Test()
{
	return oddReserveTestInternal(262144);
}

bool oddReserve16777216Test()
{
	return oddReserveTestInternal(16777216);
}

bool oddReserve1073741824Test()
{
	return oddReserveTestInternal(1073741824);
}

bool randomTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		for (u32 j = 0; j < n; ++j)
		{
			indexer.reserve();
		}

		const u32 n_rand = max(1, rand() % min(n, 20000000));

		u32 indexCount = 0;
		u64* indexes = new u64[n_rand];

		for (u32 j = 0; j < n; ++j)
		{
			if (rand() % 3 == 0)
			{
				indexes[indexCount] = j;
				indexer.release(j);

				++indexCount;
				if (indexCount == n_rand)
				{
					break;
				}
			}
		}

		for (u32 j = 0; j < indexCount; ++j)
		{
			const u32 index = indexer.reserve();
			if (index != indexes[j])
			{
				std::cout << "reserved index is " << index << ", " << "correct index is " << indexes[j] << std::endl;
				return false;
			}
		}

		delete[] indexes;
	}

	return true;
}

bool random64Test()
{
	return randomTestInternal(64);
}

bool random4096Test()
{
	return randomTestInternal(4096);
}

bool random262144Test()
{
	return randomTestInternal(262144);
}

bool random16777216Test()
{
	return randomTestInternal(16777216);
}

bool random1073741824Test()
{
	return randomTestInternal(1073741824);
}

TEST(BasicUseCaseTest, Reserve64Test) {
	EXPECT_EQ(true, reserve64Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Reserve4096Test) {
	EXPECT_EQ(true, reserve4096Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Reserve262144Test) {
	EXPECT_EQ(true, reserve262144Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Reserve16777216Test) {
	EXPECT_EQ(true, reserve16777216Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Reserve1073741824Test) {
	EXPECT_EQ(true, reserve1073741824Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, ReleaseSimpleTest) {
	EXPECT_EQ(true, releaseSimpleTest());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Release64Test) {
	EXPECT_EQ(true, release64Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Release4096Test) {
	EXPECT_EQ(true, release4096Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Release262144Test) {
	EXPECT_EQ(true, release262144Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Release16777216Test) {
	EXPECT_EQ(true, release16777216Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Release1073741824Test) {
	EXPECT_EQ(true, release1073741824Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, EvenReserve64Test) {
	EXPECT_EQ(true, evenReserve64Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, EvenReserve4096Test) {
	EXPECT_EQ(true, evenReserve4096Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, EvenReserve262144Test) {
	EXPECT_EQ(true, evenReserve262144Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, EvenReserve16777216Test) {
	EXPECT_EQ(true, evenReserve16777216Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, EvenReserve1073741824Test) {
	EXPECT_EQ(true, evenReserve1073741824Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, OddReserve64Test) {
	EXPECT_EQ(true, oddReserve64Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, OddReserve4096Test) {
	EXPECT_EQ(true, oddReserve4096Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, OddReserve262144Test) {
	EXPECT_EQ(true, oddReserve262144Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, OddReserve16777216Test) {
	EXPECT_EQ(true, oddReserve16777216Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, OddReserve1073741824Test) {
	EXPECT_EQ(true, oddReserve1073741824Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Random64Test) {
	EXPECT_EQ(true, random64Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Random4096Test) {
	EXPECT_EQ(true, random4096Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Random262144Test) {
	EXPECT_EQ(true, random262144Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Random16777216Test) {
	EXPECT_EQ(true, random16777216Test());
	EXPECT_TRUE(true);
}

TEST(BasicUseCaseTest, Random1073741824Test) {
	EXPECT_EQ(true, random1073741824Test());
	EXPECT_TRUE(true);
}