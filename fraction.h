#ifndef HEADER_FILE_FRACTION 
#define HEADER_FILE_FRACTION 

#include <iostream>

class Fraction {
	private:
		long _numerator, _denominator;
		long euclidean(long a, long b) const;
	public:
		Fraction();
		Fraction(long, long = 1);
		Fraction(const Fraction&) = default;

		long numerator()	const;
		long denominator()	const;
		void set_numerator(long);
		void set_denominator(long);

		bool reduce();
		static Fraction convert_double_to_fraction(double);
		double convert_fraction_to_double() const;
		Fraction	operator~ ();
		Fraction	operator++();
		Fraction	operator++(int);
		Fraction	operator--();
		Fraction	operator--(int);

		Fraction	operator+=(const Fraction&);
		Fraction	operator-=(const Fraction&);
		Fraction	operator*=(const Fraction&);
		Fraction	operator/=(const Fraction&);

		friend Fraction	operator+ (const Fraction&, const Fraction&);
		friend Fraction	operator- (const Fraction&, const Fraction&);
		friend Fraction	operator* (const Fraction&, const Fraction&);
		friend Fraction	operator/ (const Fraction&, const Fraction&);
		friend long		operator% (const Fraction&, const Fraction&);

		friend bool		operator< (const Fraction&, const Fraction&);
		friend bool		operator<=(const Fraction&, const Fraction&);
		friend bool		operator> (const Fraction&, const Fraction&);
		friend bool		operator>=(const Fraction&, const Fraction&);
		friend bool		operator==(const Fraction&, const Fraction&);
		friend bool		operator!=(const Fraction&, const Fraction&);

		explicit operator double()	const;
		explicit operator float()	const;
		explicit operator long()	const;
		explicit operator bool()	const;
};

bool		operator< (const Fraction&, const Fraction&);
bool		operator<=(const Fraction&, const Fraction&);
bool		operator> (const Fraction&, const Fraction&);
bool		operator>=(const Fraction&, const Fraction&);
bool		operator==(const Fraction&, const Fraction&);
bool		operator!=(const Fraction&, const Fraction&);
long		operator% (const Fraction&, const Fraction&);

Fraction	operator+ (const Fraction&, const Fraction&);
Fraction	operator+=(const Fraction&, const Fraction&);
Fraction	operator-=(const Fraction&, const Fraction&);
Fraction	operator- (const Fraction&, const Fraction&);
Fraction	operator* (const Fraction&, const Fraction&);
Fraction	operator*=(const Fraction&, const Fraction&);
Fraction	operator/ (const Fraction&, const Fraction&);
Fraction	operator/=(const Fraction&, const Fraction&);

class FractionInputFailException: public std::exception {
	public: virtual const char* what() const throw() { return "Incorrect Input"; }
};

long floor(const Fraction&);
long ceil(const Fraction&);

std::ostream& operator<<(std::ostream &, const Fraction &);
std::istream& operator>>(std::istream &, Fraction &);

#endif // HEADER_FILE_FRACTION 
