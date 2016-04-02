#pragma once

#include "../../header/n220_position/n220_200_changedListPair.hpp"

struct ChangedLists {
	ChangedListPair clistpair[2]; // 一手で動く駒は最大2つ。(動く駒、取られる駒)
	int listindex[2]; // 一手で動く駒は最大2つ。(動く駒、取られる駒)
	size_t size;
};

