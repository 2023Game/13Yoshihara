#pragma once
#include "CVehicleBase.h"
#include "CCarStatus.h"

class CModel;

// 車のクラス
class CCar : public CVehicleBase , public CCarStatus
{
public:
	// コンストラクタ
	CCar(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road);
	// デストラクタ
	~CCar();

	// 更新
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

private:
	// 移動処理
	void UpdateMove();
	// 停止処理
	void UpdateStop();
	// 壊れた処理
	void UpdateBroken();
	// 車線変更処理
	void UpdateChangeRoad();

	// 車の状態
	enum class EState
	{
		eMove,		// 移動
		eStop,		// 停止
		eBroken,	// 壊れる
		eChangeRoad,// 車線変更
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// 車の状態

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
#endif
};