#include "pch.h"
#include "helper.hpp"
#include "dictionary.hpp"

using namespace std;
using namespace helper;

int main(int argc, char* argv[]) {
	Dictionary<int, int> dict;
	dict.put(1, 12);
	dict.put(2, 13);
	dict.put(3, 2);
	dict.put(4, 4);
	dict.put(5, -1);
	dict.put(6, 12);

	cout << dict[5];
	return 0;
}