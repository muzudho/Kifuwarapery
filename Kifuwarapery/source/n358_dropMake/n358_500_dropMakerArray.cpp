#include "../../header/n358_dropMake/n358_070_dropMakerAbstract.hpp"
#include "../../header/n358_dropMake/n358_100_dropMakerHand0.hpp"
#include "../../header/n358_dropMake/n358_110_dropMakerHand1.hpp"
#include "../../header/n358_dropMake/n358_120_dropMakerHand2.hpp"
#include "../../header/n358_dropMake/n358_130_dropMakerHand3.hpp"
#include "../../header/n358_dropMake/n358_140_dropMakerHand4.hpp"
#include "../../header/n358_dropMake/n358_150_dropMakerHand5.hpp"
#include "../../header/n358_dropMake/n358_160_dropMakerHand6.hpp"
#include "../../header/n358_dropMake/n358_500_dropMakerArray.hpp"


const DropMakerHand0 DropMakerArray::m_dropMakerHand0;// = DropMakerHand0();
const DropMakerHand1 DropMakerArray::m_dropMakerHand1;// = DropMakerHand1();
const DropMakerHand2 DropMakerArray::m_dropMakerHand2;// = DropMakerHand2();
const DropMakerHand3 DropMakerArray::m_dropMakerHand3;// = DropMakerHand3();
const DropMakerHand4 DropMakerArray::m_dropMakerHand4;// = DropMakerHand4();
const DropMakerHand5 DropMakerArray::m_dropMakerHand5;// = DropMakerHand5();
const DropMakerHand6 DropMakerArray::m_dropMakerHand6;// = DropMakerHand6();

// TODO: ポインタの配列にするのは正解のはず☆（＾ｑ＾）他にはテンプレートはずしも考えないと☆
const DropMakerAbstract* DropMakerArray::m_dropMakerArray[] = {
	&DropMakerArray::m_dropMakerHand0,
	&DropMakerArray::m_dropMakerHand1,
	&DropMakerArray::m_dropMakerHand2,
	&DropMakerArray::m_dropMakerHand3,
	&DropMakerArray::m_dropMakerHand4,
	&DropMakerArray::m_dropMakerHand5,
	&DropMakerArray::m_dropMakerHand6
};
