// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

template<typename T>
class Vector {
    T* elem;
    int sz;
    int capacity;

public:
    Vector(int initialSize = 0) : elem{ new T[initialSize] }, sz{ 0 }, capacity{ initialSize } {}

    ~Vector() { delete[] elem; }

    Vector(const Vector& v) : elem{ new T[v.sz] }, sz{ v.sz }, capacity{ v.sz }
    {
        for (int i = 0; i < sz; ++i)
            elem[i] = v.elem[i];
    }

    Vector(Vector&& rhs) : elem{ rhs.elem }, sz{ rhs.sz }, capacity{ rhs.sz }
    {
        rhs.elem = nullptr;
        rhs.sz = 0;
        rhs.capacity = 0;
    }

    Vector& operator=(const Vector& v)
    {
        T* p = new T[v.sz];
        for (int i = 0; i != v.sz; ++i)
        {
            p[i] = v.elem[i];
        }
        delete[] elem;
        elem = p;
        sz = v.sz;
        capacity = v.sz;

        return *this;
    }

    Vector& operator=(const Vector&& rhs)
    {
        if (this != &rhs)
        {
            delete[] elem;
            elem = rhs.elem;
            sz = rhs.sz;
            capacity = rhs.sz;
            rhs.elem = nullptr;
            rhs.sz = 0;
            rhs.capacity = 0;
        }

        return *this;
    }

    T& operator[](int index) { return elem[index]; }

    int size() const { return sz; }

    void push_back(const T& v)
    {
        // Check if the size of the array is maxed out
        if (sz == capacity)
        {
            // Allocate additional space and create a new array with the additional space
            int additionalSpace = (capacity == 0) ? 1 : 2 * capacity;
            T* newArray = new T[additionalSpace];

            // Move the elems from old array to new array
            for (int i = 0; i < sz; ++i)
            {
                newArray[i] = elem[i];
            }

            delete[] elem;
            elem = newArray;
            capacity = additionalSpace;
        }

        // Add the new element to the end of the vector
        elem[sz] = v;
        ++sz;
    }

    bool empty() const {
        return sz == 0;
    }

