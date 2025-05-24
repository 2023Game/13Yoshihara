#include "CGrass.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"

// 草の重さ(動かさない）
#define WEIGHT 1.0f

// 画像のサイズ
#define SIZE 2.0f

// 接続ターゲットのオフセット座標
#define TARGET_OFFSET_POS CVector(0.0f, 2.0f, 0.0f)

// コンストラクタ
CGrass::CGrass()
	: CConnectObject(WEIGHT)
{
	// 重力無効
	mIsGravity = false;

	// 草の画像を生成
	mpGrassImage = new CBillBoardImage
	(
		"Field/Grass.png", ETag::eConnectObject,
		ETaskPauseType::eGame
	);
	// 親に設定
	mpGrassImage->SetParent(this);
	// サイズを取得
	CVector2 size = mpGrassImage->GetSize();
	size *= SIZE;
	// サイズを設定
	mpGrassImage->SetSize(size);
	// サイズの半分弱上に上げる
	mpGrassImage->SetOffsetPos(CVector(0.0f, size.Y() * 0.8f, 0.0f));

	// 接続ターゲットを生成
	CreateTarget(Position() + TARGET_OFFSET_POS);

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CGrass::~CGrass()
{
}

// 繋がったときの処理
void CGrass::Connect(CConnectObject* other)
{
	// 接続オブジェクトのタグが炎なら
	if (other->GetConnectObjTag() == EConnectObjTag::eFire)
	{
		// TODO：徐々に消えていく
	}
}

// コライダーを生成
void CGrass::CreateCol()
{
	mpCol = new CColliderSphere
	(
		this,ELayer::eObject,
		10.0f,true
	);
	// コネクトオブジェクトの探知用とだけ衝突判定
	mpCol->SetCollisionLayers({ ELayer::eConnectSearch });
}
