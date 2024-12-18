#pragma once
#include <initializer_list>

// オブジェクト識別用のタグ
enum class ETag
{
	eNone = -1,

	eField,	// フィールドを構成するオブジェクト
	eRideableObject,	// 乗ることができるオブジェクト
	eItem,	// アイテムのオブジェクト

	ePlayer,// プレイヤー
	eEnemy,	// エネミー
	eCar,	// 車
	eGarbageTruck, // ゴミ収集車
	eSpawnZone,		// 生成場所

	eTrashBag, // ゴミ袋
	eBullet,// 弾丸
	eFlame,	// 炎
	eSlash,	// 斬撃

	eInteractUI, //インタラクト
	eUI,

	eCamera,// カメラ

	Num
};
// オブジェクト識別用のタグの初期化リスト
using Tags = std::initializer_list<ETag>;
