/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "../Base/Assert.h"

#include <atomic>
#include <format>
#include <iostream>
#include <memory>

namespace hg
{

template<typename T>
class STLAllocator : public std::allocator<T> 
{
public:
    T* allocate(const size_t n)
    {
        allocated_size() += n;
        std::cout << std::format("[STL Allocator] Allocate {} bytes, Totally {} bytes.", n, allocated_size()).data() << std::endl;
        return std::allocator<T>::allocate(n);
    }

    void deallocate(T* const ptr, const size_t n)
    {
        Assert(allocated_size() >= n);
        allocated_size() -= n;
        std::cout << std::format("[STL Allocator] Deallocate {} bytes, Totally {} bytes.", n, allocated_size()).data() << std::endl;
        std::allocator<T>::deallocate(ptr, n);
    }

    size_t& allocated_size()
    {
        static size_t s_allocatedBytes;
        return s_allocatedBytes;
    }
};

}