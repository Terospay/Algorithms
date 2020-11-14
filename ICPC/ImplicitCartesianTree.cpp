#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

template<typename T>
struct Node {
    explicit Node(T data) :
            data(data), left(nullptr), right(nullptr), prior(std::rand()), c(1) {};
    T data;
    Node *left;
    Node *right;
    int prior, c;
};

template<typename T>
class ImplicitTree {
public:
    ImplicitTree() : _root(nullptr) { std::srand(std::time(nullptr)); }

    void Insert(T data, int pos);

    T Find(int pos);

    void Erase(int pos);

private:
    explicit ImplicitTree(Node<T> *node) : _root(node) {};

    std::pair<Node<T> *, Node<T> *> split(Node<T> *node, int key);

    Node<T> *merge(Node<T> *left, Node<T> *right);

    void update(Node<T> *node);

    Node<T> *_root;
};

template<typename T>
void ImplicitTree<T>::update(Node<T> *node) {
    node->c = 1;
    if (node->left != nullptr) {
        node->c += node->left->c;
    }
    if (node->right != nullptr) {
        node->c += node->right->c;
    }
}

template<typename T>
std::pair<Node<T> *, Node<T> *> ImplicitTree<T>::split(Node<T> *node, int key) {
    if (node == nullptr) {
        return std::make_pair(nullptr, nullptr);
    }
    int curr_key = 0;
    if (node->left != nullptr) {
        curr_key += node->left->c;
    }
    if (curr_key < key) {
        std::pair<Node<T> *, Node<T> *> pair = split(node->right, key - curr_key - 1);
        node->right = pair.first;
        update(node);
        return std::make_pair(node, pair.second);
    } else {
        std::pair<Node<T> *, Node<T> *> pair = split(node->left, key);
        node->left = pair.second;
        update(node);
        return std::make_pair(pair.first, node);
    }

}

template<typename T>
Node<T> *ImplicitTree<T>::merge(Node<T> *left, Node<T> *right) {
    if (left == nullptr || right == nullptr) {
        return left == nullptr ? right : left;
    }
    if (left->prior > right->prior) {
        left->right = merge(left->right, right);
        update(left);
        return left;
    } else {
        right->left = merge(left, right->left);
        update(right);
        return right;
    }
}

template<typename T>
void ImplicitTree<T>::Insert(T data, int pos) {
    auto node = new Node<T>(data);
    auto pair = split(_root, pos);
    _root = merge(pair.first, ImplicitTree(node)._root);
    _root = merge(_root, pair.second);
}

template<typename T>
T ImplicitTree<T>::Find(int pos) {
    auto pair = split(_root, pos + 1);
    auto pair2 = split(pair.first, pos);
    T data = pair2.second->data;
    _root = merge(pair2.first, pair2.second);
    _root = merge(_root, pair.second);
    return data;

}

template<typename T>
void ImplicitTree<T>::Erase(int pos) {
    auto pair = split(_root, pos + 1);
    auto pair2 = split(pair.first, pos);
    _root = merge(pair2.first, pair.second);
}
