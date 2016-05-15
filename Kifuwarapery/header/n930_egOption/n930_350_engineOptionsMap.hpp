#pragma once


#include <map>
#include "../../header/n930_egOption/n930_200_engineOptionable.hpp"
#include "../../header/n930_egOption/n930_300_caseInsensitiveLess.hpp"
//#include "../../header/n930_egOption/n930_350_engineOptionable.hpp"


struct EngineOptionsMap : public std::map<std::string, EngineOptionable, CaseInsensitiveLess> {
public:

	bool IsLegalOption(const std::string name) {
		return this->find(name) != std::end(*this);
	}

	void Put(const std::string key, EngineOptionable value);
};
