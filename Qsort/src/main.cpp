#include "pch.h"
#include "helper.hpp"
#include "memory.hpp"

using namespace std;
using namespace helper;

int main(int argc, char* argv[]) {
	FSA allocator;
	allocator.init();

	int* pi = reinterpret_cast<int*>(allocator.alloc(sizeof(int)));
	double* pd = reinterpret_cast<double*>(allocator.alloc(sizeof(double)));
	//int* pa = reinterpret_cast<int*>(allocator.alloc(10 * sizeof(int)));

#ifdef _DEBUG
	allocator.dumpStat();
	allocator.dumpBlocks();
#endif
	allocator.free(reinterpret_cast<void*>(pi));
	allocator.free(reinterpret_cast<void*>(pd));
	//allocator.free(reinterpret_cast<void*>(pa));
	allocator.destroy();

	return 0;
}