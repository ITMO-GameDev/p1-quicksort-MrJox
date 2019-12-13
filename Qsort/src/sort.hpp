#pragma once
#include "pch.h"
#include "helper.hpp"
using namespace std;

/// <summary>Namespace that contains sorting methods</summary>
namespace sort {
	using helper::swap;

	/// <summary>Optimal array size to sort using insertion sort over quick sort</summary>
	const int OPTIMAL_SIZE = 15;

	/// <summary>Validates whether the array is sorted according to comparison function</summary>
	/// <param name="arr">Array</param>
	/// <param name="first">Low index</param>
	/// <param name="last">High index</param>
	/// <param name="compare">Comparison function</param>
	/// <returns>Bollean value indicating whether the array is sorted or not</returns>
	template <typename T, typename Compare>
	bool is_sorted(vector<T> &arr, int first, int last, Compare compare) {
		for (int i = first + 1; i <= last; ++i) {
			if (compare(arr[i], arr[i - 1]))
				return false;
		}
		return true;
	}

	/// <summary>Validates whether the array is sorted according to comparison function</summary>
	/// <param name="arr">Array</param>
	/// <param name="compare">Comparison function</param>
	/// <returns>Bollean value indicating whether the array is sorted or not</returns>
	template <typename T, typename Compare>
	bool is_sorted(vector<T> &arr, Compare compare) {
		return is_sorted(arr, 0, arr.size() - 1, compare);
	}

	/// <summary>Insertion sort</summary>
	/// <param name="arr">Array</param>
	/// <param name="compare">Comparison function</param>
	template <typename T, typename Compare>
	void insertion_sort(vector<T> &arr, Compare compare) {
		int i, key, j;
		size_t n = arr.size();

		for (i = 1; i < n; ++i) {
			key = arr[i];
			j = i - 1;

			while (j >= 0 && compare(key, arr[j])) {
				arr[j + 1] = arr[j];
				j = j - 1;
			}
			arr[j + 1] = key;
		}
	}

	/// <summary>Calculates a median of the first, middle and the last elements</summary>
	/// <param name="arr">Array</param>
	/// <param name="first">Low index</param>
	/// <param name="last">High index</param>
	/// <param name="compare">Comparison function</param>
	/// <returns>Median index</returns>
	template <typename T, typename Compare>
	int get_median(vector<T> &arr, int first, int last, Compare compare) {
		int middle = first + (last - first) / 2;
		
		if (compare(arr[middle], arr[first]))
			swap(arr[middle], arr[first]);
		if (compare(arr[last], arr[first]))
			swap(arr[last], arr[first]);
		if (compare(arr[last], arr[middle]))
			swap(arr[last], arr[middle]);

		swap(arr[middle], arr[first]);
		return first;
	}

	/// <summary>Hoare partition</summary>
	/// <param name="arr">Array</param>
	/// <param name="first">Low index</param>
	/// <param name="last">High index</param>
	/// <param name="compare">Comparison function</param>
	/// <returns>Returns a pivot index</returns>
	template <typename T, typename Compare>
	int partition(vector<T> &arr, int first, int last, Compare compare) {
		int left = first;
		int right = last;
		T pivot = arr[get_median(arr, first, last, compare)];

		while (left < right) {
			do ++left;  while (compare(arr[left], pivot));
			do --right; while (compare(pivot, arr[right]));
			if (left < right) swap(arr[left], arr[right]);
		}

		arr[first] = arr[right];
		arr[right] = pivot;
		return right;
	}

	/// <summary>Quick sort based on median pivot and Hoare partition</summary>
	/// <param name="arr">Array</param>
	/// <param name="first">Low index</param>
	/// <param name="last">High index</param>
	/// <param name="compare">Comparison function</param>
	template <typename T, typename Compare>
	void qsort(vector<T> &arr, int first, int last, Compare compare) {
		if (first >= last) return;
		else if (arr.size() <= OPTIMAL_SIZE) return insertion_sort(arr, compare);

		while (first <= last) {
			int pivot = partition(arr, first, last, compare);

			if (pivot - first < last - pivot) {
				qsort(arr, first, pivot - 1, compare);
				first = pivot + 1;
			} else {
				qsort(arr, pivot + 1, last, compare);
				last = pivot - 1;
			}
		}

		assert(is_sorted(arr, first, last, compare));
	}

	/// <summary>Quick sort based on median pivot and Hoare partition</summary>
	/// <param name="arr">Array</param>
	/// <param name="compare">Comparison function</param>
	template <typename T, typename Compare>
	void qsort(vector<T> &arr, Compare compare) {
		size_t size = arr.size();
		if (size <= 1) return;
		qsort(arr, 0, size - 1, compare);
	}
}