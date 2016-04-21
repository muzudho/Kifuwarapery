#pragma once

#include "../n119_score___/n119_090_scoreIndex.hpp"	//Ply,ScoreIndex
#include "../n119_score___/n119_500_EvalSum.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n223_move____/n223_200_depth.hpp"

struct SplitedNode;// Flashlight（ＳｅａｒｃｈＳｔａｃｋ）,NextmoveEvent(ＭｏｖｅＰｉｃｋｅｒ),ＳｐｌｉｔＰｏｉｎｔ が三つ巴になっている☆（＾ｑ＾）


// 元の名前： ＳｅａｒｃｈＳｔａｃｋ
// スタックに積み上げていくもの。
// とりあえず、フラッシュライト（懐中電灯）に改名。
class Flashlight {
public:

	// 分岐ノードか☆
	SplitedNode*	m_splitedNode;

	// 手数か☆？
	Ply				m_ply;

	// 現在の指し手か、予想手か☆？
	Move			m_currentMove;

	// todo: これは必要？
	// トランスポジション・テーブルと関連☆？
	Move			m_excludedMove;

	// [0]新しいベストムーブ☆？
	// [1]１個前のベストムーブ☆？（１個前を覚えている☆？）
	Move			m_killers[2];

	// 削減とは☆？（＾ｑ＾）？
	Depth			m_reduction;

	// 静的な評価値とは☆？
	ScoreIndex			m_staticEval;

	// 一時的にヌル・ムーブをせずに検索させたいときのために設定するもの☆
	bool			m_skipNullMove;

	EvalSum			m_staticEvalRaw;	// 評価関数の差分計算用、値が入っていないときは [0] を ScoreNotEvaluated にしておく。
									// 常に Black 側から見た評価値を入れておく。
									// 0: 双玉に対する評価値, 1: 先手玉に対する評価値, 2: 後手玉に対する評価値
};
