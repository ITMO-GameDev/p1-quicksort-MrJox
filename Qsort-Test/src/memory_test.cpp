#include "pch.h"
#include "../../Qsort/src/memory.hpp"

TEST(MemoryTest, ZeroAllocTest) {
	bool isAlloc = false;
	MemoryAllocator alloc;
	alloc.init();
	void* p = alloc.alloc(0);
	isAlloc = p != nullptr;
	alloc.free(p);
	EXPECT_TRUE(!isAlloc);
}

TEST(MemoryTest, Pool16AllocTest) {
	bool isAlloc = false;
	MemoryAllocator alloc;
	alloc.init();
	auto p = alloc.alloc(16);
	isAlloc = p != nullptr;
	alloc.free(p);
	EXPECT_TRUE(isAlloc);
}

TEST(MemoryTest, Pool512AllocTest) {
	bool isAlloc = false;
	MemoryAllocator alloc;
	alloc.init();
	auto p = alloc.alloc(512);
	isAlloc = p != nullptr;
	alloc.free(p);
	EXPECT_TRUE(isAlloc);
}

TEST(MemoryTest, FreeListAllocTest) {
	bool isAlloc = false;
	MemoryAllocator alloc;
	alloc.init();
	auto p = alloc.alloc(2 * 1024 * 1024);
	isAlloc = p != nullptr;
	alloc.free(p);
	EXPECT_TRUE(isAlloc);
}

TEST(MemoryTest, OSAllocTest) {
	bool isAlloc = false;
	MemoryAllocator alloc;
	alloc.init();
	auto p = alloc.alloc(11 * 1024 * 1024);
	isAlloc = p != nullptr;
	alloc.free(p);
	EXPECT_TRUE(isAlloc);
}