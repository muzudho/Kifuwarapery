#pragma once


class Position;
struct SplitPoint;


class SearchStack {
public:
	SplitPoint* splitPoint;
	Ply ply;
	Move currentMove;
	Move excludedMove; // todo: ����͕K�v�H
	Move killers[2];
	Depth reduction;
	Score staticEval;
	bool skipNullMove;
	EvalSum staticEvalRaw; // �]���֐��̍����v�Z�p�A�l�������Ă��Ȃ��Ƃ��� [0] �� ScoreNotEvaluated �ɂ��Ă����B
						   // ��� Black �����猩���]���l�����Ă����B
						   // 0: �o�ʂɑ΂���]���l, 1: ���ʂɑ΂���]���l, 2: ���ʂɑ΂���]���l
};
