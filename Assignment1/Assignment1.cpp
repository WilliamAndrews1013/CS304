// Assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


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

int main()
{
    std::cout << "Hello World!\n";

    complex z = { 1,0 };
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
