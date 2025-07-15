#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属していない

	eTitle,			// タイトルシーン
	eMap1,			// マップ1シーン
	eMap2,			// マップ2シーン
	eMap3,			// マップ3シーン

	eBootMenu,	// 起動時メニュー
	eColTest,	// 衝突判定テスト
	eGameTest,	// ゲームテスト
};
