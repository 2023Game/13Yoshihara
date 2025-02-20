#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属していない

	eTitle,			// タイトルシーン
	eResult,		// リザルトシーン
	eGameOver,		// ゲームオーバーシーン
	eGameClear,		// ゲームクリアシーン

	eHome,			// 拠点シーン

	eTrashGame,		// ゴミ収集ゲームシーン
	eDeliveryGame,	// 配達ゲームシーン

	eBootMenu,	// 起動時メニュー
	eColTest,	// 衝突判定テスト
};
