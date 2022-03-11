#include "complex.h"
#include "templatearray.h"
#include "config.h"
#include "errors.h"

#include <iostream>
#include <cmath>

using namespace std;

Complex::Complex () : re_ (0), im_ (0)
{
}

Complex::Complex (double r) : re_ (r), im_ (0)
{
}

Complex::Complex (double r, double i) : re_ (r), im_ (i)
{
}

Complex::Complex (Complex const & c) : re_ (c.re_), im_ (c.im_)
{
}

Complex const & Complex::operator = (Complex const & c)
{
	re_ = c.re_;
	im_ = c.im_;
	return *this;
}

Complex const & Complex::operator *= (Complex const & c)
{
	double re = re_ * c.re_
	         - im_ * c.im_;
	double im = re_ * c.im_
	         + im_ * c.re_;
	re_ = re, im_ = im;
	return *this;
}

Complex const & Complex::operator += (Complex const & c)
{
	double re = re_ + c.re_;
	double im = im_ + c.im_;
	re_ = re, im_ = im;
	return *this;
}

Complex const & Complex::operator -= (Complex const & c)
{
	double re = re_ - c.re_;
	double im = im_ - c.im_;
	re_ = re, im_ = im;
	return *this;
}

Complex::~Complex ()
{
}

void Complex::set_re (double r) 
{
	re_ = r;
}

void Complex::set_im (double i)
{
	im_ = i;
}

double Complex::get_re () const
{
	return re_;
}

double Complex::get_im () const
{
	return im_;
}

double Complex::abs () const
{
	return std::sqrt (re_ * re_ + im_ * im_);
}

double Complex::abs2 () const
{
	return re_ * re_ + im_ * im_;
}

Complex const & Complex::conjugate ()
{
	im_*= -1;
	return *this;
}

Complex const Complex::getConjugate () const
{
	return Complex (re_, -im_);
}

bool Complex::zero () const
{
#define ZERO(x) ((x) == +0.0 && (x) == -0.0)
	return ZERO(re_) && ZERO(im_) ? true : false;
}

Complex const operator + (Complex const & x, Complex const & y)
{
	Complex z (x.re_ + y.re_, x.im_ + y.im_);
	return z;
}

Complex const operator - (Complex const & x, Complex const & y)
{
	Complex r (x.re_ - y.re_, x.im_ - y.im_);
	return r;
}

Complex const operator * (Complex const & x, Complex const & y)
{
	Complex r (x.re_ * y.re_ - x.im_ * y.im_, x.re_ * y.im_ + x.im_ * y.re_);
	return r;
}

Complex const operator / (Complex const & x, Complex const & y)
{
	return x * y.getConjugate () / y.abs2 ();
}

Complex const operator / (Complex const & c, double f)
{
	return Complex (c.re_ / f, c.im_ / f);
}

bool operator == (Complex const & c, double f)
{
	bool b = (c.im_ != 0 || c.re_ != f) ? false : true;
	return b;
}

bool operator == (Complex const & x, Complex const & y)
{
	bool b = (x.re_ != y.re_ || x.im_ != y.im_) ? false : true;
	return b;
}

ostream & operator << (ostream & os, const Complex & c)
{
	return os << "(" 
	          << c.re_
	          << ", " 
	          << c.im_
	          << ")";
}

istream & operator >> (istream & is, Complex & c)
{
	int good = false;
	int bad  = false;
	double re = 0;
	double im = 0;
	char ch = 0;

	if (is >> ch && ch == '(')
	{
		if (is >> re
		    && is >> ch
		    && ch == ','
		    && is >> im
		    && is >> ch
		    && ch == ')')
			good = true;
		else
			bad = true;
	}

	else if (is.good ())
	{
		is.putback (ch);
		
		if (is >> re)
			good = true;
		
		else
			bad = true;
	}

	if (good)
		c.re_ = re, c.im_ = im;

	if (bad)
		is.clear (ios::badbit);

	return is;
}

