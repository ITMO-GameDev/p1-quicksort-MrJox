#include "pch.h"
#include "../../Qsort/src/array.hpp"

using namespace std;
using namespace containers;

TEST(ArrayTest, ArrayRemoveAllTest) {
	int size = 10;
	Array<int> arr;
	for (int i = 0; i < size; ++i) {
		arr.insert(i + 1);
	}

	for (int i = 0; i < size; ++i) {
		arr.remove();
	}

	EXPECT_TRUE(arr.isEmpty());
}

TEST(ArrayTest, ArrayRemoveAtTest) {
	Array<int> arr(10);

	for (int i = 0; i < 10; ++i) {
		arr[i] = i + 1;
	}

	arr.removeAt(2);
	EXPECT_TRUE(arr[2] == 4);
}

TEST(ArrayTest, RemoveOneItemTest) {
	Array<int> arr(1);
	arr.remove();
	EXPECT_TRUE(arr.isEmpty());
}

TEST(ArrayTest, OutOfRangeExceptionTest) {
	bool isExcept = false;
	Array<int> arr(1);
	arr.remove();

	try {
		int first = arr[0];
	} catch (out_of_range ex) {
		isExcept = true;
	}

	EXPECT_TRUE(isExcept);
}

TEST(ArrayTest, IteratorDoesntHaveNextTest) {
	Array<int> arr(5);
	auto& it = arr.iterator();
	it.toIndex(arr.size() - 1);
	EXPECT_TRUE(!it.hasNext());
}

TEST(ArrayTest, IteratorHasPrevTest) {
	Array<int> arr(5);
	auto& it = arr.iterator();
	it.toIndex(arr.size() - 1);
	EXPECT_TRUE(it.hasPrev());
}

TEST(ArrayTest, IteratorHasNextTest) {
	Array<int> arr(5);
	auto& it = arr.iterator();
	it.toIndex(0);
	EXPECT_TRUE(it.hasNext());
}

TEST(ArrayTest, IteratorDoesntHavePrevTest) {
	Array<int> arr(5);
	auto& it = arr.iterator();
	it.toIndex(0);
	EXPECT_TRUE(!it.hasPrev());
}

TEST(ArrayTest, IteratorSetTest) {
	Array<int> arr(5);
	auto& it = arr.iterator();

	it.toIndex(3);
	it.set(33);

	EXPECT_TRUE(it.get() == 33);
}