#pragma once
#include "CObjectBase.h"
#include "CSavable.h"

class CSwitch;
class CModel;
class CCollider;

// スイッチで作用するオブジェクトのベースクラス
class CSwitchObject : public CObjectBase, public CSavable
{
public:
	// コンストラクタ
	CSwitchObject(ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	virtual ~CSwitchObject();

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

	// オンオフを切り替える
	virtual void SetOnOff(bool isOnOff);
	// オンかオフかを取得
	bool GetOnOff() const;

	// 作用するスイッチを設定する
	void SetSwitchs(std::vector<CSwitch*> switchs);

	// オンオフの状態
	enum class EState
	{
		eOff,
		eOn,
	};
protected:
	EState mState;
	// 状態の変更
	virtual void ChangeState(EState state);
	void SetState(EState state);
	EState GetState() const;
	
	// 作用していない時の処理
	virtual void UpdateOff();
	// 作用している時の処理
	virtual void UpdateOn();

	CModel* mpModel;

	CCollider* mpCol;

	// 作用するスイッチ
	std::vector<CSwitch*> mSwitchs;
	// オンのスイッチの数
	int mOnSwitchNum;
};