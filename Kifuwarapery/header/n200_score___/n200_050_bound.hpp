#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


enum Bound {
	BoundNone = 0,
	BoundUpper = Binary< 1>::value, // fail low  �Ő����� score ��������Ȃ��Balpha �ȉ����m��Ƃ����Ӗ��B
	BoundLower = Binary<10>::value, // fail high �Ő����� score ��������Ȃ��Bbeta �ȏオ�m��Ƃ����Ӗ��B
	BoundExact = Binary<11>::value  // alpha �� beta �̊Ԃ� score ������B
};
