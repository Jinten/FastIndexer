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

bool parallelAssignTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		HANDLE eventHandle = CreateEvent(nullptr, true, false, L"PARALLEL_TEST_EVENT");
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		u32 counter = 0;
		u32* indexes = new u32[n];

		auto parallelAssign = [&]()
		{
			WaitForSingleObject(eventHandle, INFINITE);

			while (true)
			{
				const u32 index = InterlockedIncrement(&counter);
				if (index >= n)
				{
					break;
				}
				indexes[index] = indexer.assign();
			}
		};

		const auto nThreads = std::thread::hardware_concurrency();
		std::thread** threads = (std::thread**)alloca(sizeof(std::thread*) * nThreads);
		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j] = new std::thread(parallelAssign);
		}

		Sleep(1); // wait for preparing all thread just in case.

		SetEvent(eventHandle);

		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j]->join();
			delete threads[j];
		}

		bool result = true;

		auto checkConsistency = [&]()
		{
			WaitForSingleObject(eventHandle, INFINITE);
			const u32 MaxLoopCount = min(n, 4096);

			while (result)
			{
				const u32 index = InterlockedIncrement(&counter);
				if (index >= MaxLoopCount)
				{
					break;
				}

				const u32 checkIndex = indexes[index];
				for (u32 j = index + 1; j < MaxLoopCount; ++j)
				{
					if (checkIndex == indexes[j])
					{
						result = false;
						return;
					}
				}
			}
		};

		counter = 0;
		ResetEvent(eventHandle);

		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j] = new std::thread(checkConsistency);
		}

		Sleep(1); // wait for preparing all thread just in case.

		SetEvent(eventHandle);

		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j]->join();
			delete threads[j];
		}

		delete[] indexes;
		CloseHandle(eventHandle);

		if (result == false)
		{
			return false;
		}
	}

	return true;
}

bool parallelAssign64Test()
{
	return parallelAssignTestInternal(64);
}

bool parallelAssign4096Test()
{
	return parallelAssignTestInternal(4096);
}

bool parallelAssign262144Test()
{
	return parallelAssignTestInternal(262144);
}

bool parallelAssign16777216Test()
{
	return parallelAssignTestInternal(16777216);
}

bool parallelAssignAndReleaseTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		HANDLE eventHandle = CreateEvent(nullptr, true, false, L"PARALLEL_TEST_EVENT");
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		u32 counter = 0;
		u32* indexes = new u32[n];

		auto parallelAssign = [&]()
		{
			WaitForSingleObject(eventHandle, INFINITE);

			while (true)
			{
				const u32 index = InterlockedIncrement(&counter);
				if (index >= n)
				{
					break;
				}
				indexes[index] = indexer.assign();
			}
		};

		const auto nThreads = std::thread::hardware_concurrency();
		std::thread** threads = (std::thread**)alloca(sizeof(std::thread*) * nThreads);
		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j] = new std::thread(parallelAssign);
		}

		Sleep(1); // wait for preparing all thread just in case.

		SetEvent(eventHandle);

		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j]->join();
			delete threads[j];
		}

		bool result = true;

		auto parallelRelease = [&]()
		{
			WaitForSingleObject(eventHandle, INFINITE);

			while (true)
			{
				const u32 index = InterlockedIncrement(&counter);
				if (index >= n)
				{
					break;
				}
				indexer.release(indexes[index]);
			}
		};

		counter = 0;
		ResetEvent(eventHandle);

		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j] = new std::thread(parallelRelease);
		}

		Sleep(1); // wait for preparing all thread just in case.

		SetEvent(eventHandle);

		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j]->join();
			delete threads[j];
		}

		delete[] indexes;
		CloseHandle(eventHandle);

		if (result == false)
		{
			return false;
		}

		// check bit state.
		for (u32 j = 0; j < n; ++j)
		{
			const u32 index = indexer.assign();
			if (index != j)
			{
				return false;
			}
		}
	}

	return true;
}

bool parallelAssignAndRelease64Test()
{
	return parallelAssignAndReleaseTestInternal(64);
}

bool parallelAssignAndRelease4096Test()
{
	return parallelAssignAndReleaseTestInternal(4096);
}

bool parallelAssignAndRelease262144Test()
{
	return parallelAssignAndReleaseTestInternal(262144);
}

bool parallelAssignAndRelease16777216Test()
{
	return parallelAssignAndReleaseTestInternal(16777216);
}

