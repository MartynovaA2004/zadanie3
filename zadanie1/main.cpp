#include <iostream>
#include <algorithm>

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;
    int height;

    Node(const T& value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
class AVLTree {
private:
    Node<T>* root;

    int getHeight(Node<T>* node);
    int getBalanceFactor(Node<T>* node);
    Node<T>* rotateRight(Node<T>* y);
    Node<T>* rotateLeft(Node<T>* x);
    Node<T>* insert(Node<T>* node, const T& value);
    Node<T>* remove(Node<T>* node, const T& value);
    Node<T>* findMin(Node<T>* node);
    void inOrder(Node<T>* node);
    void destroyTree(Node<T>* node);


public:
    AVLTree() : root(nullptr) {}

    void insert(const T& value);
    void remove(const T& value);
    void printInOrder();
    bool search(const T& value);
    ~AVLTree();
};

template <typename T>
int AVLTree<T>::getHeight(Node<T>* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

template <typename T>
int AVLTree<T>::getBalanceFactor(Node<T>* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template <typename T>
Node<T>* AVLTree<T>::rotateRight(Node<T>* y) {
    Node<T>* x = y->left;
    Node<T>* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

template <typename T>
Node<T>* AVLTree<T>::rotateLeft(Node<T>* x) {
    Node<T>* y = x->right;
    Node<T>* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

template <typename T>
Node<T>* AVLTree<T>::insert(Node<T>* node, const T& value) {
    if (node == nullptr)
        return new Node<T>(value);

    if (value < node->data)
        node->left = insert(node->left, value);
    else if (value > node->data)
        node->right = insert(node->right, value);
    else
        return node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1) {
        if (value < node->left->data) {
            return rotateRight(node);
        }
        else {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }

    if (balance < -1) {
        if (value > node->right->data) {
            return rotateLeft(node);
        }
        else {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;
}

template <typename T>
Node<T>* AVLTree<T>::remove(Node<T>* node, const T& value) {
    if (node == nullptr)
        return node;

    if (value < node->data)
        node->left = remove(node->left, value);
    else if (value > node->data)
        node->right = remove(node->right, value);
    else {
        if (node->left == nullptr || node->right == nullptr) {
            Node<T>* temp = (node->left != nullptr) ? node->left : node->right;

            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            }
            else {
                *node = *temp;
            }

            delete temp;
        }
        else {
            Node<T>* temp = findMin(node->right);
            node->data = temp->data;
            node->right = remove(node->right, temp->data);
        }
    }

    if (node == nullptr)
        return node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1) {
        if (getBalanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }
        else {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }

    if (balance < -1) {
        if (getBalanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }
        else {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;
}

template <typename T>
Node<T>* AVLTree<T>::findMin(Node<T>* node) {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

template <typename T>
void AVLTree<T>::inOrder(Node<T>* node) {
    if (node == nullptr)
        return;

    inOrder(node->left);
    std::cout << node->data << " ";
    inOrder(node->right);
}

template <typename T>
void AVLTree<T>::insert(const T& value) {
    root = insert(root, value);
}

template <typename T>
void AVLTree<T>::remove(const T& value) {
    root = remove(root, value);
}

template <typename T>
void AVLTree<T>::printInOrder() {
    inOrder(root);
    std::cout << std::endl;
}

template <typename T>
bool AVLTree<T>::search(const T& value) {
    Node<T>* current = root;

    while (current != nullptr) {
        if (value < current->data)
            current = current->left;
        else if (value > current->data)
            current = current->right;
        else
            return true; // Найден
    }

    return false; // Не найден
}

template <typename T>
AVLTree<T>::~AVLTree() {
    destroyTree(root);
}

template <typename T>
void AVLTree<T>::destroyTree(Node<T>* node) {
    if (node == nullptr)
        return;

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}
int main() {
    AVLTree<int> avl;
    avl.insert(50);
    avl.insert(30);
    avl.insert(70);
    avl.insert(20);
    avl.insert(40);
    avl.insert(60);
    avl.insert(80);

    std::cout << "In-order traversal: ";
    avl.printInOrder();

    avl.remove(30);

    std::cout << "In-order traversal after removing 30: ";
    avl.printInOrder();

    std::cout << "Searching for 60: " << (avl.search(60) ? "Found" : "Not found") << std::endl;
    std::cout << "Searching for 30: " << (avl.search(30) ? "Found" : "Not found") << std::endl;

    return 0;
}
