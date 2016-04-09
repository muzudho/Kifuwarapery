#pragma once


#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n320_searcher/n320_550_searcher.hpp"


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
		assert(m_type_ == "check" || m_type_ == "spin");
		return (m_type_ == "spin" ? atoi(m_currentValue_.c_str()) : m_currentValue_ == "true");
	}

	operator std::string() const {
		assert(m_type_ == "string");
		return m_currentValue_;
	}

public:
	std::string	GetDefaultValue()	const { return this->m_defaultValue_; }
	std::string	GetType()			const { return this->m_type_; }
	int			GetMin()			const { return this->m_min_; }
	int			GetMax()			const { return this->m_max_; }

protected:
	std::string m_defaultValue_;
	std::string m_currentValue_;
	std::string m_type_;
	int m_min_;
	int m_max_;
	Fn* m_onChange_;
	Searcher* m_searcher_;
};
