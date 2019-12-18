#pragma once

template <typename K, typename V>
struct Node {
public:
	Node(const K& _key, const V& _value) :
		key(_key), value(_value), left(nullptr), right(nullptr), parent(nullptr), isBlack(true)
	{}

	K		key;
	V		value;
	Node*	left;
	Node*	right;
	Node*	parent;
	bool	isBlack;
};

template <typename K, typename V>
class RBTree final {
public:
	RBTree() {
		check	= new Node<K, V>(K(), V());
		root	= check;
	}
	~RBTree() = default;

	RBTree(RBTree const&) = delete;
	RBTree& operator=(RBTree const&) = delete;
	RBTree(RBTree&&) = delete;
	RBTree& operator=(RBTree&&) = delete;

	void insert(const K& key, const V& value);
	void remove(const K& key);
	void leftRotate(Node<K, V>*& node);
	void rightRotate(Node<K, V>*& node);
	Node<K, V>* search(const K& key);

private:
	void insertFixup(Node<K, V>*& node);
	void removeFixup(Node<K, V>*& node);
	Node<K, V>* search(const K& key, Node<K, V>*& node);
	Node<K, V>* min(Node<K, V>*& node);

	Node<K, V> *root;
	Node<K, V> *check;
};

template <typename K, typename V>
void RBTree<K, V>::insertFixup(Node<K, V>*& node) {
	while (!node->parent->isBlack) {
		if (node->parent == node->parent->parent->left) {

			Node<K, V>* right = node->parent->parent->right;
			if (!right->isBlack) {
				node->parent->isBlack = true;
				right->isBlack = true;
				node->parent->parent->isBlack = false;
				node = node->parent->parent;

			} else {

				if (node == node->parent->right) {
					node = node->parent;
					leftRotate(node);
				}

				node->parent->isBlack = true;
				node->parent->parent->isBlack = false;
				rightRotate(node);
			}

		} else {

			Node<K, V>* left = node->parent->parent->left;
			if (!left->isBlack) {
				node->parent->isBlack = true;
				left->isBlack = true;
				node->parent->parent->isBlack = false;
				node = node->parent->parent;

			} else {

				if (node == node->parent->left) {
					node = node->parent;
					rightRotate(node);
				}

				node->parent->isBlack = true;
				node->parent->parent->isBlack = false;
				leftRotate(node);
			}
		}
	}

	root->isBlack = true;
}

template <typename K, typename V>
void RBTree<K, V>::removeFixup(Node<K, V>*& node) {
	while (node != root && node->isBlack) {
		if (node == node->parent->left) {
			Node<K, V>* right = node->parent->right;

			if (!right->isBlack) {
				right->isBlack = true;
				node->parent->isBlack = false;
				leftRotate(node->parent);
				right = node->parent->right;
			}

			if (right->left->isBlack && right->right->isBlack) {
				right->isBlack = false;
				node = node->parent;

			} else {
				if (right->right->isBlack) {
					right->left->isBlack = true;
					right->isBlack = false;
					rightRotate(right);
					right = node->parent->right;
				}

				right->isBlack = node->parent->isBlack;
				node->parent->isBlack = true;

				if (right->right) {
					right->right->isBlack = true;
				}

				leftRotate(node->parent);
				node = root;
			}

		} else {
			Node<K, V>* left = node->parent->left;
			if (!left->isBlack) {
				left->isBlack = true;
				node->parent->isBlack = false;
				rightRotate(node->parent);
				left = node->parent->left;
			}

			if (left->right->isBlack && left->left->isBlack) {
				left->isBlack = false;
				node = node->parent;

			} else {
				if (left->left->isBlack) {
					left->right->isBlack = true;
					left->isBlack = false;
					leftRotate(left);
					left = node->parent->left;
				}

				left->isBlack = node->parent->isBlack;
				node->parent->isBlack = true;
				left->left->isBlack = true;
				rightRotate(node->parent);
				node = root;
			}
		}
	}

	node->isBlack = true;
}

template <typename K, typename V>
Node<K, V>* RBTree<K, V>::search(const K& key, Node<K, V>*& node) {
	if (node == check) {
		return nullptr;
	} else if (key == node->key) {
		return node;
	} else if (key < node->key) {
		return search(key, node->left);
	} else {
		return search(key, node->right);
	}
}

template <typename K, typename V>
Node<K, V>* RBTree<K, V>::min(Node<K, V>*& node) {
	while (node->left != check) {
		node = node->left;
	}
	return node;
}

template <typename K, typename V>
void RBTree<K, V>::insert(const K& key, const V& value) {
	Node<K, V>* newNode = new Node<K, V>(key, value);
	Node<K, V>* y = check;
	Node<K, V>* x = root;

	while (x != check) {
		y = x;
		if (newNode->key < x->key) {
			x = x->left;
		} else {
			x = x->right;
		}
	}

	newNode->parent = y;
	if (root == check) {
		root = newNode;
	} else if (newNode->key < y->key) {
		y->left = newNode;
	} else {
		y->right = newNode;
	}

	newNode->right		= check;
	newNode->left		= check;
	newNode->isBlack	= false;
	insertFixup(newNode);
}

template <typename K, typename V>
void RBTree<K, V>::remove(const K& key) {
	Node<K, V>* x;
	Node<K, V>* y;
	Node<K, V>* z;
	search(data, z);

	y = z;
	if (!y) {
		return;
	}

	bool yOriginalIsBlack = y->isBlack;
	if (z->left == check) {
		x = z->right;
		transplant(z, z->right);

	} else if (z->right == check) {
		x = z->left;
		transplant(z, z->left);

	} else {
		Node<K, V>* right = min(z->right);
		yOriginalIsBlack = right->isBlack;
		x = right->right;

		if (right->parent == z) {
			x->parent = right;
		} else {
			transplant(right, right->right);
			right->right = z->right;
			right->right->parent = right;
		}

		transplant(z, right);
		right->left = z->left;
		right->left->parent = right;
		right->isBlack = z->isBlack;
	}

	if (yOriginalIsBlack == true) {
		removeFixup(x);
	}
}

template <typename K, typename V>
void RBTree<K, V>::leftRotate(Node<K, V>*& node) {
	Node<K, V>* newNode = node->right;
	node->right = newNode->left;

	if (newNode->left) {
		newNode->left->parent = node;
		newNode->parent = node->parent;
	}

	if (node->parent == check) {
		root = newNode;
	} else if (node->parent->left == node) {
		node->parent->left = newNode;
	} else {
		node->parent->right = newNode;
	}

	newNode->left = node;
	node->parent = newNode;
}

template <typename K, typename V>
void RBTree<K, V>::rightRotate(Node<K, V>*& node) {
	Node<K, V>* newNode = node->left;
	node->left = newNode->right;

	if (newNode->right) {
		newNode->right->parent = node;
		newNode->parent = node->parent;
	}
	
	if (node->parent == check) {
		root = newNode;
	} else if (node->parent->left == node) {
		node->parent->left = newNode;
	} else {
		node->parent->right = newNode;
	}

	newNode->right = node;
	node->parent = newNode;
}

template <typename K, typename V>
Node<K, V>* RBTree<K, V>::search(const K& key) {
	return search(key, root);
}