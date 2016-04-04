#pragma once


class Position;
struct SplitPoint;


class SearchStack {
public:
	SplitPoint*		m_splitPoint;
	Ply				m_ply;
	Move			m_currentMove;
	Move			m_excludedMove; // todo: ����͕K�v�H
	Move			m_killers[2];
	Depth			m_reduction;
	Score			m_staticEval;
	bool			m_skipNullMove;
	EvalSum			m_staticEvalRaw;	// �]���֐��̍����v�Z�p�A�l�������Ă��Ȃ��Ƃ��� [0] �� ScoreNotEvaluated �ɂ��Ă����B
									// ��� Black �����猩���]���l�����Ă����B
									// 0: �o�ʂɑ΂���]���l, 1: ���ʂɑ΂���]���l, 2: ���ʂɑ΂���]���l
};
