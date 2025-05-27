#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属していない

	eTitle,			// タイトルシーン
	eTutorial,		// チュートリアルシーン
	eMap1,			// マップ1シーン

	eBootMenu,	// 起動時メニュー
	eColTest,	// 衝突判定テスト
	eGameTest,	// ゲームテスト
};
