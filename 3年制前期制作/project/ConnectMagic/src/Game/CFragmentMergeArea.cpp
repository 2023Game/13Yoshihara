#include "CFragmentMergeArea.h"
#include "CColliderSphere.h"
#include "CPortal.h"
#include "CPortalFragment.h"

// 半径
#define RADIUS 10.0f

// ポータルのオフセット座標
#define PORTAL_OFFSET_POS CVector(0.0f,10.0f,0.0f)

// コンストラクタ
CFragmentMergeArea::CFragmentMergeArea(int num)
	: CObjectBase(ETag::eFragmentMergeArea, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{
	for (int i = 0; i < num; i++)
	{
		// 欠片を生成
		mFragments.push_back(new CPortalFragment(static_cast<EFragmentType>(i), this));
		// 欠片が合成場所にない
		mIsFragments.push_back(false);
	}
	// コライダーを生成
	CreateCol();
}

// デストラクタ
CFragmentMergeArea::~CFragmentMergeArea()
{
	SAFE_DELETE(mpMergeAreaCol);
}

// 欠片があるかを設定
void CFragmentMergeArea::SetIsFragment(int num, bool enable)
{
	// サイズ外なら処理しない
	if (mIsFragments.size() <= num) return;
	mIsFragments[num] = enable;
}

// 欠片のポインタを取得
CPortalFragment* CFragmentMergeArea::GetFragment(int num)
{
	// サイズ外なら処理しない
	if (mFragments.size() <= num) return nullptr;
	return mFragments[num];
}

// 更新
void CFragmentMergeArea::Update()
{
	// 全てtrueなら
	for (bool isFragment : mIsFragments)
	{
		if (!isFragment) return;
	}

	// 合成
	Merge();
}

// コライダーを生成
void CFragmentMergeArea::CreateCol()
{
	mpMergeAreaCol = new CColliderSphere(
		this, ELayer::eFragmentMergeArea,
		RADIUS, true
	);
	// オブジェクトとだけ衝突判定
	mpMergeAreaCol->SetCollisionLayers({ ELayer::eObject });
}

// すべて削除
void CFragmentMergeArea::Delete()
{
	for (int i = 0; i < mFragments.size(); i++)
	{
		// 削除
		mFragments[i]->Kill();
		mFragments[i]->Delete();
	}
	// 配列を空にする
	mFragments.clear();
}

// 合成
void CFragmentMergeArea::Merge()
{
	// ポータルを生成
	CPortal* portal = new CPortal();
	// 座標を生成
	portal->Position(Position() + PORTAL_OFFSET_POS);

	// すべて削除
	Delete();
	// 自身も削除
	Kill();
}
