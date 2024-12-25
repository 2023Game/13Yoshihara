#pragma once
#include "CCharaBase.h"

class CModel;

// 車両の基底クラス
class CVehicleBase : public CCharaBase
{
public:
	// どの道にいる状態か
	enum class ERoadType
	{
		None = -1,

		eLeft1,		// 左から一番目の道
		eLeft2,		// 左から二番目の道
		eRight1,	// 右から一番目の道
		eRight2,	// 右から二番目の道

	};

	// コンストラクタ
	CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road);
	// デストラクタ
	~CVehicleBase();

	// 更新
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// 描画
	void Render();

	/// <summary>
	/// 移動中かどうか
	/// </summary>
	/// <returns>trueならば、移動中</returns>
	bool IsMove() const;

	/// <summary>
	/// 車線を変更する
	/// </summary>
	/// <param name="moveSpeed">移動速度</param>
	/// <param name="isEnd">trueならば、終了した</param>
	void ChangeRoad(float moveSpeed, bool& isEnd);

	// どの道にいる状態かを変更する
	void ChangeRoadType(ERoadType roadType);
	// 今どの道にいるか取得する
	ERoadType GetRoadType() const;
protected:
	// どの道にいる状態か
	ERoadType mRoadType;

	// 移動速度
	CVector mMoveSpeed;

	CModel* mpModel;

	// 本体のコライダ―
	CCollider* mpBodyCol;
	// 前方判定コライダ―
	CCollider* mpFrontCol;
	// 横判定コライダ―
	CCollider* mpSideCol;
};