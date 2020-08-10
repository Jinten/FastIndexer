#pragma once

#include<array>
#include"typeAlias.h"

namespace ao
{
	class FastIndexer
	{
	public:
		FastIndexer()
		{
			for (u32 i = 0; i < 64; ++i)
			{
				const u32 stage1Index = i;
				mRoot.trees[i] = &mStage1Buffer[stage1Index];

				auto state1 = mRoot.trees[i];
				for (u32 j = 0; j < 64; ++j)
				{
					const u32 stage2Index = (i << 6) + j;
					state1->trees[j] = &mStage2Buffer[stage2Index];

					auto stage2 = state1->trees[j];
					for (u32 k = 0; k < 64; ++k)
					{
						const u32 finalIndex = (i << 12) | (j << 6) | k;
						stage2->trees[k] = &mLeafBuffer[finalIndex];
					}
				}
			}
		}

		void set(const u32 index, const bool state)
		{
			const u64 rootIndex = (index >> 18) & 0x3F;
			const u64 stage1Index = (index >> 12) & 0x3F;
			const u64 stage2Index = (index >> 6) & 0x3F;
			const u64 finalIndex = index & 0x3F;

			auto stage1 = mRoot.trees[rootIndex];
			auto stage2 = stage1->trees[stage1Index];
			auto leaf = stage2->trees[stage2Index];

			const u64 value = state ? 0 : 1;
			const u64 binary = leaf->binary & (~(u64(1) << finalIndex));	// reset
			leaf->binary = binary | (value << finalIndex);					// store

			if (leaf->binary == 0)
			{
				// modify enabled bit area if lower bit area is full.
				stage2->binary &= (~(u64(1) << stage2Index));
				if (stage2->binary == 0)
				{
					stage1->binary &= (~(u64(1) << stage1Index));
					if (stage1->binary == 0)
					{
						mRoot.binary &= (~(u64(1) << rootIndex));
					}
				}
			}
			else
			{
				// modify enabled bit area if there is blank bit at lower bit area.
				const u64 stage2Flags = stage2->binary;
				stage2->binary |= (u64(1) << stage2Index);
				if (stage2Flags == 0)
				{
					const u64 stage1Flags = stage1->binary;
					stage1->binary |= (u64(1) << stage1Index);
					if (stage1Flags == 0)
					{
						mRoot.binary |= (u64(1) << rootIndex);
					}
				}
			}
		}

		bool get(const u32 index)
		{
			const u64 rootIndex = (index >> 18) & 0x3F;
			const u64 stage1Index = (index >> 12) & 0x3F;
			const u64 stage2Index = (index >> 6) & 0x3F;
			const u64 finalIndex = index & 0x3F;

			const u64 result = mRoot.trees[rootIndex]->trees[stage1Index]->trees[stage2Index]->binary & (u64(1) << finalIndex);

			return result == 0 ? true : false;
		}

		u32 getBlankIndex() const
		{
			const u64 rootIndex = bitScanForward(mRoot.binary);
			const u64 stage1Index = bitScanForward(mRoot.trees[rootIndex]->binary);
			const u64 stage2Index = bitScanForward(mRoot.trees[rootIndex]->trees[stage1Index]->binary);
			const u64 finalIndex = bitScanForward(mRoot.trees[rootIndex]->trees[stage1Index]->trees[stage2Index]->binary);
			return static_cast<u32>((rootIndex << 18) | (stage1Index << 12) | (stage2Index << 6) | finalIndex);
		}

	private:
		static constexpr u64 MaxBit = 0xFFFFFFFFFFFFFFFF;

		template<u32 N>
		struct BitTree
		{
			u64 binary;
			std::array<BitTree<N - 1>*, 64> trees;
			BitTree() : binary(MaxBit)
			{
				trees.fill(nullptr);
			}
		};

		template<>
		struct BitTree<0>
		{
			u64 binary;
			BitTree() : binary(MaxBit)
			{

			}
		};

	private:
		BitTree<3> mRoot;
		std::array<BitTree<2>, 64> mStage1Buffer;
		std::array<BitTree<1>, 64 * 64> mStage2Buffer;
		std::array<BitTree<0>, 64 * 64 * 64> mLeafBuffer;

	private:
		static u64 bitScanForward(u64 binary)
		{
			return _BitScanForward((unsigned long*)&binary, 0xFFFFFFFFFFFFFFFF);
		}
	};
}
