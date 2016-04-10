#pragma once

#include "../n220_position/n220_200_changedListPair.hpp"

struct ChangedLists {
	ChangedListPair m_clistpair[2]; // 一手で動く駒は最大2つ。(動く駒、取られる駒)
	int m_listindex[2]; // 一手で動く駒は最大2つ。(動く駒、取られる駒)
	size_t m_size;
};

