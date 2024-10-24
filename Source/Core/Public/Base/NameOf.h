#pragma once

#include "Base/BasicTypes.h"
#include "ThirdParty/nameof/nameof.hpp"

namespace hg
{

template <typename E>
[[nodiscard]] constexpr int32 EnumCount() noexcept
{
	return nameof::detail::count_v<std::decay_t<E>>;
}

template <typename E>
[[nodiscard]] constexpr auto EnumName(E value) noexcept
{
	return nameof::nameof_enum(value);
}

}