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

    TreeNode* getRoot() { return root; }

    void setRoot(char elem)
    {
        if (root == nullptr)
            root = new TreeNode(elem);
        else
            std::cout << "Root already exists." << std::endl;
    }

    bool isOperator(char op) 
    {
        if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^')
            return true;
        return false;
    }

    TreeNode* buildTree(std::string expression) 
    {
        std::stack<TreeNode*> stack;
        TreeNode* t, * t1, * t2;
        for (int c = 0; expression.size(); ++c) {
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

                t->firstChild = t1;
                t->nextSibling = t2;

                stack.push(t);
            }
        }

        root = stack.top();
        stack.pop();

        return root;
    }

    
};

void printExpressionTree(TreeNode* root) {
    if (root == nullptr) return;
    std::cout << root->element;
    if (root->firstChild != nullptr) {
        std::cout << "(";
        printExpressionTree(root->firstChild);
        std::cout << ")";
    }
    if (root->nextSibling != nullptr) {
        std::cout << root->nextSibling->element;
        printExpressionTree(root->nextSibling->nextSibling);
    }
}

int main()
{
    std::string expression = "825*+132*+/";
    ExpressionTree tree;
    TreeNode* root = tree.buildTree(expression);
    
    std::cout << root->element << std::endl;
}