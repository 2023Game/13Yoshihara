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
	eItem,			// アイテム
	
	eInteractObj,	// 調べるオブジェクト
	eInteractSearch,// 調べるオブジェクト探知用
	
	eTrashBag,		// ゴミ袋
	eTrashBagSearch,// ゴミ袋探知用

	eObstruction,		// 障害物

	eGoal,			// ゴール
	
	ePlayer,		// プレイヤー
	eEnemy,			// 敵
	eCollector,		// 回収員
	eResident,		// 住人
	
	eCharaSearch,	// キャラクター探知用
	
	eVehicle,		// 車両
	eVehicleSearch,	// 車両の探知用

	eSearch,		// 衝突レイヤーのオブジェクト探知用

	eAttackCol,		// 攻撃判定
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
