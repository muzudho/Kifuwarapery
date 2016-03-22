#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n280_move____/n280_100_generateMoves.hpp"
#include "../../header/n280_move____/n280_150_movePicker.hpp"
#include "../../header/n320_operate_/n320_100_book.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"
#include "../../header/n360_egOption/n360_245_engineOption.hpp"



EngineOption::EngineOption(const char* v, Fn* f, Searcher* s) : EngineOptionable(v, f, s)
{
}

EngineOption::EngineOption(const bool v, Fn* f, Searcher* s) : EngineOptionable(v, f, s)
{
}

EngineOption::EngineOption(Fn* f, Searcher* s) : EngineOptionable(f, s)
{
}

EngineOption::EngineOption(const int v, const int min, const int max, Fn* f, Searcher* s) : EngineOptionable(v, min, max, f, s)
{
}


std::ostream& operator << (std::ostream& os, const EngineOptionsMap& om) {
	for (auto& elem : om) {
		const EngineOptionable& o = elem.second;
		os << "\noption name " << elem.first << " type " << o.GetType();
		if (o.GetType() != "button") {
			os << " default " << o.GetDefaultValue();
		}

		if (o.GetType() == "spin") {
			os << " min " << o.GetMin() << " max " << o.GetMax();
		}
	}
	return os;
}