    void pop() {
        if (!empty()) {
            --sz;
        } else {
            std::cout << "Stack is empty" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    T& top() {
        if (!empty()) {
            return elem[sz - 1];
        }
        else {
            std::cout << "Stack is empty" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
};

template<typename T>
class Stack {
    Vector<T> elems;

public:
    T top() { return elems.top(); }
    void push(T elem) { return elems.push_back(elem); }
    void pop() { elems.pop(); }
    bool isEmpty() { return elems.empty(); }
};


class Expression {
    std::string infix;
    std::string postfix;
    
    // Determine the priority of an operator
    static int getPriotity(char c) {
        if (c ==  '+' || c == '-') {
            return 1;
        }
        else if (c == '*' || c == '/') {
            return 2;
        }
        else {
            return 0;
        }
    }

    // Check if char is an operator
    static bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

public:

    Expression(const std::string& input, int direction) {
        if (direction == 1) {
            infix = input;
        }
        else if (direction == 2) {
            postfix = input;
        }
    }

    // Convert infix to postfix
    std::string inToPost() {
        Stack<char> operatorStack;
        std::string _postfix;

        for (char i : infix) {
            if (isdigit(i)) {
                _postfix += i;
                _postfix += " ";
            }
            else if (isOperator(i)) {
                while (!operatorStack.isEmpty() && getPriotity(operatorStack.top()) >= getPriotity(i)) {
                    _postfix += operatorStack.top();
                    _postfix += " ";
                    operatorStack.pop();
                }
                operatorStack.push(i);
            }
        }

        while (!operatorStack.isEmpty()) {
            _postfix += operatorStack.top();
            _postfix += " ";
            operatorStack.pop();
        }

        this->postfix = _postfix;
        return _postfix;
    }

    // Convert postfix to infix
    std::string postToIn() {
        Stack<std::string> operandStack;
        std::string _infix;

        if (postfix.empty()) {
            throw "No expression provided.";
        }
        for (int i = 0; i < postfix.length(); ++i) {
            if (isdigit(postfix[i])) {
                std::string operand;
                while (i < postfix.length() && isdigit(postfix[i])) {
                    operand += postfix[i];
                    ++i;
                }
                operandStack.push(operand);
                --i;
            }
            else if (isOperator(postfix[i])) {
                try {
                    std::string operand1 = operandStack.top();
                    operandStack.pop();

                    if (operandStack.isEmpty()) {
                        throw "Invalid expression. Try adding a space between operands";
                    }
                    else {
                        std::string operand2 = operandStack.top();
                        operandStack.pop();
                        std::string newOperand = "(" + operand2 + postfix[i] + operand1 + ")";
                        operandStack.push(newOperand);
                    }
                }
                catch (const char* msg) {
                    std::cout << msg << std::endl;
                    exit(1);
                }
            }
        }

        _infix = operandStack.top();
        this->infix = _infix;
        return _infix;
    }


    // Evaluate postfix expression
    double evaluate() {
        Stack<double> operandStack;

        for (int i = 0; i < postfix.length(); ++i) {
            if (isdigit(postfix[i])) {
                std::string operand;

                while (i < postfix.length() && isdigit(postfix[i])) {
                    operand += postfix[i];
                    i++;
                }
                operandStack.push(std::stod(operand));
                --i;
            }
            else if (isOperator(postfix[i])) {
                double operand1 = operandStack.top();
                operandStack.pop();
                double operand2 = operandStack.top();
                operandStack.pop();

                double result;
                switch (postfix[i]) {
                    case '+':
                        result = operand2 + operand1;
                        break;
                    case '-':
                        result = operand2 - operand1;
                        break;
                    case '*':
                        result = operand2 * operand1;
                        break;
                    case '/':
                        result = operand2 / operand1;
                        break;
                }
                operandStack.push(result);
            }
        }
        return operandStack.top();
    }
};

// Prints out the menu screen
void menu() {
    std::cout << std::endl;
    std::cout << "Select an option:" << std::endl;
    std::cout << "1: Infix to Postfix" << std::endl;
    std::cout << "2: Postfix to Infix" << std::endl;
    std::cout << "3: Evaluate Expression" << std::endl;
    std::cout << "5: Quit" << std::endl;
    std::cout << "Enter Choice: ";
}

// Quit the program
void quit() {
    std::cout << std::endl;
    std::cout << "Goodbye!" << std::endl;
    exit(0);
}

// Checks if the choice is valid
bool isValidChoice(int choice) {
    if (choice < 1 || choice > 5 || std::cin.fail()) {
        std::cout << std::endl;
        std::cout << "Invalid Choice!" << std::endl;
        return false;
    }
    else if (choice == 5) {
        quit();
    }

    return true;
}

// This function handles the user input and calls the proper funciton based on the expression entered
void handleInput(const std::string& input, int choice) {
    int direction;

    if (choice == 1)
        direction = 1;
    else if (choice == 2)
        direction = 2;
    else if (choice == 3)
        direction = 2;
    else
        direction = 0;

    Expression expression(input, direction);

    switch (choice) {
        case 1:
            std::cout << "The postfix expression is: " << expression.inToPost() << std::endl;
            break;
        case 2:
            std::cout << "The infix expression is: " << expression.postToIn() << std::endl;
            break;
        case 3:
            std::cout << "Evaluated expression: " << expression.postToIn() << std::endl;
            std::cout << "The result is: " << expression.evaluate() << std::endl;
            break;
        case 5:
            std::cout << "GoodBye!" << std::endl;
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
    }
}

int main()
{
    std::string input;
    int choice;

    for (;;) {
        menu();

        std::cin >> choice;

        if (isValidChoice(choice))
            break;
    }

    if (choice == 1) {
        std::cout << "Enter expression in this format: 2+2" << std::endl;
        std::cout << "Enter an expression: ";
    }
    else if (choice == 2) {
        std::cout << "Enter expression in this format: 2 2 +. Make sure to add a space after every operator and number" << std::endl;
        std::cout << "Enter an expression: ";
    }
    else if (choice == 3)
        std::cout << "Enter expression in this format: 2 2 +. Make sure to add a space after every operator and number" << std::endl;
        std::cout << "Enter an expression: ";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    handleInput(input, choice);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
