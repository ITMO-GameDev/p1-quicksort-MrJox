#include "pch.h"
#include "../../Qsort/src/sort.hpp"
using sort::qsort;
using sort::is_sorted;

auto compare_asc_expr = [](int a, int b) { return a < b; };
auto compare_desc_expr = [](int a, int b) { return a > b; };

TEST(QSortTest, IntegerAscPositiveTest) {
	vector<int> arr{ 9, 2, 3, 0, 1, 4 };
	qsort(arr, compare_asc_expr);
	EXPECT_TRUE(is_sorted(arr, compare_asc_expr));
}

TEST(QSortTest, IntegerDescPositiveTest) {
	vector<int> arr{ 9, 2, 3, 0, 1, 4 };
	qsort(arr, compare_desc_expr);
	EXPECT_TRUE(is_sorted(arr, compare_desc_expr));
}

TEST(QSortTest, IntegerNegativeTest) {
	vector<int> arr{ -9, -2, -3, -10, -1, -4 };
	qsort(arr, compare_asc_expr);
	EXPECT_TRUE(is_sorted(arr, compare_asc_expr));
}

TEST(QSortTest, IntegerMixedTest) {
	vector<int> arr{ -9, -2, -3, 0, -1, -4, 4, 82, 12, 1, 22, 93, 21, 23, 14, 16, -12, -4, -55, -56 };
	qsort(arr, compare_asc_expr);
	EXPECT_TRUE(is_sorted(arr, compare_asc_expr));
}

TEST(QSortTest, DoubleMixedTest) {
	vector<double> arr{ -9.23, -2.123, -3.423, 0, -1.11, -4.23, 4.1, 82.42, 12.1, 1.9, 22.21, 93.1, 21, 23.01, 14, 16, -12, -4.4, -55.55, -56 };
	qsort(arr, compare_asc_expr);
	EXPECT_TRUE(is_sorted(arr, compare_asc_expr));
}