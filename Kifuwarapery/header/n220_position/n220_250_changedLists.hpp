#pragma once

#include "../../header/n220_position/n220_200_changedListPair.hpp"

struct ChangedLists {
	ChangedListPair clistpair[2]; // ���œ�����͍ő�2�B(������A������)
	int listindex[2]; // ���œ�����͍ő�2�B(������A������)
	size_t size;
};

