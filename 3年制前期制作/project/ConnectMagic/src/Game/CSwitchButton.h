#pragma once
#include "CObjectBase.h"

class CSwitch;
class CModel;

// スイッチのボタン部分
class CSwitchButton : public CObjectBase
{
public:
	// コンストラクタ
	CSwitchButton(const CVector& pos, const CVector& scale, CSwitch* owner, bool isAttach);
	// デストラクタ
	~CSwitchButton();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 衝突処理
	void OnSensorEnter(const CollisionData& data) override;

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

	CSwitch* mpOwner;	// 持ち主

	CModel* mpModel;

	// スイッチにweightが張り付くか
	bool mIsAttach;
};