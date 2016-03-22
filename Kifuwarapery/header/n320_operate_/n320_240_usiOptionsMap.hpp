#pragma once


#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n320_operate_/n320_230_usiOptionable.hpp"
#include "../../header/n320_operate_/n320_240_usiOptionsMap.hpp"


struct OptionsMap;


struct CaseInsensitiveLess {
	bool operator() (const std::string&, const std::string&) const;
};


struct OptionsMap : public std::map<std::string, UsiOptionable, CaseInsensitiveLess> {
public:
	void initOptions(Searcher* s);
	bool isLegalOption(const std::string name) {
		return this->find(name) != std::end(*this);
	}
};
