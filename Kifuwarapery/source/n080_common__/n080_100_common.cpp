#include "../../header/n080_common__/n080_100_common.hpp"


// 乱数の種☆？（＾ｑ＾）？
std::mt19937_64 g_randomTimeSeed(std::chrono::system_clock::now().time_since_epoch().count());

