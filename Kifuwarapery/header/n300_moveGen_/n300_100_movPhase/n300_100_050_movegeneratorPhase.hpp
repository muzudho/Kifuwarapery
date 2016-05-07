#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


// �w���萶���t�F�[�Y���H�i�O���O�j320_540_�l�������o�����������Ƃ��Ŏg����
// ���̃t�F�[�Y�͏��ԂɎ��s����邺���i�O���O�j
enum GenerateMovePhase {
	N00_MainSearch,// �ŏ��̃t�F�[�Y��
	N01_PH_TacticalMoves0,
	N02_PH_Killers,
	N03_PH_NonTacticalMoves0,
	N04_PH_NonTacticalMoves1,
	N05_PH_BadCaptures,
	N06_EvasionSearch,// �������Ŏg���邱�Ƃ����邺��
	N07_PH_Evasions,
	N08_QSearch,// �������Ŏg���邱�Ƃ����邺��
	N09_PH_QCaptures0,
	N10_QEvasionSearch,// �������Ŏg���邱�Ƃ����邺��
	N11_PH_QEvasions,
	N12_ProbCut,// �������Ŏg���邱�Ƃ����邺��
	N13_PH_TacticalMoves1,
	N14_QRecapture,// �������Ŏg���邱�Ƃ����邺��
	N15_PH_QCaptures1,
	N16_PH_Stop// �ԕ����i�O���O�j�������~�߂遙
	//GenerateMovePhaseNum	//(^q^)�ǉ���
};
OverloadEnumOperators(GenerateMovePhase); // ++phase_ �ׁ̈B
static const int g_MOVEGENERATOR_PHASE_NUM = 17;