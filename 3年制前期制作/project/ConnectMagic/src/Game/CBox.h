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

	// 箱がスイッチに張り付いているかを設定
	void SetIsAttach(bool enable);
	// 箱がスイッチに張り付いているか
	bool GetIsAttach() const;

private:
	// コライダーを生成
	void CreateCol() override;

	// 初期座標
	CVector mDefaultPos;
	// リスポーンする
	bool mIsRespawn;
	// 経過時間
	float mElapsedTime;

	// 箱がスイッチに張り付いているか
	bool mIsAttach;
};