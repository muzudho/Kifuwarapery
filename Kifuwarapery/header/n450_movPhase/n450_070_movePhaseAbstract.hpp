#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"


class MovePhaseAbstract {
public:

	// 旧名：goNextPhase
	// 次のフェーズに遷移します。
	virtual void Do02_ExtendTalon(NextmoveEvent& nmEvent) = 0;

	// 旧名：nextMove の中の switch～case文。
	// カードを決定します。または、カーソルを次のカードへ移動させます。
	virtual bool Do03_PickCard_OrNextCard( Move& pickedCard, NextmoveEvent& nmEvent) const = 0;

};
