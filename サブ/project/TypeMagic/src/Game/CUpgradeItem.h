#pragma once
#include "CItemBase.h"

class CBillBoardImage;

// 強化アイテム
class CUpgradeItem : public CItemBase
{
public:
	// コンストラクタ
	CUpgradeItem();
	// デストラクタ
	~CUpgradeItem();

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 取得時の処理
	void GetItem() override;

	// 更新
	void Update() override;

private:
	// 全て削除
	void Delete() override;

	// コライダーを生成
	void CreateCol() override;

	// コライダー
	CCollider* mpCol;

	// アイテムの画像
	CBillBoardImage* mpItemImg;

	// 上へ移動している
	bool mIsMoveUp;
	// 経過時間
	float mElapsedTime;
};