#pragma once


#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n223_move____/n223_500_move.hpp"


class EngineOptionable {
protected:
	using Fn = void(Searcher*, const EngineOptionable&);
public:
	EngineOptionable(												Fn* = nullptr, Searcher* s = nullptr);
	EngineOptionable(const char* v,								Fn* = nullptr, Searcher* s = nullptr);
	EngineOptionable(const bool  v,								Fn* = nullptr, Searcher* s = nullptr);
	EngineOptionable(const int   v, const int min, const int max,	Fn* = nullptr, Searcher* s = nullptr);

	EngineOptionable& operator = (const std::string& v);

	operator int() const {
		assert(type_ == "check" || type_ == "spin");
		return (type_ == "spin" ? atoi(currentValue_.c_str()) : currentValue_ == "true");
	}

	operator std::string() const {
		assert(type_ == "string");
		return currentValue_;
	}

public:
	std::string	GetDefaultValue()	const { return this->defaultValue_; }
	std::string	GetType()			const { return this->type_; }
	int			GetMin()			const { return this->min_; }
	int			GetMax()			const { return this->max_; }

protected:
	std::string defaultValue_;
	std::string currentValue_;
	std::string type_;
	int min_;
	int max_;
	Fn* onChange_;
	Searcher* searcher_;
};
