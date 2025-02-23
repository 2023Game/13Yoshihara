#pragma once
#include "CObjectBase.h"
#include "RoadType.h"

class CModel;
class CSound;

// 配達のフィールドに落ちているアイテムクラス
class CDeliveryFieldItem : public CObjectBase
{
public:
	// コンストラクタ
	CDeliveryFieldItem();
	// デストラクタ
	~CDeliveryFieldItem();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// どの道にいるかを取得する
	ERoadType GetRoadType() const;
	// どの道にいるか設定する
	void SetRoadType(ERoadType roadType);

private:
	// コライダーを生成
	void CreateCol() override;
	// 3Dモデル
	CModel* mpModel;
	// 本体コライダ―
	CCollider* mpBodyCol;
	// 大きいか
	bool mIsBig;
	// どの道にあるか
	ERoadType mRoadType;

	// ゲット音
	CSound* mpGetSE;
};