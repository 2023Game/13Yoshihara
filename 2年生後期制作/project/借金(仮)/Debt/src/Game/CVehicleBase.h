#pragma once
#include "CCharaBase.h"

class CModel;
class CNavNode;

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
	CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints);
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
	// 最後の巡回ポイントまでの移動が終了したかどうか
	bool GetMoveEnd() const;

	/// <summary>
	/// 車線を変更する
	/// </summary>
	/// <param name="isEnd">trueならば、終了した</param>
	void ChangeRoad(bool& isEnd);

	// どの道にいる状態かを変更する
	void ChangeRoadType(ERoadType roadType);
	// 今どの道にいるか取得する
	ERoadType GetRoadType() const;

	// 本体コライダ―を取得する
	CCollider* GetBodyCol() const;
	// 経路探索用コライダ―を取得する
	CCollider* GetNavCol() const;

	// 巡回ポイントのリストを設定する
	void SetPatrolPoints(std::vector<CNavNode*> patrolPoints);

	// 車両の有効無効を切り替える
	void SetOnOff(bool setOnOff);
	// mNextPatrolIndexをリセット
	void ResetNextPatrolIndex();
protected:
	// どの道にいる状態か
	ERoadType mRoadType;
	// 今の道の進む方向
	CVector mCurrentRoadRotation;
	// 移動しているか
	bool mIsMove;

	// 移動速度
	CVector mMoveSpeed;

	CModel* mpModel;

	// 車両の周りのノード
	CNavNode* mpNode0;
	CNavNode* mpNode1;
	CNavNode* mpNode2;
	CNavNode* mpNode3;

	// 本体のコライダ―
	CCollider* mpBodyCol;
	// 経路探索用のコライダ―
	CCollider* mpNavCol;

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	/// <summary>
	/// 次に巡回するポイントを変更
	/// </summary>
	/// <param name="patrolNearDist">設定できる最短距離</param>
	void ChangePatrolPoint(float patrolNearDist);

	// 巡回ポイントのリスト
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex;	// 次に巡回するポイントの番号

	std::vector<CNavNode*> mMoveRoute;	// 求めた最短経路記憶用
	int mNextMoveIndex;					// 次に移動するノードのインデックス値

	bool mIsMoveEnd;	// 最後まで移動したかどうか
	bool mIsMovePause;	// 移動の中断中かどうか
	bool mIsChangeRoad;	// 道が変更されたかどうか
};