#pragma once
#include <iostream>
template<typename T>
class base_ptr
{
protected:
	T* data_ = nullptr;
public:
	base_ptr() {}
	base_ptr(base_ptr<T>&& other) noexcept : data_(other.data_) { other.data_ = nullptr; }
	base_ptr<T>& operator=(base_ptr<T>&& other) noexcept
	{
		if (this == &other) return *this;
		reset();
		data_ = other.data_;
		other.data_ = nullptr;
		return *this;
	}
	T* release() 
	{
		T* tmp = data_;
		data_ = nullptr;
		return tmp;
	}
	void reset()
	{
		if (data_ != nullptr)
		{
			if (std::is_array_v<T>)
				delete[] data_;
			else
				delete data_;
		}
		data_ = nullptr;
	}
	void reset(T* item) noexcept
	{
		reset();
		data_ = item;
	}
	void swap(base_ptr<T>& other) noexcept
	{
		T* tmp = data_;
		data_ = other.data_;
		other.data_ = tmp;
	}
	operator bool() { return data_; }
	T* get() { return data_; }

};

template <typename T>
class unique_ptr : public base_ptr<T>
{
public:
	unique_ptr() {}
	explicit unique_ptr(T* data)
	{
		base_ptr<T>::data_ = data;
	}
	unique_ptr(unique_ptr<T>&& other) noexcept : base_ptr<T>(std::move(other)) {}
	unique_ptr<T>& operator=(unique_ptr<T>&& other) noexcept
	{
		base_ptr<T>::operator=(std::move(other));
		return *this; 
	}
	T* operator->() const { return base_ptr<T>::ptr; }
	T& operator*() const { return *base_ptr<T>::ptr; }
	~unique_ptr() { base_ptr<T>::reset(); }
};
template <typename T>
class unique_ptr<T[]> : public base_ptr<T[]> {
public:
	unique_ptr() {}
	unique_ptr(unique_ptr<T[]>&& other) : base_ptr<T[]>(std::move(other)) {}
	explicit unique_ptr(T* ptr) 
	{
		base_ptr<T[]>::data_ = ptr;
	}
	unique_ptr<T[]>& operator=(unique_ptr<T[]>&& other) 
	{
		base_ptr<T[]>::operator=(std::move(other));
		return *this;
	}
	T& operator[](int pos) const { return base_ptr<T[]>::data_[pos]; }
	~unique_ptr() { base_ptr<T[]>::reset(); }
};
