#pragma once
#include "CConnectObject.h"

class CBox : public CConnectObject
{
public:
	// コンストラクタ
	CBox(CVector defaultPos, float scaleRatio = 1.0f);
	// デストラクタ
	~CBox();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自分のコライダー</param>
	/// <param name="other">衝突した相手のコライダ―</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;

	// キャラと衝突判定をするコライダー
	CCollider* mpCharaCol;

	// 初期座標
	CVector mDefaultPos;
	// リスポーンする
	bool mIsRespawn;
	// 経過時間
	float mElapsedTime;
};