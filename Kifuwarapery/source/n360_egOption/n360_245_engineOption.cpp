#include <ostream>
#include "../../header/n360_egOption/n360_245_engineOption.hpp"
#include "../../header/n900_main____/n900_200_searcher.hpp"



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

