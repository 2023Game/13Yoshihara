#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"

class CGaugeUI2D;
class CInteractObject;

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayerBase : public CXCharacter
{
public:
	// インスタンスのポインタの取得
	static CPlayerBase* Instance();

	// コンストラクタ
	CPlayerBase();
	// デストラクタ
	~CPlayerBase();

	// オブジェクト削除処理
	void DeleteObject(CObjectBase* obj) override;
	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

	// 描画
	void Render();

	// 一番近くにある調べるオブジェクトを取得
	CInteractObject* GetNearInteractObject() const;

protected:
	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec();

	// モーションブラーの更新処理
	void UpdateMotionBlur();

	// プレイヤーのインスタンス
	static CPlayerBase* spInstance;

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsGravity;	// 重力を掛けるかどうか

	CVector mGroundNormal;	// 接地している地面の法線

	CCollider* mpBodyCol;	// 本体のコライダ―
	CCollider* mpAttackCol;	// 攻撃のコライダ―

	CTransform* mpRideObject;

	// モーションブラーを掛ける残り時間
	float mMotionBlurRemainTime;

	// 近くにある調べるオブジェクトのリスト
	std::list<CInteractObject*> mNearInteractObjs;
	// 調べるオブジェクト探知用のコライダ―
	CCollider* mpSearchCol;

	// Hpゲージ
	CGaugeUI2D* mpHpGauge;
};
