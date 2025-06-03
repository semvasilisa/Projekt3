#ifndef HASHTABLE_AVLTREE_H
#define HASHTABLE_AVLTREE_H

#include <iostream>
#include <vector>
#include <algorithm>

// --- AVL Tree Implementation ---
template<typename Key, typename Value>
class AVLTree {
private:
    struct Node {
        Key key;
        Value value;
        int height;
        Node* left;
        Node* right;

        Node(Key k, Value v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    int height(Node* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) {
        return height(node->left) - height(node->right);
    }

    void updateHeight(Node* node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* balance(Node* node) {
        updateHeight(node);
        int bf = balanceFactor(node);

        if (bf > 1) {
            if (balanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1) {
            if (balanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* insert(Node* node, Key key, Value value) {
        if (!node) return new Node(key, value);

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else
            node->value = value;

        return balance(node);
    }

    Node* minNode(Node* node) {
        return node->left ? minNode(node->left) : node;
    }

    Node* remove(Node* node, Key key) {
        if (!node) return nullptr;

        if (key < node->key)
            node->left = remove(node->left, key);
        else if (key > node->key)
            node->right = remove(node->right, key);
        else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            } else {
                Node* temp = minNode(node->right);
                node->key = temp->key;
                node->value = temp->value;
                node->right = remove(node->right, temp->key);
            }
        }

        return balance(node);
    }

    void inorder(Node* node) const {
        if (node) {
            inorder(node->left);
            std::cout << node->key << ":" << node->value << " ";
            inorder(node->right);
        }
    }

    void free(Node* node) {
        if (!node) return;
        free(node->left);
        free(node->right);
        delete node;
    }

public:
    ~AVLTree() {
        free(root);
    }

    void insert(Key key, Value value) {
        root = insert(root, key, value);
    }

    void remove(Key key) {
        root = remove(root, key);
    }

    void display() const {
        inorder(root);
        std::cout << std::endl;
    }
};


template<typename Key, typename Value>
class HashTableAVLTree {
private:
    std::vector<AVLTree<Key, Value>*> buckets;
    int capacity;

    int hash(Key key) const {
        return key % capacity;
    }

public:
    HashTableAVLTree(int cap) : capacity(cap) {
        if (capacity <= 0)
            throw std::invalid_argument("Capacity must be greater than 0");
        buckets.resize(capacity, nullptr);
    }

    ~HashTableAVLTree() {
        for (auto tree : buckets) {
            delete tree;
        }
    }

    void insert(Key key, Value value) {
        int index = hash(key);
        if (!buckets[index]) {
            buckets[index] = new AVLTree<Key, Value>();
        }
        buckets[index]->insert(key, value);
    }

    void remove(Key key) {
        int index = hash(key);
        if (buckets[index]) {
            buckets[index]->remove(key);
        }
    }

    void display() const {
        for (int i = 0; i < capacity; ++i) {
            std::cout << "Bucket " << i << ": ";
            if (buckets[i]) {
                buckets[i]->display();
            } else {
                std::cout << "Empty" << std::endl;
            }
        }
    }
};

#endif // HASHTABLE_AVLTREE_H
