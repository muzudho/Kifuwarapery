#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n320_operate_/n320_230_usiOptionable.hpp"
#include "../../header/n320_operate_/n320_240_usiOptionsMap.hpp"



const std::string DefaultStartPositionSFEN = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";




class USIOption : public UsiOptionable {
public:
	USIOption(                                                Fn* = nullptr, Searcher* s = nullptr );
	USIOption(const char* v ,                                 Fn* = nullptr, Searcher* s = nullptr );
	USIOption(const bool  v ,                                 Fn* = nullptr, Searcher* s = nullptr );
	USIOption(const int   v , const int min , const int max , Fn* = nullptr, Searcher* s = nullptr );

private:
	friend std::ostream& operator << (std::ostream&, const OptionsMap&);
};








