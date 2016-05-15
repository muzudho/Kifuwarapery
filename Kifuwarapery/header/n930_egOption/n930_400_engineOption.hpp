#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n930_egOption/n930_200_engineOptionable.hpp"
#include "../../header/n930_egOption/n930_350_engineOptionsMap.hpp"

class Rucksack;



class EngineOption : public EngineOptionable {
public:

	EngineOption(Fn* = nullptr, Rucksack* s = nullptr);

	EngineOption(const char* v, Fn* = nullptr, Rucksack* s = nullptr);

	EngineOption(const bool  v, Fn* = nullptr, Rucksack* s = nullptr);

	EngineOption(const int   v, const int min, const int max, Fn* = nullptr, Rucksack* s = nullptr);

private:
	friend std::ostream& operator << (std::ostream&, const EngineOptionsMap&);
};
