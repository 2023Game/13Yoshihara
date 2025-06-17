#pragma once

// 属性
enum class ESpellElementalType
{
	eFire,		// 炎
	eWind,		// 風
	eEarth,		// 地
	eThunder,	// 雷
	eWater,		// 水

	eNeutral,	// 無属性

	eError,		// エラー用
};

// 形
enum class ESpellShapeType
{
	eBall,		// ボール型
	eBolt,		// ボルト型
	eBreath,	// ブレス型

	eTeleport,	// テレポート
	eShield,	// シールド
	eReflector,	// リフレクター

	eError,		// エラー用
};