#include "complex.h"

Complex::Complex(double real, double imag) :real_(real), imag_(imag) {}

double Complex::real() const {
	return real_;
}
double Complex::imag() const {
	return imag_;
}
void Complex::Set_real(double newreal) {
	real_ = newreal;	
}
void Complex::Set_imag(double newimag) {
	imag_ = newimag;
}
Complex Complex::operator+(const Complex& other) const{
	return Complex(real_ + other.real_, imag_ + other.imag_);
}
Complex Complex::operator-(const Complex& other) const {
	return Complex(real_ - other.real_, imag_ - other.imag_);
}
Complex Complex::operator*(const Complex& other) const {
	double new_real = real_ * other.real_ - imag_ * other.imag_;
	double new_imag = real_ * other.imag_ + imag_ * other.real_;
	return Complex(new_real, new_imag);
}
void Complex::print() const{
	std::cout << this->real_ << " + " << this->imag_ << "i" << std::endl;
}
Complex Complex::square() {
	double newreal = real_ * real_ - imag_ * imag_;
	double newimag = 2.0 * real_ * imag_;
	//Set_real(newreal);
	//Set_imag(newimag);
	return Complex(newreal, newimag);
}
double Complex::ABS() const{
	return(real_ * real_ + imag_ * imag_);
}
Complex Complex::operator*(double scalar) const {
	return Complex(real_ * scalar, imag_ * scalar);
}
Complex operator*(double scalar, const Complex& complex) {
	return complex * scalar; 
}
;