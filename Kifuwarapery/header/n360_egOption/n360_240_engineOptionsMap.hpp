#pragma once


#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n223_move____/n223_105_utilMove.hpp"
#include "../../header/n360_egOption/n360_230_engineOptionable.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"


struct EngineOptionsMap;


struct CaseInsensitiveLess {
	bool operator() (const std::string&, const std::string&) const;
};


struct EngineOptionsMap : public std::map<std::string, EngineOptionable, CaseInsensitiveLess> {
public:

	bool IsLegalOption(const std::string name) {
		return this->find(name) != std::end(*this);
	}

	void Put(const std::string key, EngineOptionable value);
};
