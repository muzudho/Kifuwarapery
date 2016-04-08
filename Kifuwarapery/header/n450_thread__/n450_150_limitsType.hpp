#pragma once

// 時間や探索深さの制限を格納する為の構造体
struct LimitsType {

	LimitsType() { memset(this, 0, sizeof(LimitsType)); }

	bool IsUseTimeManagement() const { return !(m_depth | m_nodes | m_moveTime | static_cast<int>(m_infinite)); }

	int m_time[ColorNum];
	int m_increment[ColorNum];
	int m_movesToGo;
	Ply m_depth;
	u32 m_nodes;
	int m_moveTime;
	bool m_infinite;
	bool m_ponder;
};
