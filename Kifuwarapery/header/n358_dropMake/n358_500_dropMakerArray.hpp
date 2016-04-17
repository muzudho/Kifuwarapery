#pragma once


#include "n358_070_dropMakerAbstract.hpp"
#include "n358_100_dropMakerHand0.hpp"
#include "n358_110_dropMakerHand1.hpp"
#include "n358_120_dropMakerHand2.hpp"
#include "n358_130_dropMakerHand3.hpp"
#include "n358_140_dropMakerHand4.hpp"
#include "n358_150_dropMakerHand5.hpp"
#include "n358_160_dropMakerHand6.hpp"


class DropMakerArray {
public:

	static const DropMakerHand0 m_dropMakerHand0;
	static const DropMakerHand1 m_dropMakerHand1;
	static const DropMakerHand2 m_dropMakerHand2;
	static const DropMakerHand3 m_dropMakerHand3;
	static const DropMakerHand4 m_dropMakerHand4;
	static const DropMakerHand5 m_dropMakerHand5;
	static const DropMakerHand6 m_dropMakerHand6;

	static const DropMakerAbstract* m_dropMakerArray[7];

};
