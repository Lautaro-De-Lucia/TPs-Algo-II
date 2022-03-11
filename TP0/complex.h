#ifndef COMPLEX_H
#define COMPLEX_H

#include "templatearray.h"
#include "config.h"
#include "errors.h"

#include <iostream>

#define PI 3.14159265358979

class Complex {
	double re_, im_;
public:
	Complex ();
	Complex (double);
	Complex (double, double);
	Complex (const Complex &);
	Complex const & operator = (Complex const &);
	Complex const & operator *= (Complex const &);
	Complex const & operator += (Complex const &);
	Complex const & operator -= (Complex const &);
	~Complex ();

	void set_re (double);
	void set_im (double);
	double get_re () const;
	double get_im () const;
	double abs () const;
	double abs2 () const;
	Complex const & conjugate ();
	Complex const getConjugate () const;
	bool zero () const;

	friend Array <Complex> & dft (Array <Complex> & arreglo_original);
	friend Array <Complex> & idft (Array <Complex> & arreglo_original);

	friend Complex const operator + (Complex const &, Complex const &);
	friend Complex const operator - (Complex const &, Complex const &);
	friend Complex const operator * (Complex const &, Complex const &);
	friend Complex const operator / (Complex const &, Complex const &);
	friend Complex const operator / (Complex const &, double);

	friend bool operator == (Complex const &, double);
	friend bool operator == (Complex const &, Complex const &);

	friend std::ostream & operator << (std::ostream &, const Complex &);
	friend std::istream & operator >> (std::istream &, Complex &);
};

#endif
