#ifndef __MY_ARRAY__
#define __MY_ARRAY__

#include <iostream>
#include <cassert>

class EmptyArray: std::runtime_error{
	public:
		EmptyArray(const std::string err): std::runtime_error{err}
		{}
};

template<typename T>
class ArrayVector{
	public:
		ArrayVector();
		ArrayVector(const ArrayVector<T>&);
		ArrayVector(ArrayVector<T>&&);
		ArrayVector& operator=(const ArrayVector<T>&);
		ArrayVector& operator=(ArrayVector<T>&&);
		~ArrayVector();

		size_t size() const { return n; };
		bool empty() const { return n==0; };
		T& operator[](size_t);
		T& at(size_t);
		void erase(size_t);
		void insert(size_t, const T&);
		void reserve(size_t);
		size_t capacity() const { return cap; };

	private:
		size_t cap;
		size_t n;
		T* data;
};

template<typename T>
ArrayVector<T>::ArrayVector():cap{0}, n{0}, data{nullptr}
{}

template<typename T>
ArrayVector<T>::ArrayVector(const ArrayVector<T>& a)
	:cap{a.cap}, n{a.n}, data{a.data}{}

template<typename T>
ArrayVector<T>::ArrayVector(ArrayVector<T>&& a)
	:cap{a.cap}, n{a.n}, data{a.data}
{
	a.data = nullptr;
	a.cap = 0;
	a.n = 0;
}

template<typename T>
ArrayVector<T>& ArrayVector<T>::operator=(const ArrayVector<T>& a)
{
	assert(cap == a.cap);
	assert(n == a.n);
	delete[] data;
	data = a.data;
	return *this;
}

template<typename T>
ArrayVector<T>& ArrayVector<T>::operator=(ArrayVector<T>&& a)
{
	cap = a.cap;
	n = a.n;
	delete[] data;
	data = a.data;
	a.cap = 0;
	n = 0;
	a.data = nullptr;
	return *this;
}

template<typename T>
ArrayVector<T>::~ArrayVector()
{
	delete[] data;
}

template<typename T>
T& ArrayVector<T>::operator[](size_t i)
{
	return data[i];
}

template<typename T>
T& ArrayVector<T>::at(size_t i)
{
	if(i>=n) throw EmptyArray("at: empty array!!!");
	return data[i];
}

template<typename T>
void ArrayVector<T>::erase(size_t i)
{
	if(i >= n) return;
	if( i == (n-1)) data[i] = 0;
	for (size_t j=i; j<n; ++j)
		data[j] = data[j+1];
	--n;
}

template<typename T>
void ArrayVector<T>::insert(size_t i, const T& e)
{
	if(n==cap) reserve(cap*2); // reserve(max(1, cap*2));
	data[i] = e;
	++n;
}

template<typename T>
void ArrayVector<T>::reserve(size_t sz)
{
	T* new_data = new T[sz];
	for(size_t i=0; i<n; ++i)
		new_data[i] = data[i];
	delete[] data;
	data = new_data;
	new_data = nullptr;
}

#endif // __MY_ARRAY__



