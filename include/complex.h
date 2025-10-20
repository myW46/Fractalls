#pragma once
#include <cmath>
#include <iostream>

class Complex {
private:
	double real_;
	double imag_;

public:
	Complex(double real_ = 0.0, double imag_ = 0.0);

	double real() const;
	double imag() const;
	void Set_real(double newreal);
	void Set_imag(double newimag);
	Complex operator+(const Complex& other) const;
	Complex operator-(const Complex& other) const;
	Complex operator*(const Complex& other) const;
	void print() const;
	Complex square();
	double ABS() const;
	Complex operator*(double scalar) const;
	friend Complex operator*(double scalar, const Complex& complex);
	Complex(const Complex&) = default;
	Complex(Complex&&) noexcept = default;
	Complex& operator=(const Complex&) = default;
	Complex& operator=(Complex&&) noexcept = default;
	~Complex() = default;

};