/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "../Base/BasicTypes.h"

namespace hg
{

template<typename T, uint32 N>
class Array
{
public:
	Array() = default;
	Array(const Array& other) = default;
	Array& operator=(const Array& other) = default;
	Array(Array&& other) = default;
	Array& operator=(Array&& other) = default;
	~Array() = default;

	T* data() { return m_data[0]; }
	const T* data() const { return m_data[0]; }
	T* begin() { return m_data[0]; }
	const T* begin() const { return m_data[0]; }
	T* end() { return m_data[N]; }
	const T* end() const { return m_data[N]; }
	uint32 size() const { return N; }
	bool empty() const { return false; }
	T& operator[](uint32 pos) { return m_data[pos]; }
	const T& operator[](uint32 pos) const { return m_data[pos]; }

private:
	T m_data[N];
};

}