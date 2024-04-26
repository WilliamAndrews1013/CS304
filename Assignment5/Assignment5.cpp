// Assignment5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <stack>
#include <list>
#include <vector>
#include <map>

template<typename KeyType, typename ValueType>
struct AvlNode {
    KeyType key;
    ValueType value;
    AvlNode* left;
    AvlNode* right;
    int height;

    AvlNode(const KeyType& k, const ValueType& v) : key{ k }, value{ v }, left { nullptr }, right{ nullptr }, height{ 1 } {}
};

template<typename KeyType, typename ValueType>
class MapADT {
    
    int numNodes;

    AvlNode<KeyType,ValueType>* copy(const AvlNode<KeyType, ValueType>* node) {
        if (node == nullptr)
            return nullptr;
        return new AvlNode<KeyType, ValueType>{ node->data, copy(node->left), copy(node->right) };
    }

    int height(AvlNode<KeyType,ValueType>* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }


    void balance(AvlNode<KeyType,ValueType>*& t) {
        if (t == nullptr)
            return;

        if (height(t->left) - height(t->right) > 1)
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);
        else if (height(t->right) - height(t->left) > 1)
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);
        t->height = std::max(height(t->left), height(t->right)) + 1;
    }

    void rotateWithLeftChild(AvlNode<KeyType,ValueType>*& k2) {
        AvlNode<KeyType,ValueType>* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1->height = std::max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }

    void rotateWithRightChild(AvlNode<KeyType,ValueType>*& k2) {
        AvlNode<KeyType,ValueType>* k1 = k2->right;
        k2->right = k1->left;
        k1->left = k2;
        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1->height = std::max(height(k1->right), k2->height) + 1;
        k2 = k1;
    }

    void doubleWithLeftChild(AvlNode<KeyType,ValueType>*& k3) {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void doubleWithRightChild(AvlNode<KeyType,ValueType>*& k3) {
        rotateWithLeftChild(k3->right);
        rotateWithRightChild(k3);
    }

public:
    AvlNode<KeyType, ValueType>* root;

    // Constructor
    MapADT<KeyType, ValueType>() : root{ nullptr }, numNodes{ 0 } {}

    // Destructor
    ~MapADT<KeyType,ValueType>() {
        clear(root);
    }

    // Copy constructor
    MapADT<KeyType, ValueType>(MapADT& rhs) : root{ nullptr }, numNodes{ 0 } {
        root = copy(rhs.root);
        numNodes = rhs.numNodes;
    }

    // Move constructor
    MapADT<KeyType, ValueType>(MapADT&& rhs) : root{ rhs.root }, numNodes{ rhs.numNodes } {
        rhs.root = nullptr;
        numNodes = 0;
    }

    // Copy assignment operator
    MapADT& operator=(const MapADT& rhs) {
        if (this != &rhs) {
            clear(root);
            root = copy(rhs.root);
            numNodes = rhs.root;
        }
    }

    // Move assignment operator
    MapADT& operator=(MapADT&& rhs) {
        if (this != &rhs) {
            clear(root);
            root = rhs.root;
            numNodes = rhs.numNodes;
            rhs.root = nullptr;
            rhs.numNodes = 0;
        }
    }

    ValueType& operator[](const KeyType& k) {

        // Used to search for key
        Iterator it = find(k);

        // If key exists return its value
        if (it != end()) {
            return it.current->value;
        }
        // If key doesn't exist, insert new key-value pair
        else {
            Iterator newInsert = insert(find(k), std::make_pair(k, ValueType{}));
            return newInsert.current->value;
        }
    }
    
    class Iterator
    {
        std::stack<AvlNode<KeyType,ValueType>*> nodes;
    public:
        AvlNode<KeyType,ValueType>* current;

        Iterator(AvlNode<KeyType, ValueType>* root) {
            if (root == nullptr) {
                current = nullptr;
                return;
            }
            
            while (root != nullptr) {
                nodes.push(root);
                root = root->left;
            }

            current = (nodes.empty() ? nullptr : nodes.top());
        }

        KeyType& operator*() {
            return current->key;
        }

        Iterator& operator++() {
            if (!nodes.empty()) {
                AvlNode<KeyType, ValueType>* node = nodes.top();
                nodes.pop();
                node = node->right;
                while (node != nullptr) {
                    nodes.push(node);
                    node = node->left;
                }
                current = (nodes.empty() ? nullptr : nodes.top());
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

    };

    Iterator begin() {
        return Iterator(root);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    Iterator insert(Iterator position, const std::pair<KeyType, ValueType>& val) {
        AvlNode<KeyType, ValueType>* parent = position.current;
        AvlNode<KeyType, ValueType>* newNode = nullptr;

        // Checks if current position is null or not
        if (parent == nullptr) {
            // Create new node and set as root
            root = new AvlNode<KeyType, ValueType>(val.first, val.second);
            newNode = root;
        }
        else {
            // Checks if key is less then parent key
            if (val.first < parent->key) {
                // If true, create new node and set as parent left child
                parent->left = new AvlNode<KeyType, ValueType>(val.first, val.second);
                newNode = parent->left;
            }
            // Checks if key is greater then parent key
            else if (val.first > parent->key) {
                // If true, create new node and set as parent right child
                parent->right = new AvlNode<KeyType, ValueType>(val.first, val.second);
                newNode = parent->right;
            }
            else {
                newNode = parent;
            }
        }

        balance(newNode);

        ++numNodes;

        return Iterator(newNode);
    }

    Iterator find(const KeyType& k) {
        AvlNode<KeyType, ValueType>* currentNode = root;

        // Checks if currentNode isn't null
        while (currentNode != nullptr) {
            // Check if KeyType is less then currentNode, if true put currentNode as it's left child
            if (k < currentNode->key)
                currentNode = currentNode->left;
            // If false, checks if currentNode is less then KeyType, if true, put currentNode as it's right child
            else if (currentNode->key < k) {
                currentNode = currentNode->right;
            }
            else {
                return Iterator(currentNode);
            }
        }

        return end();
    }
    
    void erase(Iterator position) {
        if (position.current == nullptr)
            return;

        AvlNode<KeyType, ValueType>* nodeToDelete = position.current;
        AvlNode<KeyType, ValueType>* parent = nullptr;

        // Find the parent of the node to delete
        if (nodeToDelete != root) {
            parent = root;
            while (parent != nullptr) {
                if ((parent->left != nullptr && parent->left == nodeToDelete) ||
                    (parent->right != nullptr && parent->right == nodeToDelete))
                    break;
                else if (nodeToDelete->key < parent->key)
                    parent = parent->left;
                else
                    parent = parent->right;
            }
        }

        // Node to delete is a leaf
        if (nodeToDelete->left == nullptr && nodeToDelete->right == nullptr) {
            if (parent != nullptr) {
                if (nodeToDelete->key < parent->key)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
            }
            else {
                root = nullptr;
            }
            delete nodeToDelete;
        }
        // Node to delete has one child
        else if (nodeToDelete->left == nullptr || nodeToDelete->right == nullptr) {
            AvlNode<KeyType, ValueType>* child = (nodeToDelete->left != nullptr) ? nodeToDelete->left : nodeToDelete->right;
            if (parent != nullptr) {
                if (nodeToDelete->key < parent->key)
                    parent->left = child;
                else
                    parent->right = child;
            }
            else {
                root = child;
            }
            delete nodeToDelete;
        }
        // Node to delete has two children
        else {
            AvlNode<KeyType, ValueType>* child2 = nodeToDelete->right;
            while (child2->left != nullptr)
                child2 = child2->left;

            nodeToDelete->key = child2->key;
            nodeToDelete->value = child2->value;

            // Remove the successor node from its parent
            erase(Iterator(child2));
        }

        // Rebalance the tree
        balance(root);

        // Update the number of nodes
        --numNodes;
    }

    void clear(AvlNode<KeyType, ValueType>* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    bool empty() const {
        return root == nullptr;
    }

    int size() const {
        return numNodes;
    }

    int max_size() const {
        return std::numeric_limits<int>::max();
    }

    Iterator findMin() {
        if (root == nullptr)
            return end();
        AvlNode<KeyType, ValueType>* current = root;
        if (current->left != nullptr)
            current = current->left;
        return Iterator(current);
    }

    Iterator findMax() {
        if (root == nullptr)
            return end();

        AvlNode<KeyType, ValueType>* current = root;
        while (current->right != nullptr)
            current = current->right;

        return Iterator(current);
    }

};



std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
    std::vector<int> result;
    MapADT<int, int> window;
    
    if (nums.empty() || k <= 0) 
        return result;

    // Populate the initial sliding window
    for (int i = 0; i < k; ++i) {
        window.insert(window.begin(), std::make_pair(nums[i], nums[i]));
    }

    // Add maximum of initial window to result
    result.push_back(window.findMax().current->key);

    // Slide the window and update result
    for (int i = k; i < nums.size(); ++i) {
        // Erase the element going out of the window
        window.erase(window.find(nums[i - k]));

        // Insert the new element coming into the window
        window.insert(window.begin(), std::make_pair(nums[i], nums[i]));

        // Add maximum of current window to result
        result.push_back(window.findMax().current->key);
    }

    return result;
}

int main() {

    // Example array of integers
    std::vector<int> nums = { 1, 3, -1, -3, 5, 3, 6, 7 };

    // Window size
    int k = 3;

    // Call the function
    std::vector<int> result = maxSlidingWindow(nums, k);

    // Output the result
    std::cout << "Maximum sliding window for array: ";
    for (int num : result) {
        std::cout << num << " ";
    }
    std::cout << std::endl;


    // Create an instance of MapADT
    MapADT<int, std::string> myMap;


    // Insert some key-value pairs into the map
    myMap.insert(MapADT<int, std::string>::Iterator(myMap.root), std::make_pair(42, "Hello"));
    myMap.insert(MapADT<int, std::string>::Iterator(myMap.root), std::make_pair(25, "World"));
    myMap.insert(MapADT<int, std::string>::Iterator(myMap.root), std::make_pair(10, "Goodbye"));

    myMap.erase(myMap.root);

    // Iterate through the map and print each entry
    for (auto it = myMap.begin(); it != myMap.end(); ++it) {
        
        std::cout << "Key: " << *it << ", Value: " << it.current->value << std::endl;
    }

    return 0;
}
