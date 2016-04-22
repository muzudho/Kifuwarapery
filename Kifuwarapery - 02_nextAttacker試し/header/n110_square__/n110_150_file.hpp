#pragma once

#include "../n110_square__/n110_100_square.hpp"


enum File {
	FileI, FileH, FileG, FileF, FileE, FileD, FileC, FileB, FileA, FileNum,
	FileNoLeftNum = FileD
};
OverloadEnumOperators(File);


extern const File g_squareToFile[SquareNum];

