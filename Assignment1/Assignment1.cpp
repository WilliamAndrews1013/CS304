// Assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <chrono>


/*
    PART 2A: We need to implement the Big 5 to the vecotr class because it works with dynamic memory allocation and it has ownership over that allocated memory compared to the complex class where it is just a complex number
    and doesn't use dynamic memory allocation.
*/

class complex {
private:
    double re, im;

public:
    complex(double r, double i) : re{ r }, im{ i } {}
    complex(double r) : re{ r }, im{ 0 } {}
    complex() : re{ 0 }, im{ 0 } {}
    complex(complex& z) : re{ z.re }, im{ z.im } {}

    double real() const { return re; }
    void real(double d) { re = d; }
    double imag() const { return im; }
    void imag(double d) { im = d; }

    complex& operator+=(complex z) {
        re += z.re;
        im += z.im;
        return *this;
    }

    complex& operator-=(complex z) {
        re -= z.re;
        im -= z.im;
        return *this;
    }


    // PART 1:
    complex& operator*=(complex z) {
        re *= z.re;
        im *= z.im;
        return *this;
    }

    complex& operator/=(complex z) {
        re /= z.re;
        im /= z.im;
        return *this;
    }
};

template<typename T>
class Vector {
    T* elem;
    int sz;
    int capacity;

public:
    // Deconstructor
    Vector(int s) : elem{ new T[s] }, sz{ s }, capacity{ s } {}
    
    ~Vector() { delete[] elem; }

    // Copy Constructor
    Vector(const Vector& v) : elem{ new T[v.sz] }, sz{ v.sz }, capacity{ v.sz }
    {
        for (int i = 0; i < sz; ++i)
            elem[i] = v.elem[i];
    }

    // Move Constructor
    Vector(Vector& rhs) : elem{ rhs.elem }, sz{ rhs.sz }, capacity{ rhs.sz }
    {
        rhs.elem = nullptr;
        rhs.sz = 0;
        rhs.capacity = 0;
    }

    // Copy Assignment
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

    // Move Assignment
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
};

int main()
{
    // PART 2B:
    std::cout << "Part 2B: " << std::endl;
    Vector<int> part2BVector(3);

    // Print initial vector elements
    std::cout << "\nInitial Vector elements: ";
    for (int i = 0; i < part2BVector.size(); ++i)
        std::cout << part2BVector[i] << " ";

    // Calling push_back function
    part2BVector.push_back(69);
    part2BVector.push_back(400);
    part2BVector.push_back(1000);

    // Print vector elements after push_back
    std::cout << "Vector elements after push_back: ";
    for (int i = 0; i < part2BVector.size(); ++i)
        std::cout << part2BVector[i] << " ";
    


    // PART 2C: Perform timing tests comparing your vector's performance to the STL vector
    const int vectorSize = 100;
    const int testSize = 10000;
    const int pushBackSize = 100000;

    // TEST 1: INSTANTIATE 10,000 VECTORS OF SIZE 100
    
    // Starts a timer
    auto start = std::chrono::high_resolution_clock::now();

    // Instantiate 10,000 of my vectors with sizes of 100.
    for (int i = 0; i < testSize; ++i)
        Vector<int> myVector(testSize);

    // End timer and get time
    auto end = std::chrono::high_resolution_clock::now();
    auto myVectorTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Start timer for STL Vectors
    start = std::chrono::high_resolution_clock::now();

    // Instantiare 10,000 STL vectors with sizes of 100
    for (int i = 0; i < testSize; ++i)
        std::vector<int> stlVector(vectorSize);
    

    // End timer and get time
    end = std::chrono::high_resolution_clock::now();
    auto stlVectorTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Print time results for test 1
    std::cout << "Part 2C: " << std::endl;
    std::cout << "\nTest 1 Results:" << std::endl;
    std::cout << "My Vector Time: " << myVectorTime << "ms" << std::endl;
    std::cout << "STL Vector Time: " << stlVectorTime << "ms" << std::endl;


    // TEST 2: FOR EACH CLASS, INSTANTIATE A VECTOR SIZE 1 AND ADD 100,000 ELEMENTS TO IT USING PUSH_BACK.
    
    // Starts Timer
    auto test2Start = std::chrono::high_resolution_clock::now();

    // Instantiate my vector of size and add 100,000 elements to it
    Vector<int> test2MyVector(1);
    for (int i = 0; i < pushBackSize; ++i)
        test2MyVector.push_back(i);

    // Ends timer and stores time in ms
    auto test2End = std::chrono::high_resolution_clock::now();
    auto test2MyVectorTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Starts timer
    test2Start = std::chrono::high_resolution_clock::now();

    // Instantiate STL vector of size 1 and add 100,000 elements to it
    std::vector<int> test2StlVector(1);
    for (int i = 0; i < pushBackSize; ++i)
        test2StlVector.push_back(i);

    // Ends timer and stores time in ms
    test2End = std::chrono::high_resolution_clock::now();
    auto test2StlVectorTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Prints out results for test 2
    std::cout << "\nTest 2 Results:" << std::endl;
    std::cout << "My Vector Push back Time: " << test2MyVectorTime << "ms" << std::endl;
    std::cout << "STL Vector Push Back Time: " << test2StlVectorTime << "ms" << std::endl;


    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
