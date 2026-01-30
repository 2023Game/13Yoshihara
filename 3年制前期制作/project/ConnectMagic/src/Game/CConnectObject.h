#pragma once
#include "CObjectBase.h"
#include "ConnectObjectTag.h"
#include <vector>

// 引っ張る力
constexpr float PULL_POWER = 60.0f;

class CConnectPoint;
class CConnectTarget;
class CModel;
// bullet
class btPoint2PointConstraint;

// 繋げれるオブジェクトのベースクラス
class CConnectObject : public CObjectBase
{
public:
	// コンストラクタ
	CConnectObject(float weight = 0.0f, 
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	~CConnectObject();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	/// <summary>
	/// 繋がったときの処理
	/// </summary>
	/// <param name="otherPoint">相手の接続部</param>
	virtual void Connect(CConnectPoint* otherPoint, bool isWand);

	// 繋げた瞬間の処理
	void JustConnect(CConnectPoint* otherPoint);
	// 接続解除の処理
	void Disconnect(CConnectPoint* otherPoint);

	// 接続ターゲットの作成
	void CreateTarget(CVector pos);

	// 接続ターゲットを取得
	CConnectTarget* GetTarget() const;

	// 重さを取得
	float GetWeight();

	// 接続オブジェクトのタグを設定
	void SetConnectObjTag(EConnectObjTag tag);
	// 接続オブジェクトのタグを取得
	EConnectObjTag GetConnectObjTag();

	// 接続したときに移動の処理をするかを設定
	void SetMove(bool enable);

protected:
	// ジョイントを作成
	void CreateJoint(CConnectObject* otherObj);
	// ジョイントを削除
	void DeleteJoint();

	// ジョイント
	btPoint2PointConstraint* mpJoint;

	// 接続ターゲット
	CConnectTarget* mpTarget;

	// 重さ
	float mWeight;

	CModel* mpModel;	// モデル

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 上下方向の移動速度

	bool mIsGrounded;	// 接地しているか
	bool mIsMove;		// 接続によって移動するか

	CVector mPreOtherPointPos;	// 前回の接続相手の座標

	// 接続オブジェクトのタグ
	EConnectObjTag mConnectObjTag;
};