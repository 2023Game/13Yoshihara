#pragma once
#include "CObjectBase.h"

class CSwitch;
class CModel;

// スイッチのボタン部分
class CSwitchButton : public CObjectBase
{
public:
	// コンストラクタ
	CSwitchButton(CVector pos, CSwitch* owner);
	// デストラクタ
	~CSwitchButton();

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

private:
	// コライダーを生成
	void CreateCol();

	enum class EState
	{
		eDefault,	// 押されていない
		ePush,		// 押されている
	};
	EState mState;
	// 状態を変更
	void ChangeState(EState state);

	// 押されていない時の更新処理
	void UpdateDefault();
	// 押されている時の更新処理
	void UpdatePush();

	float mDefaultY;	// 初期のY座標

	CCollider* mpPushCol;	// 押してるかの当たり判定

	CSwitch* mpOwner;	// 持ち主

	CModel* mpModel;
};