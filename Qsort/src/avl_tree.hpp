#ifndef _AVL_TREE_HPP
#define _AVL_TREE_HPP

#include <vector>
#include <tuple>
using namespace std;

int max(int a, int b) {
	return (a > b) ? a : b;
}

template <typename K, typename V>
class AvlTree final {
private:
	struct Node;

public:
	AvlTree();
	~AvlTree();

	AvlTree(AvlTree const&) = delete;
	AvlTree& operator=(AvlTree const&) = delete;
	AvlTree(AvlTree&&) = delete;
	AvlTree& operator=(AvlTree&&) = delete;

	void preorder(vector<tuple<K, V>>* = nullptr);
	void insert(const K&, const V&);
	void remove(const K&);
	V* search(const K&);

private:
	void preorder(Node*, vector<tuple<K, V>>* = nullptr);
	Node* insert(const K&, const V&, Node*);
	Node* remove(const K&, Node*);
	Node* search(const K&, Node*);

	int getHeight(Node*) const;
	int getBalance(Node*) const;
	Node* getMin(Node*) const;

	Node* rotateLeft(Node*);
	Node* rotateRight(Node*);

private:
	Node* root;
};

template <typename K, typename V>
struct AvlTree<K, V>::Node final {
public:
	Node(const K& key, const V& value)
		: left(nullptr)
		, right(nullptr)
		, height(1)
		, key(key)
		, value(value)
	{}

	~Node() {
		if (left != nullptr) {
			delete left;
			left = nullptr;
		}

		if (right != nullptr) {
			delete right;
			right = nullptr;
		}
	}

	Node(Node const&) = delete;
	Node& operator=(Node const&) = delete;
	Node(Node&&) = delete;
	Node& operator=(Node&&) = delete;

public:
	Node* left;
	Node* right;
	int height;
	K key;
	V value;
};

template <typename K, typename V>
AvlTree<K, V>::AvlTree()
	: root(nullptr)
{}

template <typename K, typename V>
AvlTree<K, V>::~AvlTree() {
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
}

template <typename K, typename V>
void AvlTree<K, V>::preorder(vector<tuple<K, V>>* container) {
	preorder(root, container);
}

template <typename K, typename V>
void AvlTree<K, V>::preorder(Node* node, vector<tuple<K, V>>* container) {
	if (node != nullptr) {
		if (container != nullptr) {
			container->push_back(make_tuple(node->key, node->value));
		}

		preorder(node->left, container);
		preorder(node->right, container);
	}
}

template <typename K, typename V>
void AvlTree<K, V>::insert(const K& key, const V& value) {
	root = insert(key, value, root);
}

template <typename K, typename V>
typename AvlTree<K, V>::Node* AvlTree<K, V>::insert(const K& key, const V& value, Node* node) {
	if (node == nullptr) {								   // if tree is empty, create the node
		return new Node(key, value);
	}

	if (key < node->key) {								   // insert in the left subtree
		node->left = insert(key, value, node->left);
	} else if (key > node->key) {						   // insert in the right subtree
		node->right = insert(key, value, node->right);
	} else {											   // no duplicates allowed
		return node;
	}

	node->height = max(getHeight(node->left),			   // update height
					getHeight(node->right)) + 1;

	int balance = getBalance(node);						   // get balance factor of current node
														   // if node appears to be unbalanced there are 4 cases to perform:
	if (balance > 1 && key < node->left->key) {			   // case 1: left left case
		return rotateRight(node);
	}
	if (balance < -1 && key > node->right->key) {		   // case 2: right right case
		return rotateLeft(node);
	}
	if (balance > 1 && key > node->left->key) {			   // case 3: left right case
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}
	if (balance < -1 && key < node->right->key) {		   // case 4: right left case
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}
	return node;
}

template <typename K, typename V>
void AvlTree<K, V>::remove(const K& key) {
	root = remove(key, root);
}

