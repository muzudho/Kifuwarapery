#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n280_move____/n280_100_generateMoves.hpp"
#include "../../header/n280_move____/n280_150_movePicker.hpp"
#include "../../header/n320_operate_/n320_100_book.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n320_operate_/n320_240_usiOptionsMap.hpp"
#include "../../header/n320_operate_/n320_245_usiOption.hpp"



USIOption::USIOption(const char* v, Fn* f, Searcher* s) : UsiOptionable(v, f, s)
{
}

USIOption::USIOption(const bool v, Fn* f, Searcher* s) : UsiOptionable(v, f, s)
{
}

USIOption::USIOption(Fn* f, Searcher* s) : UsiOptionable(f, s)
{
}

USIOption::USIOption(const int v, const int min, const int max, Fn* f, Searcher* s) : UsiOptionable(v, min, max, f, s)
{
}


std::ostream& operator << (std::ostream& os, const OptionsMap& om) {
	for (auto& elem : om) {
		const UsiOptionable& o = elem.second;
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

