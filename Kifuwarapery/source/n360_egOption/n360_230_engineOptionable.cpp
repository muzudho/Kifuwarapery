#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n280_move____/n280_100_generateMoves.hpp"
#include "../../header/n280_move____/n280_150_movePicker.hpp"
#include "../../header/n320_operate_/n320_100_book.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n360_egOption/n360_230_engineOptionable.hpp"


EngineOptionable::EngineOptionable(const char* v, Fn* f, Searcher* s) :
	type_("string"), min_(0), max_(0), onChange_(f), searcher_(s)
{
	defaultValue_ = currentValue_ = v;
}


EngineOptionable::EngineOptionable(const bool v, Fn* f, Searcher* s) :
	type_("check"), min_(0), max_(0), onChange_(f), searcher_(s)
{
	defaultValue_ = currentValue_ = (v ? "true" : "false");
}


EngineOptionable::EngineOptionable(Fn* f, Searcher* s) :
	type_("button"), min_(0), max_(0), onChange_(f), searcher_(s) {}

EngineOptionable::EngineOptionable(const int v, const int min, const int max, Fn* f, Searcher* s)
	: type_("spin"), min_(min), max_(max), onChange_(f), searcher_(s)
{
	std::ostringstream ss;
	ss << v;
	defaultValue_ = currentValue_ = ss.str();
}


EngineOptionable& EngineOptionable::operator = (const std::string& v) {
	assert(!type_.empty());

	if ((type_ != "button" && v.empty())
		|| (type_ == "check" && v != "true" && v != "false")
		|| (type_ == "spin" && (atoi(v.c_str()) < min_ || max_ < atoi(v.c_str()))))
	{
		return *this;
	}

	if (type_ != "button") {
		currentValue_ = v;
	}

	if (onChange_ != nullptr) {
		(*onChange_)(searcher_, *this);
	}

	return *this;
}