#pragma once
#include "pch.h"
using namespace std;

namespace containers {
	template <typename T>
	class Array final {
	protected:
		class Iterator;

	public:
		Array() : Array(0) {}
		Array(int capacity);
		~Array();

		Array(const Array& other);
		Array(Array&& other);
		Array& operator=(const Array& other);
		Array& operator=(Array&& other);

		inline bool isEmpty() { return _size == 0; }
		void insert(const T& value);
		void insertAt(int index, const T& value);
		void remove();
		void removeAt(int index);
		int size() const;

		const T& operator[](int index) const;
		T& operator[](int index);

		Iterator& iterator();
		const Iterator& iterator() const;

	protected:
		inline void clear();

		Iterator it;
		T*		_head;
		int		_capacity;
		int		_size;
	};

	template <typename T>
	class Array<T>::Iterator {
	public:
		Iterator(Array<T>& array) : _array(array), currentIndex(0) {}
		~Iterator() {}

		inline const T& get() const;
		void set(const T& value);
		void insert(const T& value);
		void remove();
		void next();
		void prev();
		void toIndex(int index);
		bool hasNext() const;
		bool hasPrev() const;

	private:
		Array<T>& _array;
		int currentIndex;
	};

	template <typename T>
	Array<T>::Array(int capacity)
		:	_capacity(capacity),
			_size(capacity),
			_head(nullptr),
			it(*this) {

		if (capacity > 0) {
			_head = new T[capacity];
			for (size_t i = 0; i < capacity; ++i) {
				_head[i] = 0;
			}
		}
	}

	template <typename T>
	Array<T>::~Array() {
		clear();
	}

	template <typename T>
	Array<T>::Array(const Array& other)
		:	_capacity(other._capacity),
			_size(other._size),
			_head(nullptr) {

		if (this->_capacity > 0) {
			this->_head = new T[this->_capacity];
			memcpy(this->_head, other._head, other._size * sizeof T);
		}
	}

	template <typename T>
	Array<T>::Array(Array&& other)
		:	_capacity(other._capacity),
			_size(other._size),
			_head(other._head) {

		other._capacity = 0;
		other._size = 0;
		other._head = nullptr;
	}

	template <typename T>
	Array<T>& Array<T>::operator=(const Array& other) {
		if (this != &other) {
			clear();
			this->_capacity = other._capacity;
			this->_size = other._size;

			if (this->_capacity > 0) {
				this->_head = new T[this->_capacity];
				memcpy(this->_head, other._head, other._size * sizeof T);
			}
		}
		return *this;
	}

	template <typename T>
	Array<T>& Array<T>::operator=(Array&& other) {
		if (this != &other) {
			clear();

			this->_capacity = other._capacity;
			this->_size = other._size;

			other._capacity = 0;
			other._size = 0;

			if (other._head != nullptr) {
				this->_head = other._head;
				other._head = nullptr;
			}
		}
		return *this;
	}
	
	template <typename T>
	void Array<T>::insert(const T& value) {
		if (_size < _capacity) {
			_head[_size] = value;
		} else {
			T* temp = std::move(_head);
			_capacity = (_size >= 10 ? 2 * _size : _size) + 1;
			_head = new T[_capacity];
			memcpy(_head, temp, _size * sizeof T);
			_head[_size] = value;
			delete[] temp;
		}
		++_size;
	}
	
	template <typename T>
	void Array<T>::insertAt(int index, const T& value) {
		if (index < 0 || index > _capacity) {
			throw out_of_range("You are trying to insert into an invalid index. Index " + to_string(index) + " is out of range");
		}
		if (_size < _capacity) {
			T oldValue = _head[index];
			_head[index] = value;
			memcpy(_head + index + 2, _head + index + 1, (_size - index - 1) * sizeof T);
			_head[index + 1] = oldValue;
		} else {
			T* temp = std::move(_head);
			_capacity = (_size > 10 ? 2 * _size : _size) + 1;
			_head = new T[_capacity];
			memcpy(_head, temp, index * sizeof T);
			_head[index] = value;
			memcpy(_head + index + 1, temp + index, (_size - index) * sizeof T);
			delete[] temp;
		}
		++_size;
	}
	
	template <typename T>
	void Array<T>::remove() {
		removeAt(0);
	}

	template <typename T>
	void Array<T>::removeAt(int index) {
		if (index < 0 || index > _capacity) {
			throw out_of_range("You are trying to remove an item at an invalid index. Index " + to_string(index) + " is out of range");
		}
		if (_size - 1 == 0) {
			clear();
		} else if (_size <= 0) {
			throw out_of_range("You are trying to remove an item in an empty array");
		} else if (_size < _capacity) {
			memcpy(_head + index, _head + index + 1, (_size - index - 1) * sizeof T);
		} else {
			T* temp = std::move(_head);
			_head = new T[_size - 1];
			memcpy(_head, temp, index * sizeof T);
			memcpy(_head + index, temp + index + 1, (_size - index - 1) * sizeof T);
			delete[] temp;
		}
		--_size;
	}

	template <typename T>
	int Array<T>::size() const {
		return _size;
	}

	template <typename T>
	const T& Array<T>::operator[](int index) const {
		if (index < 0 || index > _size || _size == 0) {
			throw out_of_range("You are trying to access an item at an invalid index. Index " + to_string(index) + " is out of range");
		}
		return _head[index];
	}
	
	template <typename T>
	T& Array<T>::operator[](int index) {
		if (index < 0 || index > _size || _size == 0) {
			throw out_of_range("You are trying to access an item at an invalid index. Index " + to_string(index) + " is out of range");
		}
		return _head[index];
	}

	template<typename T>
	typename Array<T>::Iterator& Array<T>::iterator() {
		return it;
	}

	template<typename T>
	typename const Array<T>::Iterator& Array<T>::iterator() const {
		return it;
	}

	template <typename T>
	inline void Array<T>::clear() {
		if (_capacity > 0 && _head != nullptr) {
			delete[] _head;
			_head = nullptr;
		}
	}

	template <typename T>
	inline const T& Array<T>::Iterator::get() const {
		return _array[currentIndex];
	}

	template <typename T>
	inline void Array<T>::Iterator::next() {
		++currentIndex;
	}

	template <typename T>
	inline void Array<T>::Iterator::prev() {
		--currentIndex;
	}

	template <typename T>
	inline void Array<T>::Iterator::toIndex(int index) {
		currentIndex = index;
	}

	template <typename T>
	inline bool Array<T>::Iterator::hasNext() const {
		return currentIndex < (_array._size - 1);
	}

	template <typename T>
	inline bool Array<T>::Iterator::hasPrev() const {
		return currentIndex > 0;
	}

	template <typename T>
	void Array<T>::Iterator::set(const T& value) {
		_array[currentIndex] = value;
	}

	template <typename T>
	void Array<T>::Iterator::insert(const T& value) {
		_array.insertAt(currentIndex, value);
	}

	template <typename T>
	void Array<T>::Iterator::remove() {
		_array.removeAt(currentIndex);
	}
}