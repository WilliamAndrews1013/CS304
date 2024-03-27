// Assignment4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
    In the best case the time complexity will be O(nlogn) and in the worst case it'll be O(n^2). When comparing to Merge Sort, Tree Sort is much faster then Merge Sort
*/


#include <iostream>
#include <stack>
#include <vector>
#include <chrono>

struct TreeNode {
    char element;
    TreeNode* firstChild;
    TreeNode* nextSibling;

    TreeNode(char elem) 
    {
        element = elem;
        firstChild = nullptr;
        nextSibling = nullptr;
    }
};

class ExpressionTree {
    TreeNode* root;

public:
    ExpressionTree() : root(nullptr) {}

    // Checks if the char is an operator or operand
    bool isOperator(char op) 
    {
        if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^')
            return true;
        return false;
    }

    // Builds an Expression Tree based on an input string that is a postFix expression
    TreeNode* buildTree(const std::string& expression) 
    {
        std::stack<TreeNode*> stack;
        TreeNode* t, * t1, * t2;

        // Loops through expression string
        for (int c = 0; c < expression.size(); ++c) {
            
            // Checks for brackets
            if (c == '(') {

            }
            else if (c == ')') {

            }
            // Creates a new node if char is not an operator
            else if (!isOperator(expression[c])) {
                t = new TreeNode(expression[c]);
                stack.push(t);
            }
            // If char is an operator, creates a new node with the 2 most recent numbers in stack 
            else {
                t = new TreeNode(expression[c]);

                t1 = stack.top();
                stack.pop();
                t2 = stack.top();
                stack.pop();

                t->firstChild = t2;
                t->nextSibling = t1;

                stack.push(t);
            }
        }

        // Root is the last char in the expression string
        root = stack.top();
        stack.pop();

        return root;
    }

    // Does a post-order traversal and prints
    void printPostfix(TreeNode* n)
    {
        if (n != nullptr) {
            printPostfix(n->firstChild);
            printPostfix(n->nextSibling);
            std::cout << (n->element) << " ";
        }
    }

    // Does a in-order traversal and prints
    void printInfix(TreeNode* n)
    {
        if (n != nullptr) {
            printInfix(n->firstChild);
            std::cout << (n->element) << " ";
            printInfix(n->nextSibling);
        }
    }

    // Does a pre-order traversal and prints
    void printPrefix(TreeNode* n)
    {
        if (n != nullptr) {
            std::cout << n->element << " ";
            printPrefix(n->firstChild);
            printPrefix(n->nextSibling);
        }
    }

    // Evaluates the expression (if expression is all numbers)
    int evaluate(TreeNode* n) 
    {
        if (n == nullptr)
            return 0;

        if (n->firstChild == nullptr && n->nextSibling == nullptr)
            return n->element - '0';

        int left = evaluate(n->firstChild);
        int right = evaluate(n->nextSibling);

        switch (n->element) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return left / right;
        }

        return 0;
    }
    
};

template<typename T>
class BST {
    TreeNode* root;

public:
    BST() : root{ nullptr } {}

    // Destructor
    ~BST() { makeEmpty(root); }

    // Copy constructor
    BST(const BST& rhs) : root{ nullptr }
    {
        root = clone(rhs.element);
    }

    // Copy assignment operator
    BST& operator=(const BST& rhs) {
        if (this != &rhs) {
            makeEmpty(root);
            root = clone(rhs.clone);
        }
        return *this;
    }

    // Move Constructor
    BST(BST&& rhs) : root{ nullptr }
    {
        root = rhs.root;
        rhs.root = nullptr;
    }

    // Move asignment operator
    BST& operator=(BST&& rhs) {
        if (this != &rhs) {
            makeEmpty(root);
            root = rhs.root;
            rhs.root = nullptr;
        }
    }

    // Inserts a new node into BST at proper position
    TreeNode* insert(T x, TreeNode*& t)
    {
        if (t == nullptr)
            t = new TreeNode(x);
        else if (x < t->element)
            insert(x, t->firstChild);
        else if (x > t->element)
            insert(x, t->nextSibling);
        else
            ;

        return t;
    }

    // Removes a node from BST and fixes it so it stays a BST
    void remove(T x, TreeNode*& t)
    {
        if (t == nullptr)
            return;
        if (x < t->element)
            remove(x, t->firstChild);
        else if (x > t->element)
            remove(x, t->nextSibling);
        else if (t->firstChild != nullptr && t->nextSibling != nullptr) {
            t->element = findMin(t->nextSibling)->element;
            remove(t->element, t->nextSibling);
        }
        else {
            TreeNode* oldNode = t;
            t = (t->firstChild != nullptr) ? t->firstChild : t->nextSibling;
            delete oldNode;
        }
    }

    // Finds smallest node
    TreeNode* findMin(TreeNode* t) const
    {
        if (t == nullptr)
            return nullptr;
        if (t->firstChild == nullptr)
            return t;
        return findMin(t->firstChild);
    }

