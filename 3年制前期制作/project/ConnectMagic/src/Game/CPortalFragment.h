#pragma once
#include "CConnectObject.h"
#include "CFragmentType.h"
#include "CFragmentMergeArea.h"

class CImage3D;

// ポータルの欠片クラス
class CPortalFragment : public CConnectObject
{
public:
	// コンストラクタ
	CPortalFragment(EFragmentType fragmentType);
	// デストラクタ
	~CPortalFragment();

	// 更新
	void Update() override;

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// コライダーを生成
	void CreateCol();
	// 欠片の種類
	EFragmentType mFragmentType;

	// 欠片の画像
	CImage3D* mpFragmentImg;

	// 合成場所に着いたか
	bool mIsMerge;

	// 合成場所の座標
	CVector mMergePos;

	// 合成地
	CFragmentMergeArea* mpMergeArea;
};