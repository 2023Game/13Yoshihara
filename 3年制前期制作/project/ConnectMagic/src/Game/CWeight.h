#pragma once
#include "CConnectObject.h"

class CWeight : public CConnectObject
{
public:
	// コンストラクタ
	CWeight(CVector defaultPos, float scaleRatio = 1.0f);
	// デストラクタ
	~CWeight();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自分のコライダー</param>
	/// <param name="other">衝突した相手のコライダ―</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;

	// 重りがスイッチに張り付いているかを設定
	void SetIsAttach(bool enable);
	// 重りがスイッチに張り付いているか
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

	// 重りがスイッチに張り付いているか
	bool mIsAttach;
};