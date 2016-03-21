#pragma once

#include "stdafx.h"
#include "ifdef.hpp"
#include "common.hpp"
#include "bitboard.hpp"

void initTable();

#if defined FIND_MAGIC
u64 findMagic(const Square sqare, const bool isBishop);
#endif // #if defined FIND_MAGIC

