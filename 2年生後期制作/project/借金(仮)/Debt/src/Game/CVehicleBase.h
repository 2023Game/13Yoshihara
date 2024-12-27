#pragma once
#include "CCharaBase.h"
#include "CVehicleStatus.h"

class CModel;

// 車両の基底クラス
class CVehicleBase : public CCharaBase , public CVehicleStatus
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
	/// <param name="isEnd">trueならば、終了した</param>
	void ChangeRoad(bool& isEnd);

	// どの道にいる状態かを変更する
	void ChangeRoadType(ERoadType roadType);
	// 今どの道にいるか取得する
	ERoadType GetRoadType() const;
protected:
	// 移動処理
	void UpdateMove();
	// 停止処理
	void UpdateStop();
	// 壊れた処理
	void UpdateBroken();
	// 車線変更処理
	void UpdateChangeRoad();

	// トラックの状態
	enum class EState
	{
		//	共通
		eMove,		// 移動
		eStop,		// 停止
		eBroken,	// 壊れる
		eChangeRoad,// 車線変更

		// トラック
		eCollect,	// 回収
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// 車両の状態
#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
#endif

	// どの道にいる状態か
	ERoadType mRoadType;
	// 今の道の進む方向
	CVector mCurrentRoadRotation;

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