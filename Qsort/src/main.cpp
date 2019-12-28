#include "pch.h"
#include "helper.hpp"
#include "memory.hpp"

using namespace std;
using namespace helper;

int main(int argc, char* argv[]) {
	MemoryAllocator alloc;
	alloc.init();
	void* p1 = alloc.alloc(256);
	void* p2 = alloc.alloc(2000000);
	alloc.dumpStat();
	alloc.free(p1);
	alloc.free(p2);
	alloc.dumpStat();

	return 0;
}