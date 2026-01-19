#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"

class CGaugeUI2D;
class CInteractObject;
class CRideableObject;

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

	// 描画
	void Render();

	// 死亡によってゲームが終了するかを取得する
	bool GetGameEnd();
	// 死亡によってゲームが終了するかを設定する
	void SetGameEnd(bool isEnd);

	// アニメーションを時間を指定して設定
	void ChangeAnimationTime(int type, float time);

	// 接地しているかのセッターゲッター
	void SetGrounded(bool isGrounded);
	bool GetGrounded() const;

protected:
	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);
	// アニメーションブレンド切り替え
	bool ChangeAnimationBlend(int type, float blendSpeed, bool restart = false);

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec();

	// モーションブラーの更新処理
	void UpdateMotionBlur();

	// プレイヤーのインスタンス
	static CPlayerBase* spInstance;

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsMoveDir;	// 移動方向を向くかどうか

	CVector mGroundNormal;	// 接地している地面の法線

	CTransform* mpRideObject;

	// モーションブラーを掛ける残り時間
	float mMotionBlurRemainTime;

	// Hpゲージ
	CGaugeUI2D* mpHpGauge;

	// 死亡してゲームが終了する時にtrueにする
	bool mIsGameEnd;
};
