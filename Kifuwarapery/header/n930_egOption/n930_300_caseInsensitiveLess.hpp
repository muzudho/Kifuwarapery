#pragma once


#include <string>


struct CaseInsensitiveLess {
	bool operator() (const std::string&, const std::string&) const;
};
