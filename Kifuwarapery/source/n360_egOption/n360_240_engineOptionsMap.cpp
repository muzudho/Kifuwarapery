#include <string>
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"

#include "../../header/n900_main____/n900_200_searcher.hpp"		//TODO:1 これを外すのがむずかしい。
//class Searcher;


bool CaseInsensitiveLess::operator () (const std::string& s1, const std::string& s2) const {
	for (size_t i = 0; i < s1.size() && i < s2.size(); ++i) {
		const int c1 = tolower(s1[i]);
		const int c2 = tolower(s2[i]);

		if (c1 != c2) {
			return c1 < c2;
		}
	}
	return s1.size() < s2.size();
}


void EngineOptionsMap::Put(const std::string key, EngineOptionable value)
{
	(*this)[key] = value;
}
