#pragma once

// Taskの優先順序
enum class ETaskPriority
{
	eNone = -1,

	// 3D関連
	eBackground,// 背景
	eDebugFov,	// 視野範囲のデバッグ表示
	eNavigation,// 経路探索関連のデバッグ表示
	eDefault,	// デフォルト
	ePlayer,	// プレイヤー
	eEnemy,		// 敵
	eTrashBag,	// ゴミ袋
	eWeapon,	// 武器
	eVehicle,	// 車両
	eSpawnZone,	// 生成場所

	eEffect,	// エフェクト
	eBillboard,	// ビルボードイメージ
	eUI3D,		// 3D空間に配置するUI

	eCamera,	// カメラ

	// 2D関連
	eUI,		// UI関連のデフォルト
	eFade,		// 画面フェード
	eSystem,	// システム関連
	eDebug,		// デバッグ関連

	Num,

	// 2D関連のタスクの開始位置
	Start2d = eUI,
};