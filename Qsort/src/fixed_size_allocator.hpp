#ifndef _FIXED_SIZE_ALLOCATOR_HPP
#define _FIXED_SIZE_ALLOCATOR_HPP

#include "pch.h"

using uint	= unsigned int;
using uchar = unsigned char;

class FixedSizeAllocator {
public:
	FixedSizeAllocator();
	~FixedSizeAllocator();

	FixedSizeAllocator(FixedSizeAllocator const&) = delete;
	FixedSizeAllocator& operator=(FixedSizeAllocator const&) = delete;
	FixedSizeAllocator(FixedSizeAllocator&&) = delete;
	FixedSizeAllocator& operator=(FixedSizeAllocator&&) = delete;

	void init(uint, uint);
	void destroy();
	void* alloc();
	void free(void*);
	bool containsAddress(void*);
#ifdef _DEBUG
	void dumpStat() const;
	void dumpBlocks() const;
	uint getChunkSize() const { return nChunkSize; }
#endif

private:
	uchar* addressFromIndex(uint) const;
	uint indexFromAddress(const uchar*) const;

private:
	uint nChunksCount;
	uint nChunkSize;
	uint nFreeChunksCount;
	uint nAllocChunksCount;
	uchar* pAlloc;
	uchar* pNext;

};

FixedSizeAllocator::FixedSizeAllocator()
	: nChunksCount(0)
	, nChunkSize(0)
	, nFreeChunksCount(0)
	, nAllocChunksCount(0)
	, pAlloc(nullptr)
	, pNext(nullptr)
{}

FixedSizeAllocator::~FixedSizeAllocator() {
	destroy();
	assert(pAlloc == nullptr);
}

void FixedSizeAllocator::init(uint _chunkSize, uint _chunksCount) {
	assert(pAlloc == nullptr);
	nChunkSize			= _chunkSize;
	nChunksCount		= _chunksCount;
	pAlloc				= new uchar[(size_t)nChunkSize * (size_t)nChunksCount];
	nFreeChunksCount	= nChunksCount;
	pNext				= pAlloc;
}

void FixedSizeAllocator::destroy() {
	assert(pAlloc);
	
	if (pAlloc) {
		delete[] pAlloc;
		pAlloc = nullptr;
		pNext = nullptr;

		nFreeChunksCount = nChunksCount;
		nAllocChunksCount = 0;
	}
}

void* FixedSizeAllocator::alloc() {
	assert(pAlloc);
	if (nAllocChunksCount < nChunksCount) {
		uint* p = reinterpret_cast<uint*>(addressFromIndex(nAllocChunksCount));
		*p = nAllocChunksCount + 1;
		++nAllocChunksCount;
	}

	void* allocAddr = nullptr;
	if (nFreeChunksCount > 0) {
		allocAddr = reinterpret_cast<void*>(pNext);
		--nFreeChunksCount;
		if (nFreeChunksCount != 0) {
			pNext = addressFromIndex(*reinterpret_cast<uint*>(pNext));
		} else {
			pNext = nullptr;
		}
	}

	return allocAddr;
}

void FixedSizeAllocator::free(void* p) {
	assert(pAlloc);

	if (pNext != nullptr) {
		(*reinterpret_cast<uint*>(p)) = indexFromAddress(pNext);
	} else {
		(*reinterpret_cast<uint*>(p)) = nChunksCount;
	}

	pNext = reinterpret_cast<uchar*>(p);
	++nFreeChunksCount;
}

bool FixedSizeAllocator::containsAddress(void* p) {
	return p >= pAlloc && p < pAlloc + nChunksCount;
}

uchar* FixedSizeAllocator::addressFromIndex(uint index) const {
	return pAlloc + ((size_t)index * (size_t)nChunkSize);
}

uint FixedSizeAllocator::indexFromAddress(const uchar* pAddress) const {
	return (((uint)(pAddress - pAlloc)) / nChunkSize);
}

#ifdef _DEBUG
void FixedSizeAllocator::dumpStat() const {
	cout << "TOTAL CHUNKS COUNT:\t" << nChunksCount << endl;
	cout << "FREE CHUNKS COUNT:\t" << nFreeChunksCount << endl;
	cout << "ALLOCATED CHUNKS COUNT:\t" << nAllocChunksCount << endl;

	for (size_t i = 0; i < nAllocChunksCount; ++i) {
		uchar* chunk = addressFromIndex(i);
		cout << "=========================================================" << endl;
		cout << "#" << (i + 1) << "\t0x" << &chunk << "\tsize:\t" << nChunkSize << endl;
	}

	cout << endl;
}

void FixedSizeAllocator::dumpBlocks() const {

}
#endif	//!_DEBUG

#endif	//!_FIXED_SIZE_ALLOCATOR_HPP