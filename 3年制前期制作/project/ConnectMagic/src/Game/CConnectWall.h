#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

// 道を繋ぐ壁クラス
class CConnectWall : public CObjectBase
{
public:
	// コンストラクタ
	CConnectWall();
	// デストラクタ
	~CConnectWall();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 初期回転値を設定
	void SetDefaultRot(CVector rot);

	// 開閉する
	void OpenClose();

private:
	// 開く
	void Open();
	// 閉じる
	void Close();

	// 繋ぐ壁の状態
	enum class EState
	{
		eIdle,	// 待機状態
		eOpen,	// 開く状態
		eClose,	// 閉じる状態
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用

	// コライダ―を生成
	void CreateCol();

	// コライダ―
	CCollider* mpMeshCol;
	// モデル
	CModel* mpModel;

	// 開いているか
	bool mIsOpen;
	// 初期回転値（立っている状態）
	CVector mDefaultRot;
	// 総回転量
	CVector mTotalRot;
};