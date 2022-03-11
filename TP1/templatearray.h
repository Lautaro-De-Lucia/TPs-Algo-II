#ifndef TEMPLATEARRAY_H
#define TEMPLATEARRAY_H

#include "config.h"
#include "errors.h"

#include <iostream>
#include <iomanip>
#include <cstdlib> 
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#define GROWTH_FACTOR 2

template <typename T>

class Array
{

	public:

		template <typename U>
		friend ostream & operator << (ostream &, const Array<U> &);
		
		template <typename U>
		friend istream & operator >> (istream &, Array<U> &);

		Array (size_t = 10); 
		Array (const Array &); 
		~Array ();

		status_t loadArray ();
		status_t loadArray (ifstream &);
		status_t addItem (T item);

		size_t getAllocSize () const;
		size_t getSize () const;
		const Array & operator = (const Array &); 
		bool operator == (const Array &) const;
		bool operator != (const Array &rArray) const {return ! (*this == rArray);}

		T & operator[ ] (size_t);
		T operator[ ] (size_t) const;

		status_t resize ();
	
	private:
	
		size_t allocSize_;
		size_t size_;
		T *arr_;
};

template <typename T>
Array <T>::Array (size_t arraySize)
{
	allocSize_ = (arraySize > 0 ? arraySize : 10); 
	size_ = arraySize;
	arr_ = new T[allocSize_];
	
	for (size_t i = 0; i < allocSize_; i++)
		arr_[i] = 0;
} 

template <typename T>
Array <T>::Array (const Array & arregloACopiar): allocSize_ (arregloACopiar.allocSize_)
{
	size_t i;

	arr_ = new T[allocSize_];

	size_ = arregloACopiar.size_;

	for (i = 0; i < size_; i++)
		arr_[i] = (arregloACopiar.arr_)[i];

	while (i < allocSize_)
	{
		arr_[i] = 0;
		i++;
	}
} 

template <typename T>
Array <T>::~Array ()
{
	delete [ ] arr_; 
} 

template <typename T>
size_t Array <T>::getAllocSize () const {return allocSize_;}

template <typename T>
size_t Array <T>::getSize () const {return size_;}

template <typename T>
status_t Array <T>::resize ()
{
	T * newArray;

	try {newArray = new T[(this->allocSize_) * GROWTH_FACTOR];}
	
	catch (bad_alloc & exceptionMemory)
	{
		printError (ERROR_EXCEPTION);
		cerr << exceptionMemory.what () << endl;
		return ERROR_MEMORY;
	}

	for (size_t i = 0; i < (this->allocSize_); ++i)
		newArray[i] = (this->arr_)[i];

	delete [] (this->arr_);

	(this->allocSize_) *= GROWTH_FACTOR;

	(this->arr_) = newArray;

	return OK;
}

template <typename T>
const Array<T> & Array <T>::operator = (const Array & rArray)
{
	if (&rArray != this)
	{
		if (allocSize_ != rArray.allocSize_)
		{
			delete [ ] arr_; 
			allocSize_ = rArray.allocSize_;
			size_ = rArray.size_; 
			arr_ = new T[allocSize_]; 
		}

		for (size_t i = 0; i < size_; i++)
			arr_[i] = rArray.arr_[i];  
	
		return *this; 
	}

	return *this;
}

template <typename T>
bool Array <T>::operator == (const Array & rArray) const
{
	if (allocSize_ != rArray.allocSize_)
		return false; 
	
	for (size_t i = 0; i < allocSize_; i++)
		if (arr_[i] != rArray.arr_[i])
			return false;
	
	return true; 
} 

template <typename T>
T & Array <T>::operator [ ] (size_t subIndex)
{
	if (subIndex < 0 || subIndex >= allocSize_)
	{
		printError(ERROR_OUT_OF_RANGE);	 
		cout << " [" << subIndex << "]" << endl;
		exit (1);
	}

	return arr_[subIndex];
}

template <typename T>
istream & operator >> (istream & input, Array <T> & a)
{
	for (size_t i = 0; i < a.allocSize_; i++)
	{		
		while (!(input>>a.arr_[i]))
		{
			input.clear ();
			input.ignore (20,'\n');
			printError (ERROR_VALUE_PROCESS_RESUMED);
		}
	}
	
	return input; 
}

template <typename T>
ostream & operator << (ostream & output, const Array<T> & a)
{
	size_t i;

	for (i = 0; i < a.size_; i++)
		output << a.arr_[i]<< endl;

	return output; 
}

template <typename T>
status_t Array<T>::loadArray ()
{
	T value;
	string str;
	size_ = 0;
	status_t st;

	while (getline (cin,str))
	{
		if (size_ == allocSize_)
		{
			if ((st = resize ()))
			{
				delete [] (this->arr_);
				return st;
			}
		}
		
		istringstream istr (str);
		
		if (!(istr >> value)){
			printError (ERROR_VALUE_PROCESS_RESUMED);
			continue;
		}
		
		arr_[size_++] = value;
	}

	return OK;
}

template <typename T>
status_t Array<T>::loadArray (ifstream & InputFile)
{
	T value;
	string str;
	size_ = 0;
	status_t st;

	while (getline (InputFile,str))
	{
		if (size_ == allocSize_)
		{
			if ((st = resize ()))
			{
				delete [] (this->arr_);
				return st;
			}	
		}

		istringstream istr (str);

		if (!(istr >> value)){
			printError (ERROR_VALUE_PROCESS_ABORTED);
			delete [] (this->arr_);
			exit(1);
		}
		
		arr_[size_++] = value;
	}

	return OK;
}

template <typename T>
status_t Array<T>::addItem (T item)
{
	status_t st;

	if (size_ == allocSize_)
	{
		if ((st = resize ()))
		{
			delete [] (arr_);
			return st;
		}
	}

	arr_[size_++] = item;

	return OK;
}

#endif
