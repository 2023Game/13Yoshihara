#pragma once
#include "CObjectBase.h"

class CPortalFragment;

// 欠片を合成する場所
class CFragmentMergeArea : public CObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="num">欠片の数</param>
	CFragmentMergeArea(int num);
	// デストラクタ
	~CFragmentMergeArea();

	/// <summary>
	/// 欠片があるかを設定
	/// </summary>
	/// <param name="num">番号</param>
	/// <param name="enable">あるか</param>
	void SetIsFragment(int num, bool enable);
	// 欠片のポインタを取得
	CPortalFragment* GetFragment(int num);

	// 更新
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;
	// 欠片を合成する場所のコライダー
	CCollider* mpMergeAreaCol;

	// すべて削除
	void Delete() override;

	// 合成
	void Merge();

	// 欠片
	std::vector<CPortalFragment*> mFragments;

	// 欠片が合成場所にあるか
	std::vector<bool> mIsFragments;
};