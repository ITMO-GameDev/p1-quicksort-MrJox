#include "pch.h"
#include "../../Qsort/src/dictionary.hpp"

TEST(DictionaryTest, SizeTest) {
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

	EXPECT_TRUE(dict.size() == 9);
}

TEST(DictionaryTest, RemoveTest) {
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
	dict.remove(5);

	EXPECT_TRUE(!dict.contains(5));
}

TEST(DictionaryTest, ContainsTest) {
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

	EXPECT_TRUE(dict.contains(4));
}

TEST(DictionaryTest, PutTest) {
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

	EXPECT_TRUE(dict.contains(33));
	EXPECT_TRUE(dict[33] == 1);
}

TEST(DictionaryTest, IndexingTest) {
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

	EXPECT_TRUE(dict[5] == -1);
}

TEST(DictionaryTest, ChangeValueTest) {
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

	dict[4] = -149;

	EXPECT_TRUE(dict[4] == -149);
}

TEST(IteratorTest, SetTest) {
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

	for (auto it = dict.iterator(); it.hasNext(); it.next()) {
		it.set(0);
	}

	EXPECT_TRUE(dict[12] == 0);
}