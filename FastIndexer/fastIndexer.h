#pragma once

#include<vector>
#include<Windows.h>
#include"typeAlias.h"

namespace ao
{
	class FastIndexer
	{
	public:
		FastIndexer(u32 exponent) : mExponent(exponent), mElementCount(64 << (exponent * 6)), mTerminateExponent(exponent == 0 ? 0 : exponent - 1)
		{
			mBuffer = new u64[mElementCount];

			std::fill(mBuffer, &mBuffer[mElementCount - 1], u64Max);
		}

		~FastIndexer()
		{
			delete[] mBuffer;
		}

		u32 reserve()
		{
			u64 dest;
			bool result = reserveInternal(dest, 0, 0);

			_ASSERT_EXPR(result == true, L"using indexes is full.");

			return u32(dest);
		}

		void release(u32 index)
		{
			releaseInternal(u64(index), 0, 0);
		}

	private:
		static constexpr u64 u64Max = UINT64_MAX;

	private:
		u32 mExponent;
		u32 mTerminateExponent;
		u32 mElementCount;
		u64* mBuffer;

	private:
		static u64 bitScanForward(u64 binary)
		{
			DWORD result;
			_BitScanForward64(&result, binary);

			return result;
		}

		bool reserveInternal(u64& destIndex, const u64 expIndex, const u64 binIndex)
		{
			while (true)
			{
				u64 src = mBuffer[binIndex];
				if (src == 0)
				{
					// this reserved area is full.
					return false;
				}

				if (expIndex == mTerminateExponent)
				{
					destIndex = bitScanForward(src);
					const u64 dest = src & (~(1 << destIndex)); // create the mask.
					const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[binIndex], dest, src);
					if (result == src)
					{
						// succeed
						return true;
					}

					if (result == 0)
					{
						// reserved area is already full.
						return false;
					}

					// other thread stored value, so need to retry;
				}
				else
				{
					destIndex = bitScanForward(src);

					u64 index;
					const u64 ofsBinIndex = u64(1) + (expIndex == 0 ? 0 : (64 << (6 * (expIndex - 1))));
					const u64 baseBinIndex = ofsBinIndex + 64 * destIndex;

					const bool result = reserveInternal(index, expIndex + 1, baseBinIndex);
					if (result)
					{
						destIndex = (destIndex << (6 * (mTerminateExponent - expIndex))) + index;

						return true;
					}
					else
					{
						// child hierarchy binary is full.
						src = mBuffer[binIndex];
						const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[binIndex], 0, src);
						if (result == src)
						{
							// keep going.
							return true;
						}

						// other thread released bit. so need to retry.
					}
				}
			}

			// kill a warning.
			return false;
		}

		void releaseInternal(const u64 index, const u64 expIndex, const u64 binIndex)
		{
			while (true)
			{
				u64 src = mBuffer[binIndex];
				if (expIndex == mTerminateExponent)
				{
					const u64 dest = src | (u64(1) << (index % 64));
					const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[binIndex], dest, src);

					if (result == src)
					{
						// succeed
						return;
					}

					// other thread modified binary. so need to retry.
				}
				else
				{
					const u64 ofsBinIndex = expIndex == 0 ? 0 : (1 << (6 * expIndex));
					const u64 nextBinIndex = expIndex == 0 ? 0 : index / ofsBinIndex;

					releaseInternal(index, expIndex + 1, nextBinIndex + 1);

					while (true)
					{
						src = mBuffer[binIndex];
						if (src == 0)
						{
							const u64 exchange = u64(1) << nextBinIndex; // enable least one bit.
							const u64 result = InterlockedCompareExchange64((long long*)&mBuffer[binIndex], exchange, src);
							if (result == src)
							{
								// succeed
								return;
							}

							// other thread modified binary. so need to retry.
						}
						else
						{
							// this bit area is not full already.
							return;
						}
					}
				}
			}
		}
	};
}
