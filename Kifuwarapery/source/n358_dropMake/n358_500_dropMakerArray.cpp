#include "../../header/n358_dropMake/n358_070_dropMakerAbstract.hpp"
#include "../../header/n358_dropMake/n358_100_dropMakerHand0.hpp"
#include "../../header/n358_dropMake/n358_110_dropMakerHand1.hpp"
#include "../../header/n358_dropMake/n358_120_dropMakerHand2.hpp"
#include "../../header/n358_dropMake/n358_130_dropMakerHand3.hpp"
#include "../../header/n358_dropMake/n358_140_dropMakerHand4.hpp"
#include "../../header/n358_dropMake/n358_150_dropMakerHand5.hpp"
#include "../../header/n358_dropMake/n358_160_dropMakerHand6.hpp"
#include "../../header/n358_dropMake/n358_500_dropMakerArray.hpp"


DropMakerAbstract* g_dropMakerArray[7] = {
	&g_dropMakerHand0,
	&g_dropMakerHand1,
	&g_dropMakerHand2,
	&g_dropMakerHand3,
	&g_dropMakerHand4,
	&g_dropMakerHand5,
	&g_dropMakerHand6
};
