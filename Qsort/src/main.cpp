#include "pch.h"
#include "helper.hpp"
#include "sort.hpp"
#include <random>
#include <chrono>

using namespace std;
using namespace helper;
using namespace sort;

int main(int argc, char* argv[]) {
	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<int> dist(-50, 100);

	vector<int> arr_small;
	vector<int> arr_optimal;
	vector<int> arr_average;
	vector<int> arr_large;

	for (size_t i = 0; i < 200; ++i) {
		if (i < 10) arr_small.push_back(dist(rng));
		if (i < 15) arr_optimal.push_back(dist(rng));
		if (i < 30) arr_average.push_back(dist(rng));
		arr_large.push_back(dist(rng));
	}

	cout << "small array: ";
	print_array(begin(arr_small), end(arr_small));
	cout << endl << "average array: ";
	print_array(begin(arr_average), end(arr_average));
	cout << endl << "large array: ";
	print_array(begin(arr_large), end(arr_large));
	cout << endl << "optimal array: ";
	print_array(begin(arr_optimal), end(arr_optimal));
	cout << endl;
	auto sorting_order = [](int a, int b) { return a < b; };

	auto start = chrono::high_resolution_clock::now();
	insertion_sort(arr_small, sorting_order);
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << "insertion_sort() for 10 elements time elapsed: " << duration << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	qsort(arr_small, sorting_order);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << "quick_sort() for 10 elements time elapsed: " << duration << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	insertion_sort(arr_average, sorting_order);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << "insertion_sort() for 30 elements time elapsed: " << duration << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	qsort(arr_average, sorting_order);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << "quick_sort() for 30 elements time elapsed: " << duration << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	insertion_sort(arr_large, sorting_order);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << "insertion_sort() for 200 elements time elapsed: " << duration << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	qsort(arr_large, sorting_order);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << "quick_sort() for 200 elements time elapsed: " << duration << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	insertion_sort(arr_optimal, sorting_order);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << "insertion_sort() for 15 elements time elapsed: " << duration << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	qsort(arr_optimal, sorting_order);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
	cout << "quick_sort() for 15 elements time elapsed: " << duration << " ms" << endl << endl;

	cout << "sorted small array: ";
	helper::print_array(std::begin(arr_small), std::end(arr_small));
	cout << "sorted average array: ";
	helper::print_array(std::begin(arr_average), std::end(arr_average));
	cout << "sorted large array: ";
	helper::print_array(std::begin(arr_large), std::end(arr_large));
	cout << "sorted optimal array: ";
	helper::print_array(std::begin(arr_optimal), std::end(arr_optimal));

	return 0;
}