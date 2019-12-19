#pragma once
#include "pch.h"
using namespace std;

constexpr size_t DEFAULT_CHUNK_SIZE		= 16;
constexpr size_t DEFAULT_CHUNKS_COUNT	= 256;

class FSA {
private:
#pragma pack(push, 8)
	struct Chunk {
		Chunk* pNext = nullptr;
	};
#pragma pack(pop)
public:
	FSA(size_t = DEFAULT_CHUNK_SIZE, size_t = DEFAULT_CHUNKS_COUNT);
	virtual ~FSA();

	virtual void* alloc(size_t size);
	virtual void free(void* p);
	virtual void init();
	virtual void destroy();
#ifdef _DEBUG
	virtual void dumpStat() const;
	virtual void dumpBlocks() const;
#endif

private:
	Chunk* allocatePool(size_t);

private:
	Chunk* pAlloc;
	size_t chunkSize;
	size_t chunksCount;
#ifdef _DEBUG
	size_t usedChunksCount;
#endif
};

class FLA {
public:
	FLA();
	virtual ~FLA();

	virtual void* alloc(size_t size);
	virtual void free(void* p);
	virtual void init();
	virtual void destroy();
#ifdef _DEBUG
	virtual void dumpStat() const;
	virtual void dumpBlocks() const;
#endif

private:

};

FSA::FSA(size_t chunkSize, size_t chunksCount) :
	pAlloc(nullptr),
#ifdef _DEBUG
	usedChunksCount(0),
#endif
	chunkSize(chunkSize),
	chunksCount(chunksCount) {

#ifdef _DEBUG
	printf_s("FSA(chunk size: %i, chunks count: %i)\n", (int)chunkSize, (int)chunksCount);
#endif
}

FSA::~FSA() {
#ifdef _DEBUG
	cout << "~FSA()" << endl;
#endif
	destroy();
	assert(pAlloc == nullptr);
}

void FSA::init() {
	assert(pAlloc == nullptr);

	if (pAlloc) {
#ifdef _DEBUG
		cout << "FSA::init(): pAlloc isn't nullptr." << endl;
#endif
		return;
	}

	pAlloc = allocatePool(chunkSize * chunksCount);
}

void FSA::destroy() {
#ifdef _DEBUG
	assert(usedChunksCount == 0);
#endif
	assert(pAlloc);

	delete[] pAlloc;
	pAlloc = nullptr;
}

void* FSA::alloc(size_t size) {
	assert(pAlloc);

#ifndef _DEBUG
	if (size > chunkSize) {
#else
	if (size > chunkSize || usedChunksCount >= chunksCount) {
#endif
		return nullptr;
	}

#ifdef _DEBUG
	++usedChunksCount;
#endif
	void* pFreeChunk = reinterpret_cast<void*>(pAlloc);
	pAlloc = pAlloc->pNext;
	return pFreeChunk;
}

void FSA::free(void* p) {
	assert(pAlloc);
	assert(p);

	Chunk* pUsedChunk = reinterpret_cast<Chunk*>(p);
	pUsedChunk->pNext = pAlloc;
	pAlloc = pUsedChunk;
#ifdef _DEBUG
	--usedChunksCount;
#endif
}

FSA::Chunk* FSA::allocatePool(size_t size) {
	Chunk* pPoolHead = new Chunk[size];
	Chunk* pCurrentChunk = pPoolHead;

	for (int i = 0; i < chunksCount - 1; ++i) {
		pCurrentChunk->pNext = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(pCurrentChunk) + chunkSize);
		pCurrentChunk = pCurrentChunk->pNext;
	}

	pCurrentChunk->pNext = nullptr;
	return pPoolHead;
}

#ifdef _DEBUG
void FSA::dumpStat() const {

}

void FSA::dumpBlocks() const {

}
#endif

FLA::FLA() {
#ifdef _DEBUG
	cout << "FLA()" << endl;
#endif
}

FLA::~FLA() {
#ifdef _DEBUG
	cout << "~FLA()" << endl;
#endif
	destroy();
}

void FLA::init() {

}

void FLA::destroy() {

}

void* FLA::alloc(size_t size) {
	return nullptr;
}

void FLA::free(void* p) {

}

#ifdef _DEBUG
void FLA::dumpStat() const {

}

void FLA::dumpBlocks() const {

}
#endif