#pragma once
#include "CObjectBase.h"

class CModel;

class CDeliveryItem : public CObjectBase
{
public:
	// コンストラクタ
	CDeliveryItem();
	// デストラクタ
	~CDeliveryItem();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 移動を設定
	void SetMoveSpeed(CVector moveSpeed);

private:
	// コライダ―を生成
	void CreateCol();
	// 3dモデル
	CModel* mpModel;
	// 本体コライダ―
	CCollider* mpBodyCol;

	// 移動
	CVector mMoveSpeed;
	// 経過時間
	float mElapsedTime;
};