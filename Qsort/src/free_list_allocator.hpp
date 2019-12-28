#ifndef _FREE_LIST_ALLOCATOR_HPP
#define _FREE_LIST_ALLOCATOR_HPP

#include "pch.h"
#include "linked_list.hpp"

using uchar = unsigned char;

class FreeListAllocator {
private:
	constexpr static uint DEFAULT_ALIGNMENT = 8;

	struct FreeHeader {
		size_t blockSize;
	};

	struct AllocationHeader {
		size_t blockSize;
		char padding;
	};

	using Node = LinkedList<FreeHeader>::Node;

public:
	FreeListAllocator(const size_t);
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
	void coalescence(Node*, Node*);
	void find(const size_t, const size_t, size_t&, Node*&, Node*&);
	const size_t calculatePadding(const size_t, const size_t);
	const size_t calculatePadding(const size_t, const size_t, const size_t);

private:
	void* pAlloc;
	LinkedList<FreeHeader> freeList;
	size_t nTotalSize;
	size_t nUsed;
	size_t nPeak;
};

FreeListAllocator::FreeListAllocator(const size_t totalSize)
	: nTotalSize(totalSize)
	, nUsed(0)
	, nPeak(0)
	, pAlloc(nullptr)
{}

FreeListAllocator::~FreeListAllocator() {
	destroy();
	assert(pAlloc == nullptr);
}

void FreeListAllocator::init() {
	assert(pAlloc == nullptr);
	pAlloc = new uchar[nTotalSize];

	Node* root = reinterpret_cast<Node*>(pAlloc);
	root->data.blockSize = nTotalSize;
	root->pNext = nullptr;
	freeList.pHead = nullptr;
	freeList.insert(nullptr, root);
}

void FreeListAllocator::destroy() {
	assert(pAlloc);
	
	if (pAlloc) {
		delete[] pAlloc;
		pAlloc = nullptr;
	}
}

void* FreeListAllocator::alloc(size_t size, size_t alignment) {
	assert(size >= sizeof(Node));
	assert(alignment >= 8);
	assert(pAlloc);

	const size_t allocHeaderSize = sizeof(AllocationHeader);
	const size_t freeHeaderSize = sizeof(FreeHeader);

	size_t padding;
	Node* affectedNode;
	Node* previousNode;

	find(size, alignment, padding, previousNode, affectedNode);
	assert(affectedNode != nullptr);

	const size_t alignPadding = padding - allocHeaderSize;
	const size_t requiredSize = size + padding;
	const size_t rest = affectedNode->data.blockSize - requiredSize;

	if (rest > 0) {
		Node* newFreeNode = reinterpret_cast<Node*>((size_t)affectedNode + requiredSize);
		newFreeNode->data.blockSize = rest;
		freeList.insert(affectedNode, newFreeNode);
	}

	freeList.remove(previousNode, affectedNode);

	const size_t headerAddress = (size_t)affectedNode + alignPadding;
	const size_t dataAddress = headerAddress + allocHeaderSize;
	((AllocationHeader*)headerAddress)->blockSize = requiredSize;
	((AllocationHeader*)headerAddress)->padding = (char)alignPadding;

	nUsed += requiredSize;
	nPeak = max(nPeak, nUsed);
	return reinterpret_cast<void*>(dataAddress);
}

void FreeListAllocator::free(void* p) {
	const size_t currentAddress = (size_t)p;
	const size_t headerAddress = currentAddress - sizeof(AllocationHeader);
	const AllocationHeader* allocationHeader { reinterpret_cast<AllocationHeader*>(headerAddress) };

	Node* pFreeNode = reinterpret_cast<Node*>(headerAddress);
	pFreeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->padding;
	pFreeNode->pNext = nullptr;

	Node* pIt = freeList.pHead;
	Node* pItPrev = nullptr;

	while (pIt != nullptr) {
		if (p < pIt) {
			freeList.insert(pItPrev, pFreeNode);
			break;
		}

		pItPrev = pIt;
		pIt = pIt->pNext;
	}

	nUsed -= pFreeNode->data.blockSize;
	coalescence(pItPrev, pFreeNode);
}

void FreeListAllocator::coalescence(Node* prevNode, Node* freeNode) {
	if (freeNode->pNext != nullptr && (size_t)freeNode + freeNode->data.blockSize == (size_t)freeNode->pNext) {
		freeNode->data.blockSize += freeNode->pNext->data.blockSize;
		freeList.remove(freeNode, freeNode->pNext);
	}

	if (prevNode != nullptr && (size_t)prevNode + prevNode->data.blockSize == (size_t)freeNode) {
		prevNode->data.blockSize += freeNode->data.blockSize;
		freeList.remove(prevNode, freeNode);
	}
}

// Find Best-Fit
void FreeListAllocator::find(const size_t size, const size_t alignment, size_t& padding, Node*& previousNode, Node*& foundNode) {
	constexpr size_t smallestDiff = numeric_limits<size_t>::max();
	Node* bestBlock = nullptr;
	Node* it = freeList.pHead;
	Node* itPrev = nullptr;

	while (it != nullptr) {
		padding = calculatePadding((size_t)it, alignment, sizeof(AllocationHeader));
		const size_t requiredSpace = size + padding;
		
		if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff)) {
			bestBlock = it;
		}

		itPrev = it;
		it = it->pNext;
	}

	previousNode = itPrev;
	foundNode = bestBlock;
}

const size_t FreeListAllocator::calculatePadding(const size_t baseAddress, const size_t alignment) {
	const size_t multiplier = (baseAddress / alignment) + 1;
	const size_t alignedAddress = multiplier * alignment;
	const size_t padding = alignedAddress - baseAddress;
	return padding;
}

const size_t FreeListAllocator::calculatePadding(const size_t baseAddress, const size_t alignment, const size_t headerSize) {
	size_t padding = calculatePadding(baseAddress, alignment);
	size_t neededSpace = headerSize;

	if (padding < neededSpace) {
		neededSpace -= padding;

		if (neededSpace % alignment > 0) {
			padding += alignment * (1 + (neededSpace / alignment));
		} else {
			padding += alignment * (neededSpace / alignment);
		}
	}

	return padding;
}

bool FreeListAllocator::containsAddress(void* p) {
	return p >= pAlloc && p < reinterpret_cast<uchar*>(pAlloc) + nTotalSize;
}

#ifdef _DEBUG
void FreeListAllocator::dumpStat() const {
	cout << "TOTAL SIZE:\t" << nTotalSize << endl;
	cout << "USED SIZE:\t" << nUsed << endl;
	cout << "PEAK SIZE:\t" << nPeak << endl;
	Node* it = freeList.pHead;
	int i = 0;

	while (it != nullptr) {
		cout << "=========================================================" << endl;
		cout << "#" << (i + 1) << "\t0x" << &it << "\tsize:\t" << it->data.blockSize << endl;
		it = it->pNext;
		++i;
	}

	cout << endl;
}

void FreeListAllocator::dumpBlocks() const {

}
#endif	//!_DEBUG

uint FreeListAllocator::getBlockSize(void* p) const {
	Node* it = freeList.pHead;
	while (it != nullptr) {
		if (it == p) {
			return it->data.blockSize;
		}
		it = it->pNext;
	}
}

#endif	//!_FREE_LIST_ALLOCATOR_HPP