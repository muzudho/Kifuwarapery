#pragma once


// 指し手の種類
enum MoveType {
	N00_Capture,            // 駒を取る手。     歩, 飛, 角 の不成は含まない。香の二段目の不成を含まない。
	N01_NonCapture,         // 駒を取らない手。 歩, 飛, 角 の不成は含まない。香の二段目の不成を含まない。
	N02_Drop,               // 駒打ち。 二歩、打ち歩詰めは含まない。
	N03_CapturePlusPro,     // Capture + (歩 の駒を取らない成る手)
	N04_NonCaptureMinusPro, // NonCapture - (歩 の駒を取らない成る手) - (香の三段目への駒を取らない不成)
	N05_Recapture,          // 特定の位置への取り返しの手
	N06_Evasion,            // 王手回避。歩, 飛, 角 の不成はは含まない。
	N07_NonEvasion,         // 王手が掛かっていないときの合法手 (玉の移動による自殺手、pinされている駒の移動による自殺手は回避しない。)
	N08_Legal,              // 王手が掛かっていれば Evasion, そうでないなら NonEvasion を生成し、
							// 玉の自殺手と pin されてる駒の移動による自殺手を排除。(連続王手の千日手は排除しない。)
	N09_LegalAll,           // Legal + 歩, 飛, 角 の不成、香の二段目の不成、香の三段目への駒を取らない不成を生成
	N10_MoveTypeNone
};
