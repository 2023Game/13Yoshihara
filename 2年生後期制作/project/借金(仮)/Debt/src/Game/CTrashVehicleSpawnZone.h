#pragma once
#include "CCharaBase.h"
#include "CColliderMesh.h"

// 車両の生成場所に車がいるか判断するためのクラス
class CTrashVehicleSpawnZone : public CCharaBase
{

private:
	CColliderMesh* mpSpawnZoneColliderMesh;

	// 生成可能かのデータ
	struct IsCanPops
	{
		bool IsLeft1CanPop;		// 左から1番目の道に生成可能か
		bool IsLeft2CanPop;		// 左から2番目の道に生成可能か
		bool IsRight1CanPop;	// 右から1番目の道に生成可能か
		bool IsRight2CanPop;	// 右から2番目の道に生成可能か
	};
	IsCanPops mIsCanPops;	// 生成可能かのデータ

	const float mLeft1PosX;	// 左から1番のX座標
	const float mLeft2PosX;	// 左から2番のX座標
	const float mRight1PosX;// 右から1番のX座標
	const float mRight2PosX;// 右から2番のX座標

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="left1">左から1番のX座標</param>
	/// <param name="left2">左から2番のX座標</param>
	/// <param name="right1">右から1番のX座標</param>
	/// <param name="right2">右から2番のX座標</param>
	CTrashVehicleSpawnZone(float left1, float left2, float right1, float right2);
	~CTrashVehicleSpawnZone();

	void Update();
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 生成可能かのデータを取得
	IsCanPops GetCanPops() const;

	// 生成可能かを設定
	void SetLeft1CanPop(bool canPop);	// 左から1番
	void SetLeft2CanPop(bool canPop);	// 左から2番
	void SetRight1CanPop(bool canPop);	// 右から1番
	void SetRight2CanPop(bool canPop);	// 右から2番
	
};