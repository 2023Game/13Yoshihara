#pragma once
#include "CObjectBase.h"

// TODO：エリアの可視化
// 判定に触れると次からその場所でリスポーンするエリアのクラス
class CRespawnArea : public CObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="respawnPos">リスポーン地点</param>
	/// <param name="radius">衝突判定の半径</param>
	CRespawnArea(CVector respawnPos, float radius);
	// デストラクタ
	~CRespawnArea();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
private:
	// コライダーを生成
	void CreateCol(float radius);
	// プレイヤーが触れるとリスポーン地点に設定するコライダー
	CCollider* mpRespawnCol;
};