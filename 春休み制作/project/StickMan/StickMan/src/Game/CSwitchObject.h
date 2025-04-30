#pragma once
#include "CObjectBase.h"

// スイッチで作用するオブジェクトのベースクラス
class CSwitchObject : public CObjectBase
{
public:
	// コンストラクタ
	CSwitchObject(ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// デストラクタ
	virtual ~CSwitchObject();

	// 更新
	void Update() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自分のコライダー</param>
	/// <param name="other">衝突した相手のコライダ―</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// オンオフを切り替える
	void SetOnOff(bool isOnOff);

protected:
	// オンオフの状態
	enum class EState
	{
		eOff,
		eOn,
	};
	EState mState;
	// 状態の変更
	void ChangeState(EState state);	
	
	// 作用していない時の処理
	virtual void UpdateOff();
	// 作用している時の処理
	virtual void UpdateOn();
};