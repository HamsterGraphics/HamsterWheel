#pragma once

#include "../Base/BasicTypes.h"

#include <unordered_map>

namespace hg
{

template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;

}