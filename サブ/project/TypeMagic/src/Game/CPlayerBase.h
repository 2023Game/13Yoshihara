#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"

class CGaugeUI2D;

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

	// 死亡によってゲームが終了するかを取得する
	bool GetGameEnd();
	// 死亡によってゲームが終了するかを設定する
	void SetGameEnd(bool isEnd);

protected:
	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);
	// アニメーションブレンド切り替え
	bool ChangeAnimationBlend(int type, float blendSpeed, bool restart = false);

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec();

	// プレイヤーのインスタンス
	static CPlayerBase* spInstance;

	CCollider* mpBodyCol;	// 本体のコライダ―
	CCollider* mpAttackCol;	// 攻撃のコライダ―

	// モーションブラーを掛ける残り時間
	float mMotionBlurRemainTime;

	// Hpゲージ
	CGaugeUI2D* mpHpGauge;

	// 死亡してゲームが終了する時にtrueにする
	bool mIsGameEnd;

	// TODO：テストモデル
	CModel* mpTestModel;
};
