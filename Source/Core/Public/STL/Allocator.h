/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <iostream>
#include <memory>

namespace hg
{

template<typename T>
class STLAllocator : public std::allocator<T> 
{
public:
    T* allocate(size_t n)
    {
        std::cout << "[STL Allocator] Request to allocate " << n << std::endl;
        return std::allocator<T>::allocate(n);
    }
};

}