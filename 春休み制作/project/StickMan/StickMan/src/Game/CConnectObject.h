#pragma once
#include "CObjectBase.h"

class CConnectTarget;
class CModel;

// 繋げれるオブジェクトのベースクラス
class CConnectObject : public CObjectBase
{
public:
	// コンストラクタ
	CConnectObject(float weight = 0.0f, 
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// デストラクタ
	virtual ~CConnectObject();

	// オブジェクト削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	// 描画
	void Render() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自分のコライダー</param>
	/// <param name="other">衝突した相手のコライダ―</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 引っ張られた時の処理
	virtual void Pull();

	// 接続ターゲットの作成
	void CreateTarget(CVector pos);

protected:
	// 接続ターゲット
	std::vector<CConnectTarget*> mTargets;

	// 重さ
	float mWeight;

	CCollider* mpCol;	// コライダー

	CModel* mpModel;	// モデル

	float mMoveSpeedY;	// 上下方向の移動速度
};