Array <Complex> dft (Array <Complex> & originalArray)
{
	size_t N = originalArray.getSize ();
	
	if (!N)
	{
		printError(ERROR_NO_SAMPLES);
		exit (1);
	}

	Array <Complex> transformedArray (N);

	Complex aux = 0;
	Complex temp = 0;

	for (size_t k = 0; k <= N - 1; ++k)
	{
		for (size_t n = 0; n <= N - 1; ++n)
		{
			aux.set_re (cos((2*PI*k*n)/N));
			aux.set_im (-sin((2*PI*k*n)/N));
			temp += (originalArray[n] * (aux));
		}

		transformedArray[k] = temp;
		temp = 0;
	}

	return transformedArray;
}

Array <Complex> idft (Array <Complex> & originalArray)
{
	size_t N = originalArray.getSize ();

	if (!N)
	{
		printError(ERROR_NO_SAMPLES);
		exit (1);
	}

	Array <Complex> transformedArray (N);

	Complex aux = 0;
	Complex temp = 0;

	for (size_t k = 0; k <= N - 1; ++k)
	{
		for (size_t n = 0; n <= N - 1; ++n)
		{
			aux.set_re (cos((2*PI*k*n)/N));
			aux.set_im (sin((2*PI*k*n)/N));
			temp += (originalArray[n] * (aux));
		}

		transformedArray[k] = temp/N;
		temp = 0;
	}
	
	return transformedArray;
}

Array <Complex> fft (Array <Complex> & originalArray)
{
	size_t N = originalArray.getSize ();

	Complex zero;

	if (!N)
	{
		printError (ERROR_NO_SAMPLES);
		exit (1);
	}

	while (N & (N - 1))
    {
		originalArray.addItem (zero);
		N++;
	}

	return _fft (originalArray);

}

Array <Complex> _fft (Array <Complex> & originalArray)
{
	size_t N = originalArray.getSize ();
	size_t M = N / 2;

	Array <Complex> evenArray (M),oddArray (M);
	Array <Complex> transformedEvenArray (M),transformedOddArray (M);

	if (N <= 1)
		return originalArray;

	Array <Complex> transformedArray(N);


	for (size_t i = 0; i < M; i++)
	{
		evenArray[i] = originalArray[2 * i];
		oddArray[i] = originalArray[(2 * i) + 1];
	}

	transformedEvenArray = _fft (evenArray);
	transformedOddArray = _fft (oddArray);

	for (size_t k = 0 ; k < N ; k++ )
	{
		Complex aux = 0;
		aux.set_re (cos ((2*PI*k)/N));
		aux.set_im (-sin ((2*PI*k)/N));
		transformedArray[k] = (transformedEvenArray[k%M] + (aux * (transformedOddArray[k%M])));
	}

	return transformedArray;

}

Array <Complex> ifft (Array <Complex> & originalArray)
{
	size_t N = originalArray.getSize ();

	Complex zero;

	if (!N)
	{
		printError (ERROR_NO_SAMPLES);
		exit (1);
	}

	while (N & (N - 1))
    {
		originalArray.addItem (zero);
		N++;
	}

	originalArray = _ifft (originalArray);

	for (size_t i = 0; i < N; i++)
		originalArray[i] = (originalArray[i]/N);

	return originalArray;
}

Array <Complex> _ifft (Array <Complex> & originalArray)
{
	size_t N = originalArray.getSize ();
	size_t M = N / 2;

	Array <Complex> evenArray (M),oddArray (M);
	Array <Complex> transformedEvenArray (M),transformedOddArray (M);
	Array <Complex> transformedArray(N);

	if (N <= 1)
		return originalArray;

	for (size_t i = 0; i < M; i++)
	{
		evenArray[i] = originalArray[2 * i];
		oddArray[i] = originalArray[(2 * i) + 1];
	}

	transformedEvenArray = _ifft (evenArray);
	transformedOddArray = _ifft (oddArray);

	for (size_t k = 0 ; k < N ; k++ )
	{
		Complex aux = 0;
		aux.set_re (cos ((2*PI*k)/N));
		aux.set_im (sin ((2*PI*k)/N));
		transformedArray[k] = (transformedEvenArray[k%M] + (aux * (transformedOddArray[k%M])));
	}

	return transformedArray;
	
}