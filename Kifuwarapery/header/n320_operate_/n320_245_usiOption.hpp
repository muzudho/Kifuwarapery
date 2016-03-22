#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n320_operate_/n320_230_usiOptionable.hpp"
#include "../../header/n320_operate_/n320_240_usiOptionsMap.hpp"




class USIOption : public UsiOptionable {
public:
	USIOption(Fn* = nullptr, Searcher* s = nullptr);
	USIOption(const char* v, Fn* = nullptr, Searcher* s = nullptr);
	USIOption(const bool  v, Fn* = nullptr, Searcher* s = nullptr);
	USIOption(const int   v, const int min, const int max, Fn* = nullptr, Searcher* s = nullptr);

private:
	friend std::ostream& operator << (std::ostream&, const OptionsMap&);
};
