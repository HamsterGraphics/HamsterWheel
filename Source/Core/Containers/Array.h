#pragma once

#include "../Base/BasicTypes.h"

#include <array>

namespace hg
{

template<typename T, uint32 N>
using Array = std::array<T, N>;

}