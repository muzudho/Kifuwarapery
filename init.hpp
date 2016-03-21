#pragma once

#include "stdafx.h"
#include "header/n080_common__/n080_050_ifdef.hpp"
#include "header/n080_common__/n080_100_common.hpp"
#include "bitboard.hpp"

void initTable();

#if defined FIND_MAGIC
u64 findMagic(const Square sqare, const bool isBishop);
#endif // #if defined FIND_MAGIC

