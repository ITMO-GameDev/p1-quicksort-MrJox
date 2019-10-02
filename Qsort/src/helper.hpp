#pragma once
#include "pch.h"
using namespace std;

/// <summary>Helper methods to perform common operations</summary>
namespace helper {
	/// <summary>Swaps left and right arguments</summary>
	/// <param name="left">First param</param>
	/// <param name="right">Second param</param>
	template <typename T>
	void swap(T &left, T &right) {
		if (left == right) return;
		T temp = left;
		left = right;
		right = temp;
	}

	/// <summary>Prints an array</summary>
	/// <param name="begin">Begin iterator</param>
	/// <param name="end">End iterator</param>
	template <class InputIterator>
	void print_array(InputIterator begin, InputIterator end) {
		for (auto itr = begin; itr != end; ++itr)
			cout << *itr << " ";
		cout << endl;
	}

	/// <summary>Calculates array size</summary>
	/// <returns>Array size</returns>
	template <class T, size_t n>
	constexpr size_t array_size(T(&)[n]) {
		return n;
	}
}