bool parallelAssignAndReleaseSimultaneouslyTestInternal(const u32 n)
{
	const u32 init = (u32)(std::log(n) / std::log(64)) - 1;

	for (u32 i = init; i < MaxExponent; ++i)
	{
		FastIndexer indexer(i);
		HANDLE eventHandle = CreateEvent(nullptr, true, false, L"PARALLEL_TEST_EVENT");
		std::cout << "exponent = " << i << ", size = " << sizeof(indexer) + indexer.size() << std::endl;

		const auto nThreads = std::thread::hardware_concurrency();

		auto parallelAssignAndRelease = [&]()
		{
			WaitForSingleObject(eventHandle, INFINITE);

			const u32 local_n = n / nThreads;
			u32* indexes = new u32[local_n];

			for (u32 j = 0; j < local_n; ++j)
			{
				indexes[j] = indexer.assign();
			}

			for (u32 j = 0; j < local_n; ++j)
			{
				indexer.release(indexes[j]);
			}

			delete[] indexes;
		};

		std::thread** threads = (std::thread**)alloca(sizeof(std::thread*) * nThreads);
		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j] = new std::thread(parallelAssignAndRelease);
		}

		Sleep(1); // wait for preparing all thread just in case.

		SetEvent(eventHandle);

		for (u32 j = 0; j < nThreads; ++j)
		{
			threads[j]->join();
			delete threads[j];
		}

		CloseHandle(eventHandle);

		// check bit state.
		for (u32 j = 0; j < n; ++j)
		{
			const u32 index = indexer.assign();
			if (index != j)
			{
				return false;
			}
		}
	}

	return true;
}

bool parallelAssignAndReleaseSimultaneously64Test()
{
	return parallelAssignAndReleaseSimultaneouslyTestInternal(64);
}

bool parallelAssignAndReleaseSimultaneously4096Test()
{
	return parallelAssignAndReleaseSimultaneouslyTestInternal(4096);
}

bool parallelAssignAndReleaseSimultaneously262144Test()
{
	return parallelAssignAndReleaseSimultaneouslyTestInternal(262144);
}

bool parallelAssignAndReleaseSimultaneously16777216Test()
{
	return parallelAssignAndReleaseSimultaneouslyTestInternal(16777216);
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

TEST(RandomTest, Random64Test)
{
	EXPECT_EQ(true, random64Test());
}

TEST(RandomTest, Random4096Test)
{
	EXPECT_EQ(true, random4096Test());
}

TEST(RandomTest, Random262144Test)
{
	EXPECT_EQ(true, random262144Test());
}

TEST(RandomTest, Random16777216Test)
{
	EXPECT_EQ(true, random16777216Test());
}

TEST(ParallelTest , ParallelAssign64Test)
{
	EXPECT_EQ(true, parallelAssign64Test());
}

TEST(ParallelTest, ParallelAssign4096Test)
{
	EXPECT_EQ(true, parallelAssign4096Test());
}

TEST(ParallelTest, ParallelAssign262144Test)
{
	EXPECT_EQ(true, parallelAssign262144Test());
}

TEST(ParallelTest, ParallelAssign16777216Test)
{
	EXPECT_EQ(true, parallelAssign16777216Test());
}

TEST(ParallelTest, ParallelAssignAndRelease64Test)
{
	EXPECT_EQ(true, parallelAssignAndRelease64Test());
}

TEST(ParallelTest, ParallelAssignAndRelease4096Test)
{
	EXPECT_EQ(true, parallelAssignAndRelease4096Test());
}

TEST(ParallelTest, ParallelAssignAndRelease262144Test)
{
	EXPECT_EQ(true, parallelAssignAndRelease262144Test());
}

TEST(ParallelTest, ParallelAssignAndRelease16777216Test)
{
	EXPECT_EQ(true, parallelAssignAndRelease16777216Test());
}

TEST(ParallelTest, ParallelAssignAndReleaseSimultaneously64Test)
{
	EXPECT_EQ(true, parallelAssignAndReleaseSimultaneously64Test());
}

TEST(ParallelTest, ParallelAssignAndReleaseSimultaneously4096Test)
{
	EXPECT_EQ(true, parallelAssignAndReleaseSimultaneously4096Test());
}

TEST(ParallelTest, ParallelAssignAndReleaseSimultaneously262144Test)
{
	EXPECT_EQ(true, parallelAssignAndReleaseSimultaneously262144Test());
}

TEST(ParallelTest, ParallelAssignAndReleaseSimultaneously16777216Test)
{
	EXPECT_EQ(true, parallelAssignAndReleaseSimultaneously16777216Test());
}