    // Finds biggest node
    TreeNode* findMax(TreeNode* t) const
    {
        if (t != nullptr)
            while (t->nextSibling != nullptr)
                t = t->nextSibling;
        return t;
    }

    // Clears BST
    void makeEmpty(TreeNode*& t)
    {
        if (t != nullptr) {
            makeEmpty(t->firstChild);
            makeEmpty(t->nextSibling);
            delete t;
        }

        t = nullptr;
    }

    // Copy helper
    TreeNode* clone(TreeNode* t) const
    {
        if (t == nullptr)
            return nullptr;
        else
            return new TreeNode(t->element, clone(t->firstChild), clone(t->nextSibling));
    }

    // Sorts a vector of elements using in-order traversal
    std::vector<T> treeSort(std::vector<T> elems)
    {
        TreeNode* root = nullptr;
        for (auto val : elems)
            root = insert(val, root);

        std::vector<T> sortedVals;
        inOrder(root, sortedVals);
        return sortedVals;
    }

    // In-order traversal
    void inOrder(TreeNode* t, std::vector<T>& arr)
    {
        if (t != nullptr) {
            inOrder(t->firstChild, arr);
            arr.push_back(t->element);
            inOrder(t->nextSibling, arr);
        }
    }
};


void merge(std::vector<int>& A, int p, int q, int r)    // c*O(n) = O(n)
{
    int n1 = q - p + 1; // Size of left array O(1)
    int n2 = r - q; // Size of right array O(1)

    std::vector<int> leftArray(n1 + 1); // O(n)
    std::vector<int> rightArray(n2 + 1);    // O(n)

    for (int i = 0; i < n1; ++i)    // O(n)
        leftArray[i] = A[p + i];
    for (int i = 0; i < n2; ++i)    // O(n)
        rightArray[i] = A[q + i + 1];

    leftArray[n1] = 9999999;    // O(1)
    rightArray[n2] = 9999999;   // O(1)

    int i = 0;  // O(1)
    int j = 0;  // O(1)

    for (int k = p; k <= r; ++k)    // O(n)
    {
        if (leftArray[i] <= rightArray[j])
        {
            A[k] = leftArray[i];
            ++i;
        }
        else
        {
            A[k] = rightArray[j];
            ++j;
        }
    }
}

void mergeSort(std::vector<int>& A, int p, int r) // O(nlogn)
{
    if (p < r)
    {
        int q = (p + r) / 2;
        mergeSort(A, p, q);
        mergeSort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

int main()
{
    // ----TEST PART 1----
    std::cout << "Part 1:" << std::endl;
    std::string intExpression = "12+34*-";
    ExpressionTree tree;
    TreeNode* root = tree.buildTree(intExpression);
    tree.printInfix(root);
    std::cout << std::endl;
    tree.printPrefix(root);
    std::cout << std::endl;
    tree.printPostfix(root);
    std::cout << std::endl;
    int result = tree.evaluate(root);
    std::cout << result << std::endl;
    std::cout << std::endl;

    std::string expression = "ab+cd*-";
    ExpressionTree tree2;
    TreeNode* root2 = tree2.buildTree(expression);
    tree.printInfix(root2);
    std::cout << std::endl;
    tree.printPrefix(root2);
    std::cout << std::endl;
    tree.printPostfix(root2);
    std::cout << std::endl;
    std::cout << std::endl;

    // ----TEST PART 2-----
    std::cout << "Part 2:" << std::endl;

    std::vector<int> arr = { 74, 65, 96, 72, 44, 8, 77, 7, 21, 83, 30, 37, 46, 12, 56,10, 85, 69, 24, 75, 98, 6, 15, 39, 42, 38, 2, 58, 25, 91, 2, 76, 95, 26, 1, 88, 92, 35, 13, 34, 28, 94, 49, 90, 27, 4, 50, 86, 93, 9, 100};
    BST<int> bstTree;

    // Timing for treeSort
    auto treeSort_start_time = std::chrono::high_resolution_clock::now();
    std::vector<int> sortedArray = bstTree.treeSort(arr);
    auto treeSort_end_time = std::chrono::high_resolution_clock::now();
    auto treeSort_duration = std::chrono::duration_cast<std::chrono::microseconds>(treeSort_end_time - treeSort_start_time);

    auto mergeSort_start_time = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, arr.size() - 1);
    auto mergeSort_end_time = std::chrono::high_resolution_clock::now();
    auto mergeSort_duration = std::chrono::duration_cast<std::chrono::microseconds>(mergeSort_end_time - mergeSort_start_time);

    std::cout << "Tree sort" << std::endl;
    for (auto val : sortedArray) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    std::cout << "Time: " << (double)(treeSort_duration.count() / 1000.0) << "ms" << std::endl;
    std::cout << std::endl;

    std::cout << "MergeSort" << std::endl;
    for (auto elem : arr)
        std::cout << elem << ", ";
    std::cout << std::endl;
    std::cout << "Time: " << (double)(mergeSort_duration.count() / 1000.0) << "ms" << std::endl;
}