#include "pch.h"

using namespace ao;

static constexpr u32 MaxExponent = 4;

bool assignTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		for (u32 j = 0; j < n; ++j)
		{
			const u32 index = indexer.assign();
			if (index != j)
			{
				std::cout << "assignd index is " << index << ", " << "correct index is " << j << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool assign64Test()
{
	return assignTestInternal(64);
}

bool assign4096Test()
{
	return assignTestInternal(4096);
}

bool assign262144Test()
{
	return assignTestInternal(262144);
}

bool assign16777216Test()
{
	return assignTestInternal(16777216);
}

bool assign1073741824Test()
{
	if (MaxExponent < 5)
	{
		std::cout << "MaxExponent requires as setting of 5 or higher. MaxExponent = " << MaxExponent << std::endl;
		return false;
	}

	return assignTestInternal(1073741824);
}

bool releaseSimpleTest()
{
	for (u32 i = 0; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		const u32 index = indexer.assign();
		if (index != 0)
		{
			std::cout << "assignd index is " << index << ", " << "correct index is " << 0 << std::endl;
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
			indexer.assign();
		}

		for (u32 j = 0; j < n; ++j)
		{
			indexer.release(j);
		}

		const u32 index = indexer.assign();
		if (index != 0)
		{
			std::cout << "assignd index is " << index << ", " << "correct index is " << 0 << std::endl;
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
	if (MaxExponent < 5)
	{
		std::cout << "MaxExponent requires as setting of 5 or higher. MaxExponent = " << MaxExponent << std::endl;
		return false;
	}
	return releaseTestInternal(1073741824);
}

bool evenAssignTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		for (u32 j = 0; j < n; ++j)
		{
			indexer.assign();
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
			const u32 index = indexer.assign();
			if (index != (j << 1))
			{
				std::cout << "assignd index is " << index << ", " << "correct index is " << (j << 1) << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool evenAssign64Test()
{
	return evenAssignTestInternal(64);
}

bool evenAssign4096Test()
{
	return evenAssignTestInternal(4096);
}

bool evenAssign262144Test()
{
	return evenAssignTestInternal(262144);
}

bool evenAssign16777216Test()
{
	return evenAssignTestInternal(16777216);
}

bool evenAssign1073741824Test()
{
	if (MaxExponent < 5)
	{
		std::cout << "MaxExponent requires as setting of 5 or higher. MaxExponent = " << MaxExponent << std::endl;
		return false;
	}
	return evenAssignTestInternal(1073741824);
}

bool oddAssignTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		for (u32 j = 0; j < n; ++j)
		{
			indexer.assign();
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
			const u32 index = indexer.assign();
			if (index != ((j << 1) + 1))
			{
				std::cout << "assignd index is " << index << ", " << "correct index is " << ((j << 1) + 1) << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool oddAssign64Test()
{
	return oddAssignTestInternal(64);
}

bool oddAssign4096Test()
{
	return oddAssignTestInternal(4096);
}

bool oddAssign262144Test()
{
	return oddAssignTestInternal(262144);
}

bool oddAssign16777216Test()
{
	return oddAssignTestInternal(16777216);
}

bool oddAssign1073741824Test()
{
	if (MaxExponent < 5)
	{
		std::cout << "MaxExponent requires as setting of 5 or higher. MaxExponent = " << MaxExponent << std::endl;
		return false;
	}
	return oddAssignTestInternal(1073741824);
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
			indexer.assign();
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
			const u32 index = indexer.assign();
			if (index != indexes[j])
			{
				std::cout << "assignd index is " << index << ", " << "correct index is " << indexes[j] << std::endl;
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
	if (MaxExponent < 5)
	{
		std::cout << "MaxExponent requires as setting of 5 or higher. MaxExponent = " << MaxExponent << std::endl;
		return false;
	}
	return randomTestInternal(1073741824);
}

TEST(BasicUseCaseTest, Assign64Test) 
{
	EXPECT_EQ(true, assign64Test());
}

TEST(BasicUseCaseTest, Assign4096Test) 
{
	EXPECT_EQ(true, assign4096Test());
}

TEST(BasicUseCaseTest, Assign262144Test) 
{
	EXPECT_EQ(true, assign262144Test());
}

TEST(BasicUseCaseTest, Assign16777216Test) 
{
	EXPECT_EQ(true, assign16777216Test());
}

TEST(BasicUseCaseTest, Assign1073741824Test)
{
	EXPECT_EQ(true, assign1073741824Test());
}

TEST(BasicUseCaseTest, ReleaseSimpleTest)
{
	EXPECT_EQ(true, releaseSimpleTest());
}

TEST(BasicUseCaseTest, Release64Test)
{
	EXPECT_EQ(true, release64Test());
}

TEST(BasicUseCaseTest, Release4096Test)
{
	EXPECT_EQ(true, release4096Test());
}

TEST(BasicUseCaseTest, Release262144Test)
{
	EXPECT_EQ(true, release262144Test());
}

TEST(BasicUseCaseTest, Release16777216Test)
{
	EXPECT_EQ(true, release16777216Test());
}

TEST(BasicUseCaseTest, Release1073741824Test)
{
	EXPECT_EQ(true, release1073741824Test());
}

TEST(BasicUseCaseTest, EvenAssign64Test)
{
	EXPECT_EQ(true, evenAssign64Test());
}

TEST(BasicUseCaseTest, EvenAssign4096Test)
{
	EXPECT_EQ(true, evenAssign4096Test());
}

TEST(BasicUseCaseTest, EvenAssign262144Test)
{
	EXPECT_EQ(true, evenAssign262144Test());
}

TEST(BasicUseCaseTest, EvenAssign16777216Test)
{
	EXPECT_EQ(true, evenAssign16777216Test());
}

TEST(BasicUseCaseTest, EvenAssign1073741824Test)
{
	EXPECT_EQ(true, evenAssign1073741824Test());
}

TEST(BasicUseCaseTest, OddAssign64Test)
{
	EXPECT_EQ(true, oddAssign64Test());
}

TEST(BasicUseCaseTest, OddAssign4096Test)
{
	EXPECT_EQ(true, oddAssign4096Test());
}

TEST(BasicUseCaseTest, OddAssign262144Test)
{
	EXPECT_EQ(true, oddAssign262144Test());
}

TEST(BasicUseCaseTest, OddAssign16777216Test)
{
	EXPECT_EQ(true, oddAssign16777216Test());
}

TEST(BasicUseCaseTest, OddAssign1073741824Test)
{
	EXPECT_EQ(true, oddAssign1073741824Test());
}

TEST(BasicUseCaseTest, Random64Test)
{
	EXPECT_EQ(true, random64Test());
}

TEST(BasicUseCaseTest, Random4096Test)
{
	EXPECT_EQ(true, random4096Test());
}

TEST(BasicUseCaseTest, Random262144Test)
{
	EXPECT_EQ(true, random262144Test());
}

TEST(BasicUseCaseTest, Random16777216Test)
{
	EXPECT_EQ(true, random16777216Test());
}

TEST(BasicUseCaseTest, Random1073741824Test)
{
	EXPECT_EQ(true, random1073741824Test());
}