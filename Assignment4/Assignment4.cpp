// Assignment4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stack>

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

    bool isOperator(char op) 
    {
        if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^')
            return true;
        return false;
    }

    TreeNode* buildTree(const std::string& expression) 
    {
        std::stack<TreeNode*> stack;
        TreeNode* t, * t1, * t2;
        for (int c = 0; c < expression.size(); ++c) {
            if (c == '(') {

            }
            else if (c == ')') {

            }
            else if (!isOperator(expression[c])) {
                t = new TreeNode(expression[c]);
                stack.push(t);
            }
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

        root = stack.top();
        stack.pop();

        return root;
    }

    void printPostfix(TreeNode* n)
    {
        if (n != nullptr) {
            printPostfix(n->firstChild);
            printPostfix(n->nextSibling);
            std::cout << (n->element) << " ";
        }
    }

    void printInfix(TreeNode* n)
    {
        if (n != nullptr) {
            printInfix(n->firstChild);
            std::cout << (n->element) << " ";
            printInfix(n->nextSibling);
        }
    }

    void printPrefix(TreeNode* n)
    {
        if (n != nullptr) {
            std::cout << n->element << " ";
            printPrefix(n->firstChild);
            printPrefix(n->nextSibling);
        }
    }

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

class BST {
    TreeNode* root;

public:
    BST() : root(nullptr) {}


};

int main()
{
    std::string expression = "12+34*-";
    ExpressionTree tree;
    TreeNode* root = tree.buildTree(expression);
    tree.printInfix(root);
    std::cout << std::endl;
    tree.printPrefix(root);
    std::cout << std::endl;
    tree.printPostfix(root);
    std::cout << std::endl;
    int result = tree.evaluate(root);
    std::cout << result << std::endl;



}