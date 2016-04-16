#include "../../header/n358_dropMake/n358_070_dropMakerAbstract.hpp"
#include "../../header/n358_dropMake/n358_100_dropMakerHand0.hpp"
#include "../../header/n358_dropMake/n358_110_dropMakerHand1.hpp"
#include "../../header/n358_dropMake/n358_120_dropMakerHand2.hpp"
#include "../../header/n358_dropMake/n358_130_dropMakerHand3.hpp"
#include "../../header/n358_dropMake/n358_140_dropMakerHand4.hpp"
#include "../../header/n358_dropMake/n358_150_dropMakerHand5.hpp"
#include "../../header/n358_dropMake/n358_160_dropMakerHand6.hpp"
#include "../../header/n358_dropMake/n358_500_dropMakerArray.hpp"

const DropMakerAbstract DropMakerArray::m_dropMakerArray[] = {
	DropMakerHand0(),
	DropMakerHand1(),
	DropMakerHand2(),
	DropMakerHand3(),
	DropMakerHand4(),
	DropMakerHand5(),
	DropMakerHand6()
};
