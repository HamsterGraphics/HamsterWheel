/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#if defined(__clang__)
#define HG_COMPILER_CLANG
#elif defined(_MSC_VER)
#define HG_COMPILER_MSC
#elif defined(__GNUC__)
#define HG_COMPILER_GCC
#endif

#define UNUSED(a) ((void)(a))
#define INLINE inline
#define NODISCARD [[nodiscard]]
#define THREAD_LOCAL __declspec(thread)