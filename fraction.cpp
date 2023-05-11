#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <exception>
#include "fraction.h"

Fraction::Fraction() 
	:_numerator(0)
	,_denominator(1)
{}

Fraction::Fraction(long Numerator, long Denominator)
	: _numerator(Numerator)
	, _denominator(Denominator)
{}

long Fraction::euclidean(long a, long b) const {
	return b == 0 ? a : this->euclidean(b, a % b);
}

long Fraction::numerator() const {
	return this->_numerator;
}

long Fraction::denominator() const {
	return this->_denominator;
}

void Fraction::set_numerator(long Numerator) {
	this->_numerator = Numerator;
}

void Fraction::set_denominator(long Denominator) {
	this->_denominator = Denominator;
}

bool Fraction::reduce(void) {
	long gcd(this->euclidean(this->_numerator, this->_denominator));

	if (1 < gcd) {
		this->_numerator /= gcd;
		this->_denominator /= gcd;

		return true;
	} else {
		return false;
	}
}

Fraction Fraction::convert_double_to_fraction(double Number) {
	long _denominator = 1;

	while(((double)(int)Number) != Number) {
		Number = Number * 10;
		if (10000 < (_denominator = _denominator * 10))
			break;
	}

	long _numerator = (long)Number;

	Fraction out(_numerator, _denominator);
	out.reduce();

	return out;
}

double Fraction::convert_fraction_to_double(void) const {
	return (double)this->_numerator / (double)this->_denominator;
}

bool operator<(const Fraction& f, const Fraction& s) {
	return (f._numerator * s.denominator()) < (s.numerator() * f._denominator);
}

bool operator<=(const Fraction& f, const Fraction& s) {
	return (f._numerator * s.denominator()) <= (s.numerator() * f._denominator);
}

bool operator>(const Fraction& f, const Fraction& s) {
	return (f._numerator * s.denominator()) > (s.numerator() * f._denominator);
}

bool operator>=(const Fraction& f, const Fraction& s) {
	return (f._numerator * s.denominator()) >= (s.numerator() * f._denominator);
}

bool operator==(const Fraction& f, const Fraction& s) {
	return (f._numerator * s.denominator()) == (s.numerator() * f._denominator);
}

bool operator!=(const Fraction& f, const Fraction& s) {
	return (f._numerator * s.denominator()) != (s.numerator() * f._denominator);
}

long operator%(const Fraction& f, const Fraction& s) {
	long result;

	result = ((f._numerator   * s.denominator()) % 
			  (f._denominator * s.numerator())) / 
			  (f._denominator * s.denominator());

	return result;
}

Fraction::operator double() const {
	return this->convert_fraction_to_double();
}

Fraction::operator float() const {
	return (float)this->convert_fraction_to_double();
}

Fraction::operator long() const {
	return (long)this->convert_fraction_to_double();
}
Fraction::operator bool() const {
	return _numerator;
}

Fraction operator+(const Fraction& f, const Fraction& s) {
	Fraction resultFraction;

	if (f._denominator == s.denominator()) {
		resultFraction.set_numerator(f._numerator + s.numerator());
		resultFraction.set_denominator(f._denominator);
	} else {
		resultFraction.set_numerator((f._numerator * s.denominator()) + (s.numerator() * f._denominator));
		resultFraction.set_denominator(f._denominator * s.denominator());
	}
    resultFraction.reduce();

	return resultFraction;
}

Fraction Fraction::operator+=(const Fraction& fraction) {
	if (this->_denominator == fraction.denominator()) {
		this->_numerator += fraction.numerator();
	} else {
		this->_numerator = (this->_numerator * fraction.denominator()) + (fraction.numerator() * this->_denominator);
		this->_denominator *= fraction.denominator();
	}

    reduce();
    return *this;
}

Fraction operator-(const Fraction& f, const Fraction& s) {
	Fraction resultFraction;

	if (f._denominator == s.denominator()) {
		resultFraction.set_numerator(f._numerator - s.numerator());
		resultFraction.set_denominator(f._denominator);
	} else {
		resultFraction.set_numerator((f._numerator * s.denominator()) - (s.numerator() * f._denominator));
		resultFraction.set_denominator(f._denominator * s.denominator());
	}

    resultFraction.reduce();
    return resultFraction;
}

Fraction Fraction::operator-=(const Fraction& fraction) {
	if (this->_denominator == fraction.denominator()) {
		this->_numerator -= fraction.numerator();
	} else {
		this->_numerator = (this->_numerator * fraction.denominator()) - (fraction.numerator() * this->_denominator);
		this->_denominator *= fraction.denominator();
	}

    reduce();
    return *this;
}

Fraction operator*(const Fraction& f, const Fraction& s) {
	Fraction resultFraction;

	resultFraction.set_numerator(f._numerator * s.numerator());
	resultFraction.set_denominator(f._denominator * s.denominator());

    resultFraction.reduce();
    return resultFraction;
}

Fraction Fraction::operator*=(const Fraction& fraction) {
	this->_denominator *= fraction.denominator();
	this->_numerator *= fraction.numerator();


    reduce();
    return *this;
}

Fraction operator/(const Fraction& f, const Fraction& s) {
	Fraction resultFraction;

	resultFraction.set_denominator(f._denominator * s.numerator());
	resultFraction.set_numerator(f._numerator * s.denominator());

    resultFraction.reduce();
    return resultFraction;
}

Fraction Fraction::operator/=(const Fraction& fraction) {
	this->_denominator *= fraction.numerator();
	this->_numerator *= fraction.denominator();

    reduce();
    return *this;
}

Fraction Fraction::operator~(void) {
	Fraction resultFraction;

	if(this->_numerator > this->_denominator) {
		return *this;
	} else {
		resultFraction.set_numerator(this->_denominator - this->_numerator);
		resultFraction.set_denominator(this->_denominator);

		return resultFraction;
	}
}

Fraction Fraction::operator++() {
	this->_numerator += 1;

	return *this;
}
Fraction Fraction::operator++(int) {
	Fraction tmp = *this;
	operator++();
	return tmp;
}

Fraction Fraction::operator--() {
	this->_numerator -= 1;

	return *this;
}

Fraction Fraction::operator--(int) {
	Fraction tmp = *this;
	operator--();
	return tmp;
}

long floor(const Fraction& fraction) {
	return fraction.numerator() / fraction.denominator();
}

long ceil(const Fraction& fraction) {
	return fraction.numerator() / fraction.denominator()
		+ (fraction.numerator() % fraction.denominator() != 0);
}

std::ostream& operator<<(std::ostream &out, const Fraction &fraction) {
	out << fraction.numerator();
	if (fraction.denominator() != 1)
		out << "/" << fraction.denominator();

	return out;
}

bool convert_string_to_fraction(std::string fraction_string, Fraction& fraction) {
	std::size_t pos = fraction_string.find("/");

	if (pos != std::string::npos) {
		try {
			fraction.set_numerator(atol(fraction_string.substr(0, pos).c_str()));
			fraction.set_denominator(atol(fraction_string.substr(pos + 1).c_str()));
		} catch(...) {
			return false;
		}

		return (fraction.denominator() == 0) ? false : true;
	}

	return false;
}

std::istream& operator>>(std::istream &in, Fraction &fraction) {
	std::string input;

	in >> input;

	if (false == convert_string_to_fraction(input, fraction)) {
		// Throw own exception object
		throw FractionInputFailException();
	}

	return in;
}
