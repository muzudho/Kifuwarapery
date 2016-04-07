#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n223_move____/n223_105_utilMove.hpp"
#include "../../header/n360_egOption/n360_230_engineOptionable.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"




class EngineOption : public EngineOptionable {
public:
	EngineOption(Fn* = nullptr, Searcher* s = nullptr);
	EngineOption(const char* v, Fn* = nullptr, Searcher* s = nullptr);
	EngineOption(const bool  v, Fn* = nullptr, Searcher* s = nullptr);
	EngineOption(const int   v, const int min, const int max, Fn* = nullptr, Searcher* s = nullptr);

private:
	friend std::ostream& operator << (std::ostream&, const EngineOptionsMap&);
};
