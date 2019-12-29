#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include "pch.h"
#include "fixed_size_allocator.hpp"
#include "free_list_allocator.hpp"
using namespace std;

class MemoryAllocator {
private:
	constexpr static uint POOLS_COUNT			= 6;
	constexpr static uint POOL_MIN_CHUNK_SIZE	= 16;
	constexpr static uint POOL_MAX_CHUNK_SIZE	= 512;
	constexpr static uint POOL_CHUNKS_COUNT		= 1024;
	constexpr static uint TEN_MB				= 10485760;

public:
	MemoryAllocator();
	virtual ~MemoryAllocator();

	MemoryAllocator(MemoryAllocator const&) = delete;
	MemoryAllocator& operator=(MemoryAllocator const&) = delete;
	MemoryAllocator(MemoryAllocator&&) = delete;
	MemoryAllocator& operator=(MemoryAllocator&&) = delete;

	virtual void init();
	virtual void destroy();
	virtual void* alloc(size_t);
	virtual void free(void*);
#ifdef _DEBUG
	virtual void dumpStat() const;
	virtual void dumpBlocks() const;
#endif

private:
	size_t getPoolIndex(size_t);

private:
	vector<FixedSizeAllocator*>	memoryPools;
	vector<FreeListAllocator*> memoryLists;
	vector<void*> memoryHeap;
#ifdef _DEBUG
	uint nTotalMemUsed;
	uint nTotalMemFreed;
#endif
};

MemoryAllocator::MemoryAllocator()
#ifdef _DEBUG
	: nTotalMemUsed(0)
	, nTotalMemFreed(0)
#endif
{
	for (size_t i = 0; i < POOLS_COUNT; ++i) {
		FixedSizeAllocator* pPool = new FixedSizeAllocator;
		memoryPools.push_back(pPool);
	}

	memoryLists.push_back(new FreeListAllocator(TEN_MB));
}

MemoryAllocator::~MemoryAllocator() {
	destroy();
	assert(memoryPools.size() == 0);
	assert(memoryLists.size() == 0);
}

void MemoryAllocator::init() {
	// init 6 pools with chunk sizes: 16, 32, 64, 128, 256, 512
	for (size_t i = 0; i < memoryPools.size(); ++i) {
		uint chunk_size = (uint)pow(2, i + 4);
		memoryPools[i]->init(chunk_size, POOL_CHUNKS_COUNT);
	}

	memoryLists[0]->init();
}

void MemoryAllocator::destroy() {
	for (size_t i = 0; i < memoryPools.size(); ++i) {
		delete memoryPools[i];
	}
	memoryPools.clear();

	for (size_t i = 0; i < memoryLists.size(); ++i) {
		delete memoryLists[i];
	}
	memoryLists.clear();

#ifdef _DEBUG
	nTotalMemFreed = 0;
	nTotalMemUsed = 0;
#endif
}

void* MemoryAllocator::alloc(size_t size) {
	if (size == 0) {
		return nullptr;
	}

	uint alignedSize = 8;
	if (size > 8) {
		auto closestPower = ceil(log(size) / log(2));
		alignedSize = (uint)pow(2, closestPower);
	}

	if (alignedSize >= POOL_MIN_CHUNK_SIZE && alignedSize <= POOL_MAX_CHUNK_SIZE) {
		size_t index = getPoolIndex(alignedSize);
		if (index > POOLS_COUNT) {
			return nullptr;
		}
#ifdef _DEBUG
		nTotalMemUsed += alignedSize;
#endif
		return memoryPools[index]->alloc();
	}

	if (alignedSize <= TEN_MB && alignedSize > POOL_MAX_CHUNK_SIZE) {
		void* p = nullptr;

		for (size_t i = 0; i < memoryLists.size(); ++i) {
			p = memoryLists[0]->alloc(alignedSize);
		}

		if (p != nullptr) {
#ifdef _DEBUG
			nTotalMemUsed += alignedSize;
#endif
			return p;
		} else {
			auto freeList = new FreeListAllocator(TEN_MB);
			memoryLists.push_back(freeList);
#ifdef _DEBUG
			nTotalMemUsed += alignedSize;
#endif
			return freeList->alloc(alignedSize);
		}
	}

	if (alignedSize > TEN_MB) {
		uchar* p = new uchar[alignedSize];
		memoryHeap.push_back(p);
#ifdef _DEBUG
		nTotalMemUsed += alignedSize;
#endif
		return p;
	}

	return nullptr;
}

void MemoryAllocator::free(void* p) {
	if (p == nullptr) {
		return;
	}

	for (size_t i = 0; i < memoryPools.size(); ++i) {
		if (memoryPools[i]->containsAddress(p)) {
			memoryPools[i]->free(p);
#ifdef _DEBUG
			nTotalMemFreed += memoryPools[i]->getChunkSize();
#endif
			return;
		}
	}

	for (size_t i = 0; i < memoryLists.size(); ++i) {
		if (memoryLists[i]->containsAddress(p)) {
#ifdef _DEBUG
			nTotalMemFreed += memoryLists[i]->getBlockSize(p);
#endif
			memoryLists[i]->free(p);
			return;
		}
	}

	for (size_t i = 0; i < memoryHeap.size(); ++i) {
		if (memoryHeap[i] == p) {
			delete[] p;
			p = nullptr;
			memoryHeap.erase(memoryHeap.begin() + i);
			return;
		}
	}
}

size_t MemoryAllocator::getPoolIndex(size_t size) {
	return (size_t)(log(size) / log(2)) - 4;
}

#ifdef _DEBUG
void MemoryAllocator::dumpStat() const {
	for (size_t i = 0; i < memoryPools.size(); ++i) {
		cout << "POOL #" << i + 1 << endl;
		cout << "=========================================================" << endl;
		memoryPools[i]->dumpStat();
	}

	for (size_t i = 0; i < memoryLists.size(); ++i) {
		cout << "FREE LIST #" << i + 1 << endl;
		cout << "=========================================================" << endl;
		memoryLists[i]->dumpStat();
	}

	cout << "TOTAL MEMORY ALLOCATED:\t" << nTotalMemUsed << endl;
	cout << "TOTAL MEMORY FREED:\t" << nTotalMemFreed << endl;
	cout << endl;
}

void MemoryAllocator::dumpBlocks() const {

}
#endif	//!_DEBUG

#endif	//!_MEMORY_HPP