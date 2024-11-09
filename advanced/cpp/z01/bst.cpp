
#include <iostream>
struct Node
{
    Node* left;
    Node* right;
    Node* parent;
    int key;
};

Node* insert(Node*& root, int n)
{
    if (root == nullptr)
    {
        root = new Node{.left = nullptr, .right = nullptr, .parent = nullptr, .key = n};
        return root;
    }
    if (n < root->key)
    {
        auto tmp = insert(root->left, n);
        if (tmp == root->left)
            root->left->parent = root;
        return root->left;
    }
    else
    {
        auto tmp = insert(root->right, n);
        if (tmp == root->right)
            root->right->parent = root;
        return root->right;
    }
}

void free(Node*& root)
{
    if (root == nullptr)
        return;
    free(root->left);
    free(root->right);
    delete root;
    root = nullptr;
}

void print_in_order(const Node* root)
{
    if (root == nullptr)
        return;
    print_in_order(root->left);
    std::cout << root->key << " ";
    print_in_order(root->right);
}

void print_pre_order(const Node* root)
{
    if (root == nullptr)
        return;
    std::cout << root->key << " ";
    print_in_order(root->left);
    print_in_order(root->right);
}

void print_post_order(const Node* root)
{
    if (root == nullptr)
        return;
    print_in_order(root->left);
    print_in_order(root->right);
    std::cout << root->key << " ";
}

void verify(const Node* root, bool root_node)
{
    if (root == nullptr)
        return;
    if (root->left != nullptr && root->left->parent != root)
        throw std::logic_error("Invalid BST tree structure (left branch)");
    if (root->right != nullptr && root->right->parent != root)
        throw std::logic_error("Invalid BST tree structure (right branch)");
    if (root_node && root->parent != nullptr)
        throw std::logic_error("Invalid BST tree structure (parent)");
    if (root->left != nullptr && root->left->key >= root->key)
        throw std::logic_error("Invalid BST tree structure (left key comparison)");
    if (root->right != nullptr && root->right->key < root->key)
        throw std::logic_error("Invalid BST tree structure (right key comparison)");

    verify(root->left, false);
    verify(root->right, false);
}

void iota(Node*& root, int & counter)
{
    if (!root)
        return;
    iota(root->left, counter);
    root->key = counter++;
    iota(root->right, counter);
}

void iota(Node*& root)
{
    int n= 1;
    iota(root, n);
}


int main()
{
    Node* node = nullptr;
    insert(node, 8);
    insert(node, 3);
    insert(node, 6);
    insert(node, 10);
    insert(node, 4);
    insert(node, 14);
    insert(node, 7);
    insert(node, 13);
    insert(node, 1);

    verify(node, true);
    print_in_order(node);
    std::cout << "\n";
    print_pre_order(node);
    std::cout << "\n";
    print_post_order(node);
    std::cout << "\n";

    iota(node);
    verify(node, true);
    print_in_order(node);
    std::cout << "\n";
    print_pre_order(node);
    std::cout << "\n";
    print_post_order(node);
    std::cout << "\n";

    free(node);
}
