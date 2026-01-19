#pragma once
#include <vector>

// 衝突判定レイヤー
enum class ELayer
{
	eNone = -1,
	eTest,			// テスト用
	eField,			// 地形
	eObject,		// オブジェクト
	ePortal,		// ポータル
	eCrushed,		// クラッシュさせられるオブジェクト
	eShield,		// シールド
	eSensor,		// センサー

	eConnectObj,	// 接続できるオブジェクト
	eConnectSearch,	// 接続できるオブジェクト探知用

	eSwitch,		// スイッチ
	
	ePlayer,		// プレイヤー
	eEnemy,			// 敵
	
	eCharaSearch,	// キャラクター探知用

	eSearch,		// 衝突レイヤーのオブジェクト探知用

	eAttackCol,		// 攻撃判定
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::vector<ELayer>;
