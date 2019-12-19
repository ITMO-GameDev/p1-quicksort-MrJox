#pragma once
#include "pch.h"
using namespace std;

constexpr auto DEFAULT_CHUNK_SIZE	= 64;
constexpr auto DEFAULT_CHUNKS_COUNT	= 4096;

class MemoryAllocator {
protected:
	struct Chunk {
		Chunk* pNext = nullptr;
	};

public:
	MemoryAllocator();
	MemoryAllocator(size_t, size_t);
	virtual ~MemoryAllocator();

	MemoryAllocator(MemoryAllocator const&) = delete;
	MemoryAllocator& operator=(MemoryAllocator const&) = delete;
	MemoryAllocator(MemoryAllocator&&) = delete;
	MemoryAllocator& operator=(MemoryAllocator&&) = delete;

	virtual void init() = 0;
	virtual void destroy() = 0;

	virtual void* alloc(size_t) = 0;
	virtual void free(void*) = 0;

#ifdef _DEBUG
	virtual void dumpStat() const = 0;
	virtual void dumpBlocks() const = 0;
#endif

protected:
	Chunk*	pAlloc;
	void*	pPool;
	size_t	chunkSize;
	size_t	chunksCount;
	size_t	usedChunksCount;
#ifdef _DEBUG
	size_t	freeChunksCount;
	size_t	allocChunksCount;
#endif
	bool	isInit;
};

class FSA final : public MemoryAllocator {
public:
	FSA();
	virtual ~FSA();

	virtual void init() final;
	virtual void destroy() final;

	virtual void* alloc(size_t size) override;
	virtual void free(void* p) override;

#ifdef _DEBUG
	virtual void dumpStat() const override;
	virtual void dumpBlocks() const override;
#endif

};

class FLA final : public MemoryAllocator {
public:
	FLA();
	virtual ~FLA();

	virtual void init() final;
	virtual void destroy() final;

	virtual void* alloc(size_t size) override;
	virtual void free(void* p) override;

#ifdef _DEBUG
	virtual void dumpStat() const override;
	virtual void dumpBlocks() const override;
#endif

};

MemoryAllocator::MemoryAllocator() :
	MemoryAllocator(DEFAULT_CHUNK_SIZE, DEFAULT_CHUNKS_COUNT)
{}

MemoryAllocator::MemoryAllocator(size_t chunkSize, size_t chunksCount) :
	  isInit(false)
	, pAlloc(nullptr)
	, pPool(nullptr)
	, chunkSize(chunkSize)
	, chunksCount(chunksCount)
	, usedChunksCount(0)
#ifdef _DEBUG
	, freeChunksCount(0)
	, allocChunksCount(0)
#endif
{

#ifdef _DEBUG
	printf_s("MemoryAllocator(chunk size: %i, chunks count: %i)\n", chunkSize, chunksCount);
#endif
}

MemoryAllocator::~MemoryAllocator() {
#ifdef _DEBUG
	cout << "~MemoryAllocator()" << endl;
#endif
}

FSA::FSA() :
	MemoryAllocator() {
#ifdef _DEBUG
	cout << "FMA()" << endl;
#endif
}

FSA::~FSA() {
#ifdef _DEBUG
	cout << "~FMA()" << endl;
#endif
	destroy();
	assert(!isInit);
}

void FSA::init() {
	//pPool = new void[static_cast<size_t>(chunksCount) * static_cast<size_t>(chunkSize)];
	isInit = true;
}

void FSA::destroy() {
	assert(isInit);
	::free(pPool);
	isInit = false;
}

void* FSA::alloc(size_t size) {
	assert(isInit);
	assert(size <= chunkSize);
	assert(usedChunksCount < chunksCount);

	if (size > chunkSize || usedChunksCount >= chunksCount) {
		return nullptr;
	}

	return nullptr;
}

void FSA::free(void* p) {
	assert(isInit);

}

#ifdef _DEBUG
void FSA::dumpStat() const {

}

void FSA::dumpBlocks() const {

}
#endif

FLA::FLA() :
	MemoryAllocator() {
#ifdef _DEBUG
	cout << "FLA()" << endl;
#endif
}

FLA::~FLA() {
#ifdef _DEBUG
	cout << "~FLA()" << endl;
#endif
	destroy();
	assert(!isInit);
}

void FLA::init() {
	isInit = true;
}

void FLA::destroy() {
	assert(isInit);
	isInit = false;
}

void* FLA::alloc(size_t size) {
	assert(isInit);
	return nullptr;
}

void FLA::free(void* p) {
	assert(isInit);

}

#ifdef _DEBUG
void FLA::dumpStat() const {

}

void FLA::dumpBlocks() const {

}
#endif