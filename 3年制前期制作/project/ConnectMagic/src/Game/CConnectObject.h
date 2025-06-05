#pragma once
#include "CObjectBase.h"
#include "ConnectObjectTag.h"

// 引っ張る力
#define PULL_POWER 60.0f

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
	~CConnectObject();

	// オブジェクト削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	// 更新
	void Update() override;
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

	/// <summary>
	/// 引っ張られたときの処理
	/// </summary>
	/// <param name="pullDir">引っ張られる方向</param>
	/// <param name="opponentWeight">相手の重さ</param>
	virtual void Pull(CVector pullDir, float opponentWeight);

	/// <summary>
	/// 繋がったときの処理
	/// </summary>
	/// <param name="other">相手</param>
	virtual void Connect(CConnectObject* other);

	// 接続ターゲットの作成
	void CreateTarget(CVector pos);

	// 接続ターゲットを取得
	std::vector<CConnectTarget*> GetTargets();

	// 重さを取得
	float GetWeight();

	// 重力を掛けるかを設定
	void SetGravity(bool isGravity);

	// 接続オブジェクトのタグを設定
	void SetConnectObjTag(EConnectObjTag tag);
	// 接続オブジェクトのタグを取得
	EConnectObjTag GetConnectObjTag();

protected:
	// 接続ターゲット
	std::vector<CConnectTarget*> mTargets;

	// 重さ
	float mWeight;

	CCollider* mpCol;	// コライダー

	CModel* mpModel;	// モデル

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 上下方向の移動速度

	bool mIsGrounded;	// 接地しているか
	bool mIsGravity;	// 重力を掛けるか

	// 接続オブジェクトのタグ
	EConnectObjTag mConnectObjTag;
};