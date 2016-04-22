#include "../../header/n162_bonaDir_/n162_070_bonaDirAbstract.hpp"
#include "../../header/n351_bonaDir_/n351_090_nullBonaDir.hpp"
#include "../../header/n351_bonaDir_/n351_100_direcMisc.hpp"
#include "../../header/n351_bonaDir_/n351_120_direcFile.hpp"
#include "../../header/n351_bonaDir_/n351_130_direcRank.hpp"
#include "../../header/n351_bonaDir_/n351_140_direcDiagNESW.hpp"
#include "../../header/n351_bonaDir_/n351_150_direcDiagNWSE.hpp"
#include "../../header/n351_bonaDir_/n351_500_bonaDirArray.hpp"


BonaDirAbstract* g_bonaDirArray[6] = {
	&g_direcMisc,
	&g_nullBonaDir,
	&g_direcFile,
	&g_direcRank,
	&g_direcDiagNESW,
	&g_direcDiagNWSE
};
