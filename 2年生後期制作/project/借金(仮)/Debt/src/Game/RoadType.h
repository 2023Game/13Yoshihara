#pragma once

// どの道にいる状態か
enum class ERoadType
{
	None = -1,

	eLeft1,		// 左から一番目の道
	eLeft2,		// 左から二番目の道
	eRight1,	// 右から一番目の道
	eRight2,	// 右から二番目の道

};