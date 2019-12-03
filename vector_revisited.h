#pragma once

#include<iostream>
#include<initializer_list>
#include<cassert>

template<typename T>
class Vector
{
private:
	size_t m_Size{ 0 };
	size_t m_Capacity{ 0 };
	T* m_Arr{ nullptr };

public:
	class Iterator
	{
	public:
		Iterator(T *vector, size_t index);
		const T* operator*() const;
		Iterator& operator++();
		Iterator operator++(int);
		bool operator!=(const Iterator& other) const;
	private:
		
		T *m_Vector;
		size_t m_Index{ 0 };
	};

public:

	Vector() = default;
	Vector(const std::initializer_list<T>&);	//I've added a new constructor with initializer_list
	Vector(const size_t);
	Vector(const Vector&);
	Vector(Vector&&);
	Vector& operator=(const Vector& v);
	Vector& operator=(Vector&& v);
	~Vector()
	{
		if (m_Arr)
			delete[] m_Arr;
	}

	//ostream operator overloading attempt for Vector
	friend std::ostream& operator<<(std::ostream& os, const Vector<T>& v)
	{
		v.printVector();
		return os;
	}

	size_t size() const;
	size_t capacity() const;
	
	T& front();					//additional methods
	const T& front() const;		//additional methods
	T& back();					//additional methods
	const T& back() const;		//additional methods
	
	void push_back(const T& num);
	void pop_back(T& num);


	T& operator[](const int);
	T* operator[](const int) const;

	void printVector() const;

	Iterator begin() const;
	Iterator end() const;

};

template<typename T>
Vector<T>::Vector(const size_t capacity) :m_Size{ 0 }, m_Capacity{ capacity }, m_Arr{ new T[m_Capacity] {T()} }{}

template<typename T>
Vector<T>::Vector(const std::initializer_list<T>& list) : m_Size{ list.size() }, m_Capacity{ list.size() }, m_Arr{ new T[m_Capacity] {T()} }
{
	size_t j{ 0 };
		for (const auto& i: list)
		{
			m_Arr[j++] = i;
		}
}

template<typename T>
Vector<T>::Vector(const Vector<T>& v) : m_Size{ v.mSize }, m_Capacity{ v.m_Capacity }, m_Arr{ new T[m_Capacity]{T()} }
{
	for (int i{ 0 }; i < m_Size; ++i)
	{
		m_Arr[i] = v.m_Arr[i];
	}
}

template<typename T>
Vector<T>::Vector(Vector<T>&& v) :m_Size{ v.m_Size }, m_Capacity{ v.m_Capacity }, m_Arr{ nullptr }
{
	std::swap(m_Arr, v.m_Arr);
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& v)
{
	if (this != &v)
	{
		std::swap(m_Size = v.m_Size);
		std::swap(m_Capacity = v.m_Capacity);
		std::swap(m_Arr = v.m_Arr);
	}
	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
	if (this != &v)
	{
		m_Size = v.m_Size;
		m_Capacity = v.m_Capacity;
		delete[] m_Arr;
		m_Arr = new T[m_Capacity];
		for (int i{ 0 }; i < m_Size; ++i)
		{
			m_Arr[i] = v.m_Arr[i];
		}
	}
	return *this;
}

template<typename T>
size_t Vector<T>::size() const
{
	return m_Size;
}

template<typename T>
size_t Vector<T>::capacity() const
{
	return m_Capacity;
}

template<typename T>
T& Vector<T>::front()
{
	return m_Arr[0];
}

template<typename T>
const T& Vector<T>::front() const
{
	return m_Arr[0];
}

template<typename T>
T& Vector<T>::back()
{
	return m_Arr[m_Size - 1];
}

template<typename T>
const T& Vector<T>::back() const 
{
	return m_Arr[m_Size - 1];
}

template<typename T>
void Vector<T>::push_back(const T& num)
{
	if (m_Capacity <= m_Size)
	{
		m_Capacity += 10;
		//I thought enclosing memory allocation in try-block would help.
		//I'm not sure this is the proper solution.
		try
		{
			T* newArr = new T[m_Capacity]{ T() };
			for (size_t i{ 0 }; i < m_Size; ++i)
			{
				newArr[i] = m_Arr[i];
			}
			delete[]m_Arr;
			m_Arr = newArr;
		}
		catch (const std::bad_alloc& e)
		{
			std::cout << e.what() << std::endl;
		}
		
	}
	m_Arr[m_Size++] = num;
}

template<typename T>
void Vector<T>::printVector() const
{
	if (m_Arr)
	{
		for (size_t i{ 0 }; i < m_Size; ++i)
		{
			std::cout << m_Arr[i] << ' ';
		}
		std::cout << std::endl;
	}
}

template<typename T>
void Vector<T>::pop_back(T& num)
{
	num = m_Arr[m_Size - 1];
	m_Arr[m_Size - 1] = T();
	m_Size--;
}


template<typename T>
T& Vector<T>::operator[](const int i)
{
	assert(i >= 0 && i < m_Size);
	return m_Arr[i];
}

template<typename T>
T* Vector<T>::operator[](const int i) const
{
	assert(i >= 0 && i < m_Size);
	return m_Arr + i;
}

template <typename T>
Vector<T>::Iterator::Iterator(T* vector, size_t index) : m_Vector(vector), m_Index(index){}

template <typename T>
const T* Vector<T>::Iterator::operator*() const
{
	return m_Vector + m_Index;
}

template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
{
	++m_Index;
	return *this;
	//return Vector<T>::Iterator{ this, m_Index };	
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
{
	int temp = m_Index++;
	return Vector<T>::Iterator{ this, temp };
}

template <typename T>
//This portion of != operator code results in errors during compilation in VStudio. However I get no error when compiling 
//with g++ in Ubuntu VM. Also, I don't understand how I should rewrite it to cover your comment:
//"It will return true for iterators pointing at same index of two *different* vectors!"
bool Vector<T>::Iterator::operator!=(typename const Vector<T>::Iterator& other) const
{
	return m_Index != other.m_Index;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin() const
{
	return Vector<T>::Iterator{ this->m_Arr, 0 };
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end() const
{
	return Vector<T>::Iterator{ this->m_Arr, m_Size };
}