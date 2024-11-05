/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Base/Assert.h"
#include "Base/BasicTypes.h"
#include "Base/TypeTraits.h"

namespace hg
{

template<typename T>
class Vector
{
public:
	static constexpr uint32 DefaultCapacity = 16;

public:
	Vector() :
		m_capacity(DefaultCapacity),
		m_size(0)
	{
		m_data = new T[m_capacity];
	}
	explicit Vector(uint32 size) :
		m_capacity(DefaultCapacity)
	{
		while (size > m_capacity)
		{
			m_capacity *= 2;
		}
		m_data = new T[m_capacity];
		m_size = size;
	}
	Vector(const Vector& other) = default;
	Vector& operator=(const Vector& other) = default;
	Vector(Vector&& other) = default;
	Vector& operator=(Vector&& other) = default;
	~Vector()
	{
		delete[] m_data;
	}

	void push_back(T data)
	{
		if (m_size >= m_capacity)
		{
			grow(m_capacity * 2);
		}
		m_data[m_size++] = MoveTemp(data);
	}

	void pop_back()
	{
		if (m_size > 0)
		{
			--m_size;
		}
	}

	void remove(uint32 index)
	{
		Assert(index < m_size && m_size > 0);
		for (uint32 ii = index; ii < m_size - 1; ++ii)
		{
			m_data[ii] = m_data[ii + 1];
		}
		--m_size;
	}

	void reserve(uint32 newSize)
	{
		if (newSize > m_capacity)
		{
			grow(newSize);
		}
	}

	void resize(uint32 newSize)
	{
		reserve(newSize);
		m_size = newSize;
	}

	void clear()
	{
		m_size = 0;
	}

	void grow(uint32 newCapacity)
	{
		m_capacity = newCapacity;

		T* pNewData = new T[m_capacity];
		for (uint32 ii = 0; ii < m_size; ++ii)
		{
			pNewData[ii] = MoveTemp(m_data[ii]);
		}
		delete m_data;
		m_data = pNewData;
	}

	T* data() { return m_data; }
	const T* data() const { return m_data; }
	uint32 size() const { return m_size; }
	uint32 capacity() const { return m_capacity; }
	T* begin() { return m_data; }
	const T* begin() const { return m_data; }
	T* end() { return m_data + size(); }
	const T* end() const { return m_data + size(); }
	bool empty() const { return 0 == m_size; }
	T& operator[](uint32 pos) { return m_data[pos]; }
	const T& operator[](uint32 pos) const { return m_data[pos]; }

private:
	uint32 m_capacity;
	uint32 m_size;
	T* m_data;
};

}