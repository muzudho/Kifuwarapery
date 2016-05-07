#include "../../header/n080_common__/n080_085_mutex.hpp"
#include "../../header/n080_common__/n080_090_syncCout.hpp"


std::ostream& operator << (std::ostream& os, SyncCout sc) {
	static Mutex mutex;
	if (sc == IOLock) { mutex.lock(); }
	else if (sc == IOUnlock) { mutex.unlock(); }
	return os;
}
