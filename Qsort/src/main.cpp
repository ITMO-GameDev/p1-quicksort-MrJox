#include "pch.h"
#include "helper.hpp"
#include "dictionary.hpp"

using namespace std;
using namespace helper;

int main(int argc, char* argv[]) {
	Dictionary<int, int> dict;
	dict.put(6, 12);
	dict.put(2, 13);
	dict.put(4, 4);
	dict.put(1, 12);
	dict.put(5, -1);
	dict.put(3, 299);
	dict.put(12, 33);
	dict.put(31, -22);
	dict.put(33, 1);

	auto it = dict.iterator();
	for (; it.hasNext(); it.next()) {
		it.set(4);
		cout << "key: " << it.key() << "\tvalue: " << it.value() << endl;
	}

	dict.remove(6);
	cout << endl;
	for (; it.hasPrev(); it.prev()) {
		cout << "key: " << it.key() << "\tvalue: " << it.value() << endl;
	}

	return 0;
}