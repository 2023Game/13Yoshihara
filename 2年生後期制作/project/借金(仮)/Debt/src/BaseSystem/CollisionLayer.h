#pragma once
#include <initializer_list>

// 衝突判定レイヤー
enum class ELayer
{
	eNone = -1,
	eTest,			// テスト用
	eGround,		// 地面
	eWall,			// 壁
	eObject,		// オブジェクト
	eInteractObj,	// 調べるオブジェクト
	eInteractSearch,// 調べるオブジェクト探知用
	ePlayer,		// プレイヤー
	eEnemy,			// 敵
	eVehicle,		// 車両
	eVehicleSearch,	// 車両の探知用
	eSpawnZone,		// 生成場所

	eAttackCol,		// 攻撃判定
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
