
#include "../../header/n320_searcher/n320_550_searcher.hpp"			//TODO: これを外すのがむずかしい。
 
#include "../../header/n360_egOption/n360_230_engineOptionable.hpp"	//TODO: これを外すのがむずかしい。


EngineOptionable::EngineOptionable(const char* v, Fn* f, Searcher* s) :
	m_type_("string"), m_min_(0), m_max_(0), m_onChange_(f), m_searcher_(s)
{
	m_defaultValue_ = m_currentValue_ = v;
}


EngineOptionable::EngineOptionable(const bool v, Fn* f, Searcher* s) :
	m_type_("check"), m_min_(0), m_max_(0), m_onChange_(f), m_searcher_(s)
{
	m_defaultValue_ = m_currentValue_ = (v ? "true" : "false");
}


EngineOptionable::EngineOptionable(Fn* f, Searcher* s) :
	m_type_("button"), m_min_(0), m_max_(0), m_onChange_(f), m_searcher_(s) {}

EngineOptionable::EngineOptionable(const int v, const int min, const int max, Fn* f, Searcher* s)
	: m_type_("spin"), m_min_(min), m_max_(max), m_onChange_(f), m_searcher_(s)
{
	std::ostringstream ss;
	ss << v;
	m_defaultValue_ = m_currentValue_ = ss.str();
}


EngineOptionable& EngineOptionable::operator = (const std::string& v) {
	assert(!m_type_.empty());

	if ((m_type_ != "button" && v.empty())
		|| (m_type_ == "check" && v != "true" && v != "false")
		|| (m_type_ == "spin" && (atoi(v.c_str()) < m_min_ || m_max_ < atoi(v.c_str()))))
	{
		return *this;
	}

	if (m_type_ != "button") {
		m_currentValue_ = v;
	}

	if (m_onChange_ != nullptr) {
		(*m_onChange_)(m_searcher_, *this);
	}

	return *this;
}
