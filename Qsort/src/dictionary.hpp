#pragma once
#include "avl_tree.hpp"

template <typename K, typename V>
class Dictionary final {
private:
	class Iterator;

public:
	Dictionary();
	~Dictionary() = default;

	Dictionary(Dictionary const&) = delete;
	Dictionary& operator=(Dictionary const&) = delete;
	Dictionary(Dictionary&&) = delete;
	Dictionary& operator=(Dictionary&&) = delete;

	int size() const { return _size; }
	void put(const K& key, const V& value);
	void remove(const K& key);
	bool contains(const K& key);

	const V& operator[](const K& key) const;
	V& operator[](const K& key);

	Iterator iterator();
	const Iterator iterator() const;

private:
	AvlTree<K, V>	_tree;
	int				_size;
};

template <typename K, typename V>
class Dictionary<K, V>::Iterator {
public:
	Iterator(AvlTree<K, V>& tree)
		: _tree(tree)
		, currentIndex(0)
	{
		_tree.preorder(&_container);
	}

	const K& key() const;
	const V& value() const;
	void set(const V& value);
	void next();
	void prev();
	bool hasNext() const;
	bool hasPrev() const;

private:
	AvlTree<K, V>& _tree;
	vector<tuple<K, V>> _container;
	int currentIndex;
};

template <typename K, typename V>
Dictionary<K, V>::Dictionary() :
	_size(0)
{}

template <typename K, typename V>
void Dictionary<K, V>::put(const K& key, const V& value) {
	V* pValue = _tree.search(key);

	if (pValue == nullptr) {
		_tree.insert(key, value);
		++_size;
	} else {
		*pValue = value;
	}
}

template <typename K, typename V>
void Dictionary<K, V>::remove(const K& key) {
	if (contains(key)) {
		_tree.remove(key);
		--_size;
	}
}

template <typename K, typename V>
bool Dictionary<K, V>::contains(const K& key) {
	return _tree.search(key) != nullptr;
}

template <typename K, typename V>
const V& Dictionary<K, V>::operator[](const K& key) const {
	V* pValue = _tree.search(key);

	if (pValue != nullptr) {
		return *pValue;
	}

	return V();
}

template <typename K, typename V>
V& Dictionary<K, V>::operator[](const K& key) {
	V* pValue = _tree.search(key);
	
	if (pValue != nullptr) {
		return *pValue;
	}

	V& value = *new V();
	_tree.insert(key, value);
	return value;
}

template <typename K, typename V>
typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator() {
	return Iterator(_tree);
}

template <typename K, typename V>
typename const Dictionary<K, V>::Iterator Dictionary<K, V>::iterator() const {
	return Iterator(_tree);
}

template <typename K, typename V>
const K& Dictionary<K, V>::Iterator::key() const {
	return get<0>(_container.at(currentIndex));
}

template <typename K, typename V>
const V& Dictionary<K, V>::Iterator::value() const {
	return get<1>(_container.at(currentIndex));
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::set(const V& value) {
	V* pValue = _tree.search(key());
	if (pValue != nullptr) {
		*pValue = value;
	}
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::next() {
	++currentIndex;
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::prev() {
	--currentIndex;
}

template <typename K, typename V>
bool Dictionary<K, V>::Iterator::hasNext() const {
	return currentIndex < (_container.size() - 1);
}

template <typename K, typename V>
bool Dictionary<K, V>::Iterator::hasPrev() const {
	return (currentIndex + 1) > 0;
}