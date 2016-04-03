#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_500_move.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n276_genMove_/n276_250_makePromoteMove.hpp"
#include "../../header/n280_move____/n280_150_movePicker.hpp"
#include "../../header/n300_book____/n300_100_book.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"



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
