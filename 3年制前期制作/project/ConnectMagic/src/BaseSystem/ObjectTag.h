#pragma once
#include <initializer_list>

// オブジェクト識別用のタグ
enum class ETag
{
	eNone = -1,

	eField,	// フィールドを構成するオブジェクト
	eRideableObject,	// 乗ることができるオブジェクト
	eInteractObject,	// 調べるオブジェクト
	eConnectObject,		// 繋げれるオブジェクト
	eItem,				// アイテムのオブジェクト
	eObstruction,		// 障害物
	eSwitch,			// スイッチ
	eSwitchObject,		// スイッチで作用するオブジェクト

	ePlayer,	// プレイヤー
	eEnemy,		// エネミー

	eBullet,// 弾丸
	eFlame,	// 炎
	eSlash,	// 斬撃
	eWand,	// 杖
	eConnectPoint,	// 接続部

	eUI,
	eInteractUI, //インタラクトUI

	eCamera,// カメラ

	eDebug,	// デバッグ用のオブジェクト

	Num
};
// オブジェクト識別用のタグの初期化リスト
using Tags = std::initializer_list<ETag>;
