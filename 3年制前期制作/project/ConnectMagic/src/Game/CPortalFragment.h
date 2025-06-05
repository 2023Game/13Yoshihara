#pragma once
#include "CConnectObject.h"
#include "FragmentType.h"
#include "CDropItem.h"

class CImage3D;
class CFragmentMergeArea;

// ポータルの欠片クラス
class CPortalFragment : public CConnectObject, public CDropItem
{
public:
	// コンストラクタ
	CPortalFragment(EFragmentType fragmentType, CFragmentMergeArea* mergeArea);
	// デストラクタ
	~CPortalFragment();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// すべて削除
	void Delete() override;

private:
	// 状態
	enum class EState
	{
		eJump,	// 最初の飛び上がり
		eIdle,	// 待機
		eMerge,	// 合成
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;
	int mStateStep;		// 状態内のステップ管理用
	float mElapsedTime;	// 経過時間計測用

	// 飛び上がり
	void UpdateJump();	
	// 待機
	void UpdateIdle();	
	// 合成
	void UpdateMerge();

	// コライダーを生成
	void CreateCol();
	// 欠片の種類
	EFragmentType mFragmentType;

	// 欠片の画像
	CImage3D* mpFragmentImg;

	// 合成地
	CFragmentMergeArea* mpMergeArea;
};