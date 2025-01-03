/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "../STL/List.h"
#include "../Delegates/Delegate.hpp"

#define DECLARE_MULTICAST_DELEGATE(name, ...) \
using name = hg::MulticastDelegate<void(__VA_ARGS__)>;

namespace hg
{

template<typename T>
class MulticastDelegate
{
private:
	static_assert("MulticastDelegate class should accept more than one parameter. Void should also pass through.");
};

template<typename RetVal, typename... Args>
class MulticastDelegate<RetVal(Args...)>
{
public:
	using DelegateType = Delegate<RetVal(Args...)>;

public:
	MulticastDelegate() = default;
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate& operator=(const MulticastDelegate&) = delete;
	MulticastDelegate(MulticastDelegate&&) = default;
	MulticastDelegate& operator=(MulticastDelegate&&) = default;
	~MulticastDelegate() = default;

	template<RetVal(*Function)(Args...)>
	void Bind()
	{
		DelegateType delegate;
		delegate.template Bind<Function>();
		m_delegates.emplace_back(std::move(delegate));
	}

	template<typename C, RetVal(C::*Function)(Args...)>
	void Bind(C* pInstance)
	{
		DelegateType delegate;
		delegate.template Bind<C, Function>(pInstance);
		m_delegates.emplace_back(std::move(delegate));
	}

	template<typename C, RetVal(C::*Function)(Args...) const>
	void Bind(const C* pInstance)
	{
		DelegateType delegate;
		delegate.template Bind<C, Function>(pInstance);
		m_delegates.emplace_back(std::move(delegate));
	}

	void Clear()
	{
		m_delegates.clear();
	}

	bool Empty() const { return m_delegates.empty(); }

	void Invoke(Args... args) const
	{
		for (const auto& delegate : m_delegates)
		{
			delegate.Invoke(args...);
		}
	}

private:
	List<DelegateType> m_delegates;
};

template<typename... Args>
using Event = MulticastDelegate<void(Args...)>;

}