#ifndef _FREE_LIST_ALLOCATOR_HPP
#define _FREE_LIST_ALLOCATOR_HPP

#include "pch.h"
#include "linked_list.hpp"

using uchar = unsigned char;

class FreeListAllocator {
private:
	constexpr static uint DEFAULT_ALIGNMENT = 8;

	struct AllocationHeader {
		size_t size;
		uchar adjustment;
	};

	struct FreeBlock {
		size_t size;
		FreeBlock *next;
	};

public:
	FreeListAllocator(size_t);
	~FreeListAllocator();

	FreeListAllocator(FreeListAllocator const&) = delete;
	FreeListAllocator& operator=(FreeListAllocator const&) = delete;
	FreeListAllocator(FreeListAllocator&&) = delete;
	FreeListAllocator& operator=(FreeListAllocator&&) = delete;

	void* alloc(size_t, size_t = DEFAULT_ALIGNMENT);
	void free(void*);
	void init();
	void destroy();
	bool containsAddress(void*);
#ifdef _DEBUG
	void dumpStat() const;
	void dumpBlocks() const;
	uint getBlockSize(void* p) const;
#endif

private:
	bool isAligned(const AllocationHeader* address) const;
	uchar alignForwardAdjustment(const void*, uchar) const;
	uchar alignForwardAdjustmentWithHeader(const void*, uchar) const;
	const void* add(const void*, size_t) const;
	void* subtract(void*, size_t) const;

private:
	FreeBlock* pAlloc;
	size_t nTotalSize;
	size_t nUsed;
	size_t nAlloc;
};

FreeListAllocator::FreeListAllocator(size_t size)
	: nTotalSize(size)
	, nUsed(0)
	, nAlloc(0)
	, pAlloc(nullptr)
{
	assert(size > sizeof(FreeBlock));
}

FreeListAllocator::~FreeListAllocator() {
	destroy();
	nTotalSize = 0;
	nUsed = 0;
	nAlloc = 0;
	assert(pAlloc == nullptr);
}

void FreeListAllocator::init() {
	assert(pAlloc == nullptr);
	pAlloc = (FreeBlock*)(new uchar[nTotalSize]);
	pAlloc->size = nTotalSize;
	pAlloc->next = nullptr;
}

void FreeListAllocator::destroy() {
	assert(pAlloc);
	delete[] pAlloc;
	pAlloc = nullptr;
}

void* FreeListAllocator::alloc(size_t size, size_t alignment) {
	assert(pAlloc);
	assert(alignment >= 8);
	assert(size != 0);

	FreeBlock* prev_free_block = nullptr;
	FreeBlock* free_block = reinterpret_cast<FreeBlock*>(pAlloc);

	FreeBlock* best_fit_prev = nullptr;
	FreeBlock* best_fit = nullptr;
	uchar	   best_fit_adjustment = 0;
	size_t     best_fit_total_size = 0;

	//Find best fit
	while (free_block != nullptr) {
		//Calculate adjustment needed to keep object correctly aligned
		uchar adjustment = alignForwardAdjustmentWithHeader(free_block, alignment);
		size_t total_size = size + adjustment;

		//If its an exact match use this free block
		if (free_block->size == total_size) {
			best_fit_prev = prev_free_block;
			best_fit = free_block;
			best_fit_adjustment = adjustment;
			best_fit_total_size = total_size;
			break;
		}

		//If its a better fit switch
		if (free_block->size > total_size && (best_fit == nullptr || free_block->size < best_fit->size)) {
			best_fit_prev = prev_free_block;
			best_fit = free_block;
			best_fit_adjustment = adjustment;
			best_fit_total_size = total_size;
		}

		prev_free_block = free_block;
		free_block = free_block->next;
	}

	if (best_fit == nullptr) {
		return nullptr;
	}

	//If allocations in the remaining memory will be impossible
	if (best_fit->size - best_fit_total_size <= sizeof(AllocationHeader)) {
		//Increase allocation size instead of creating a new FreeBlock
		best_fit_total_size = best_fit->size;

		if (best_fit_prev != nullptr) {
			best_fit_prev->next = best_fit->next;
		} else {
			pAlloc = best_fit->next;
		}
	} else {
		//Prevent new block from overwriting best fit block info
		assert(best_fit_total_size > sizeof(FreeBlock));

		//Else create a new FreeBlock containing remaining memory
		FreeBlock* new_block = (FreeBlock*)(add(best_fit, best_fit_total_size));
		new_block->size = best_fit->size - best_fit_total_size;
		new_block->next = best_fit->next;

		if (best_fit_prev != nullptr) {
			best_fit_prev->next = new_block;
		} else {
			pAlloc = new_block;
		}
	}

	uintptr_t aligned_address = (uintptr_t)best_fit + best_fit_adjustment;

	AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
	header->size = best_fit_total_size;
	header->adjustment = best_fit_adjustment;

	assert(isAligned(header));

	nUsed += best_fit_total_size;
	++nAlloc;

	assert(alignForwardAdjustment((void*)aligned_address, alignment) == 0);
	return (void*)aligned_address;
}