template <typename K, typename V>
typename AvlTree<K, V>::Node* AvlTree<K, V>::remove(const K& key, Node* node) {
	if (node == nullptr) {									// at least one node needs to exist
		return node;
	}

	if (key < node->key) {								    // node to be deleted is in the left subtree
		node->left = remove(key, node->left);			   
	} else if (key > node->key) {						    // node to be deleted is in the right subtree
		node->right = remove(key, node->right);
	} else {												// node to be deleted was found
		if (node->left == nullptr ||						// node with only one child
			node->right == nullptr) {						// or no children at all

			Node* temp = node->left != nullptr ? node->left : node->right;

			if (temp == nullptr) {							// no child case
				temp = node;
				node = nullptr;
			} else {									    // one child case
				node->left		= temp->left;			   	// shallow copy
				node->right		= temp->right;				// shallow copy
				node->key		= temp->key;				// shallow copy
				node->value		= temp->value;				// shallow copy
				node->height	= temp->height;				// shallow copy
			}

			delete temp;									// free temp node memory
		} else {
			Node* temp = getMin(node->right);				// get the smallest node in the right subtree
			node->key	= temp->key;						// copy the inorder key
			node->value	= temp->value;						// copy the inorder value
			node->right = remove(temp->key, node->right);	// delete the inorder node
		}
	}

	if (node == nullptr) {									// return if tree had only one node
		return node;										
	}

	node->height = max(getHeight(node->left),				// update height of the current node
						getHeight(node->right)) + 1;

	int balance = getBalance(node);							// get balance factor of current node
															// if node appears to be unbalanced there are 4 cases to perform:
	if (balance > 1 && getBalance(node->left) >= 0) {		// case 1: left left case
		return rotateRight(node);
	}
	if (balance < -1 && getBalance(node->right) <= 0) {		// case 2: right right case
		return rotateLeft(node);
	}
	if (balance > 1 && getBalance(node->left) < 0) {		// case 3: left right case
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}
	if (balance < -1 && getBalance(node->right) > 0) {		// case 4: right left case
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}
	return node;
}

template <typename K, typename V>
V* AvlTree<K, V>::search(const K& key) {
	Node* node = search(key, root);
	if (node != nullptr) {
		return &node->value;
	} else {
		return nullptr;
	}
}

template <typename K, typename V>
typename AvlTree<K, V>::Node* AvlTree<K, V>::search(const K& key, Node* node) {
	if (node == nullptr) {
		return nullptr;
	}

	if (key < node->key) {
		return search(key, node->left);
	} else if (key > node->key) {
		return search(key, node->right);
	}

	return node;
}

template <typename K, typename V>
int AvlTree<K, V>::getHeight(Node* node) const {
	if (node == nullptr) {
		return 0;
	}

	return node->height;
}

template <typename K, typename V>
int AvlTree<K, V>::getBalance(Node* node) const {
	if (node == nullptr) {
		return 0;
	}

	return getHeight(node->left) - getHeight(node->right);
}

template <typename K, typename V>
typename AvlTree<K, V>::Node* AvlTree<K, V>::getMin(Node* node) const {
	Node* current = node;
	while (current->left != nullptr) {
		current = current->left;
	}
	return current;
}

template <typename K, typename V>
typename AvlTree<K, V>::Node* AvlTree<K, V>::rotateLeft(Node* node) {
	Node* newRoot = node->right;
	Node* temp = newRoot->left;

	// Perform rotation  
	newRoot->left = node;
	node->right = temp;

	// Update heights  
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	newRoot->height = max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

	// Return new root  
	return newRoot;
}

template <typename K, typename V>
typename AvlTree<K, V>::Node* AvlTree<K, V>::rotateRight(Node* node) {
	Node* newRoot = node->left;
	Node* temp = newRoot->right;

	// Perform rotation  
	newRoot->right = node;
	node->left = temp;

	// Update heights  
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	newRoot->height = max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

	// Return new root  
	return newRoot;
}

#endif // !_AVL_TREE_HPP