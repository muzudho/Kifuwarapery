#pragma once

#include <map>
#include <string>
//#include <utility>	//std::end か☆？
//#include <iostream>	//std::end か☆？
#include "../n680_egOption/n680_230_engineOptionable.hpp"



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
