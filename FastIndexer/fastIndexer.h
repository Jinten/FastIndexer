#pragma once

#include<atomic>
#include<algorithm>
#include<Windows.h>
#include"typeAlias.h"

namespace ao
{
	class FastIndexer
	{
	public:
		FastIndexer(u32 exponent) : mExponent(exponent)
		{
			mElementCount = 1;
			for (u32 i = 0; i < exponent; ++i)
			{
				mElementCount += 1 << (6 * exponent);
			}

			mBuffer = new u64[mElementCount];

			std::fill(mBuffer, &mBuffer[mElementCount], u64Max);
		}

		~FastIndexer()
		{
			delete[] mBuffer;
		}

		u32 assign()
		{
			u64 dest;
			ResultType result = ResultType::Invalid;

			while (true)
			{
				result = assignInternal(dest, 0, 0, 1);
				if (result == ResultType::Success || result == ResultType::Full)
				{
					break;
				}

				_ASSERT_EXPR(result != ResultType::Over, L"Cannot reserve a index any more. Used index is full.");
			}

			return u32(dest);
		}

		void release(u32 index)
		{
			releaseInternal(u64(index), 0, 0, 1);
		}

		u64 size()
		{
			return sizeof(u64) * mElementCount;
		}

	private:
		static constexpr u64 u64Max = UINT64_MAX;

		enum class ResultType
		{
			Invalid = -1,
			Success,
			Full,
			Retry,
			Over,
		};

	private:
		u32 mExponent;
		u32 mElementCount;
		u64* mBuffer;

	private:
		static u64 bitScanForward(u64 binary)
		{
			DWORD result;
			_BitScanForward64(&result, binary);

			return result;
		}

		ResultType assignInternal(u64& destIndex, const u64 expIndex, const u64 bufIndex, const  u64 bufOffset)
		{
			while (true)
			{
				u64 src = mBuffer[bufIndex];
				if (src == 0)
				{
					if (expIndex == 0)
					{
						return ResultType::Over;
					}
					// this reserved area is full by other thread modifying.
					return ResultType::Retry;
				}

				if (expIndex == mExponent)
				{
					destIndex = bitScanForward(src);

					const u64 dest = src & (~(u64(1) << destIndex)); // create the mask.
					const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[bufIndex], dest, src);

					if (result == src)
					{
						// succeed
						// check full or has blank with interlocked api.
						src = mBuffer[bufIndex];
						const u64 currentValue = InterlockedCompareExchange64((long long*)&mBuffer[bufIndex], src, src);

						// if bit area is full, need to modify parent bit.
						return currentValue != 0 ? ResultType::Success : ResultType::Full;
					}

					// other thread stored value, so need to retry;
				}
				else
				{
					destIndex = bitScanForward(src);

					const u64 nextBufIndex = bufOffset + destIndex;
					const u64 nextBufOffset = (u64(64) << (6 * expIndex)) * (destIndex + 1);

					u64 index;
					const ResultType result = assignInternal(index, expIndex + 1, nextBufIndex, bufOffset + nextBufOffset);

					switch (result)
					{
					case ResultType::Success:
					{
						destIndex = (destIndex << (6 * (mExponent - expIndex))) + index;
						return ResultType::Success;
					}
					case ResultType::Full:
					{
						const u64 binIndex = destIndex;

						destIndex = (destIndex << (6 * (mExponent - expIndex))) + index;

						while (true)
						{
							src = mBuffer[bufIndex];

							const u64 mask = src & (~(u64(1) << binIndex));
							const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[bufIndex], mask, src);

							if (result == src)
							{
								// success
								// check full or has blank with interlocked api.
								src = mBuffer[bufIndex];
								const u64 currentValue = InterlockedCompareExchange64((long long*)&mBuffer[bufIndex], src, src);

								// if bit area is full, need to modify parent bit.
								return currentValue != 0 ? ResultType::Success : ResultType::Full;
							}

							// other thread released bit. so need to retry.
						}
						break;
					}
					case ResultType::Retry:
						// child bit area was full, so need to retry from parent bit area to next child bit area.
						while (true)
						{
							src = mBuffer[bufIndex];

							const u64 mask = src & (~(u64(1) << destIndex));
							const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[bufIndex], mask, src);

							if (result == src)
							{
								// succeed
								// retry to reserve to child bit area.
								break;
							}

							if (result != 0)
							{
								// this bit area is not full already.
								// retry to reserve to child bit area.
								break;
							}

							// other thread released bit. so need to retry.
						}
						break;
					}
				}
			}

			// kill a warning.
			return ResultType::Invalid;
		}

		void releaseInternal(const u64 index, const u64 expIndex, const u64 bufIndex, const u64 bufOffset)
		{
			while (true)
			{
				u64 src = mBuffer[bufIndex];
				if (expIndex == mExponent)
				{
					const u64 dest = src | (u64(1) << (index % 64));
					const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[bufIndex], dest, src);

					if (result == src)
					{
						// succeed
						return;
					}

					// other thread modified binary. so need to retry.
				}
				else
				{
					const u64 binIndex = (index / (u64(1) << (6 * (mExponent - expIndex)))) % 64;
					const u64 nextBufIndex = bufOffset + binIndex;
					const u64 nextBufOffset = (u64(64) << (6 * expIndex)) * (binIndex + 1);

					releaseInternal(index, expIndex + 1, nextBufIndex, bufOffset + nextBufOffset);

					const u64 bin = u64(1) << binIndex;

					while (true)
					{
						// check current value
						src = mBuffer[bufIndex];
						if ((src & bin) != 0)
						{
							// there is still usable bit or other thread released and modified parent bit already.
							return;
						}
						else
						{
							// child bit area is full used.
							const u64 exchange = src | bin; // enable least one bit because released child bit.
							const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[bufIndex], exchange, src);

							if (result == src)
							{
								// succeed
								return;
							}
						}

						// other thread modified binary. so need to retry.
					}
				}
			}
		}
	};
}
