#include <ostream>
#include "../../header/n885_searcher/n885_040_rucksack.hpp"
#include "../../header/n930_egOption/n930_400_engineOption.hpp"



EngineOption::EngineOption(const char* v, Fn* f, Rucksack* s) : EngineOptionable(v, f, s)
{
}

EngineOption::EngineOption(const bool v, Fn* f, Rucksack* s) : EngineOptionable(v, f, s)
{
}

EngineOption::EngineOption(Fn* f, Rucksack* s) : EngineOptionable(f, s)
{
}

EngineOption::EngineOption(const int v, const int min, const int max, Fn* f, Rucksack* s) : EngineOptionable(v, min, max, f, s)
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

