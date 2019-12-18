#pragma once
#include "rbtree.hpp"

template <typename K, typename V>
class Dictionary final {
private:
	class Iterator;

public:
	Dictionary();
	virtual ~Dictionary();

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

private:
	RBTree<K, V>	_tree;
	int				_size;
};

template <typename K, typename V>
Dictionary<K, V>::Dictionary() :
	_size(0) {

}

template <typename K, typename V>
Dictionary<K, V>::~Dictionary() {

}

template <typename K, typename V>
void Dictionary<K, V>::put(const K& key, const V& value) {
	Node<K, V>* pNode = _tree.search(key);

	if (pNode == nullptr) {
		_tree.insert(key, value);
		++_size;
	} else {
		pNode->value = value;
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
	Node<K, V>* pNode = _tree.search(key);

	if (pNode != nullptr) {
		return pNode->value;
	}

	return *new V();
}

template <typename K, typename V>
V& Dictionary<K, V>::operator[](const K& key) {
	Node<K, V>* pNode = _tree.search(key);
	
	if (pNode != nullptr) {
		return pNode->value;
	}

	V& value = *new V();
	_tree.insert(key, value);
	return value;
}

// TODO: ITERATOR
