#pragma once

#include "../../header/n220_position/n220_200_changedListPair.hpp"

struct ChangedLists {
	ChangedListPair m_clistpair[2]; // ���œ�����͍ő�2�B(������A������)
	int m_listindex[2]; // ���œ�����͍ő�2�B(������A������)
	size_t m_size;
};

