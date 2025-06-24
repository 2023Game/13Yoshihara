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
	
	ePlayer,		// プレイヤー
	eEnemy,			// 敵
	
	eCharaSearch,	// キャラクター探知用
	eSpellSearch,	// 呪文探知用
	eSearch,		// 衝突レイヤーのオブジェクト探知用

	eAttackCol,		// 攻撃判定
	eDefenseCol,	// 防御判定
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