void FreeListAllocator::free(void* p) {
	assert(p != nullptr);

	AllocationHeader* header = (AllocationHeader*)subtract(p, sizeof(AllocationHeader));

	uintptr_t   block_start = reinterpret_cast<uintptr_t>(p) - header->adjustment;
	size_t		block_size = header->size;
	uintptr_t   block_end = block_start + block_size;

	FreeBlock* prev_free_block = nullptr;
	FreeBlock* free_block = pAlloc;

	while (free_block != nullptr) {
		if ((uintptr_t)free_block >= block_end) {
			break;
		}

		prev_free_block = free_block;
		free_block = free_block->next;
	}

	if (prev_free_block == nullptr) {
		prev_free_block = (FreeBlock*)block_start;
		prev_free_block->size = block_size;
		prev_free_block->next = pAlloc;

		pAlloc = prev_free_block;
	} else if ((uintptr_t)prev_free_block + prev_free_block->size == block_start) {
		prev_free_block->size += block_size;
	} else {
		FreeBlock* temp = (FreeBlock*)block_start;
		temp->size = block_size;
		temp->next = prev_free_block->next;

		prev_free_block->next = temp;
		prev_free_block = temp;
	}

	assert(prev_free_block != nullptr);

	if ((uintptr_t)prev_free_block + prev_free_block->size == (uintptr_t)prev_free_block->next) {
		prev_free_block->size += prev_free_block->next->size;
		prev_free_block->next = prev_free_block->next->next;
	}

	--nAlloc;
	nUsed -= block_size;
}

uchar FreeListAllocator::alignForwardAdjustment(const void* address, uchar alignment) const {
	uchar adjustment = alignment - (reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1));

	if (adjustment == alignment) {
		return 0;
	}

	return adjustment;
}

uchar FreeListAllocator::alignForwardAdjustmentWithHeader(const void* address, uchar alignment) const {
	if (__alignof(AllocationHeader) > alignment) {
		alignment = __alignof(AllocationHeader);
	}

	uchar adjustment = sizeof(AllocationHeader) + alignForwardAdjustment(add(address, sizeof(AllocationHeader)), alignment);
	return adjustment;
}

bool FreeListAllocator::containsAddress(void* p) {
	return p < pAlloc && p >= subtract(pAlloc, nUsed);
}

const void* FreeListAllocator::add(const void* p, size_t x) const {
	return (const void*)(reinterpret_cast<uintptr_t>(p) + x);
}

void* FreeListAllocator::subtract(void* p, size_t x) const {
	return (void*)(reinterpret_cast<uintptr_t>(p) - x);
}

bool FreeListAllocator::isAligned(const AllocationHeader* address) const {
	return alignForwardAdjustment(address, __alignof(AllocationHeader)) == 0;
}

#ifdef _DEBUG
void FreeListAllocator::dumpStat() const {
	cout << "TOTAL SIZE:\t" << nTotalSize << endl;
	cout << "USED SIZE:\t" << nUsed << endl;
	cout << "ALLOCATIONS COUNT:\t" << nAlloc << endl;
	FreeBlock* it = pAlloc;
	int i = 0;

	while (it != nullptr) {
		cout << "=========================================================" << endl;
		cout << "#" << (i + 1) << "\t0x" << &it << "\tsize:\t" << it->size << endl;
		it = it->next;
		++i;
	}

	cout << endl;
}

void FreeListAllocator::dumpBlocks() const {

}

uint FreeListAllocator::getBlockSize(void* p) const {
	auto ptr = reinterpret_cast<FreeBlock*>(p);
	return ptr->size;
}
#endif	//!_DEBUG

#endif	//!_FREE_LIST_ALLOCATOR_HPP