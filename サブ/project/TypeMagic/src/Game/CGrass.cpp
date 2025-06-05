#include "CGrass.h"
#include "CColliderRectangle.h"
#include "CBillBoardImage.h"
#include "CFlamethrower.h"
#include "CConnectPointManager.h"

// 画像のサイズ
#define GRASS_SIZE 2.0f
#define VINE_SIZE 12.0f

// アルファの減少速度
#define ALPHA_DECREASE_SPEED 0.5f

// 頂点
#define VERT_POS_1 CVector(-VINE_WIDTH/2.0f, VINE_HEIGHT, 0.0f)
#define VERT_POS_2 CVector(-VINE_WIDTH/2.0f,		0.0f, 0.0f)
#define VERT_POS_3 CVector( VINE_WIDTH/2.0f,		0.0f, 0.0f)
#define VERT_POS_4 CVector( VINE_WIDTH/2.0f, VINE_HEIGHT, 0.0f)

// コンストラクタ
CGrass::CGrass(EGrassType type, CVector fireOffsetPos, float fireScale)
	: CFire(fireOffsetPos, fireScale)
	, CItemDrop()
	, mGrassType(type)
	, mpVineCol(nullptr)
{
	// タグを草に設定
	SetConnectObjTag(EConnectObjTag::eGrass);

	// 画像を生成
	CreateImg();

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CGrass::~CGrass()
{
	SAFE_DELETE(mpVineCol);
}

// 更新
void CGrass::Update()
{
	// 基底クラスの更新処理
	CConnectObject::Update();

	// 炎がついているなら
	if (mIsFire)
	{
		// アルファを減算していく
		float alpha = mpGrassImage->GetAlpha();
		alpha -= ALPHA_DECREASE_SPEED * Times::DeltaTime();
		// アルファが0以下なら
		if (alpha <= 0.0f)
		{
			// 全て削除
			Delete();
			return;
		}
		// アルファを設定
		mpGrassImage->SetAlpha(alpha);
	}
}

// コライダーを生成
void CGrass::CreateCol()
{
	switch (mGrassType)
	{
		// 通常の草なら
	case EGrassType::eGrass:
		// 基底クラスのコライダーを生成
		CFire::CreateCol();
		break;

		// ツタなら
	case EGrassType::eVine:
		// 四角形コライダーを生成
		mpCol = new CColliderRectangle(
			this, ELayer::eObject,
			VERT_POS_1,
			VERT_POS_2,
			VERT_POS_3,
			VERT_POS_4,
			true
		);
		// 少しずらす
		mpCol->Position(VectorZ());
		// オブジェクトとプレイヤーと敵と
		// 接続オブジェクトの探知用とだけ衝突判定
		mpCol->SetCollisionLayers({ ELayer::eObject,
			ELayer::ePlayer,ELayer::eEnemy,ELayer::eConnectSearch });
		mpVineCol = new CColliderRectangle(
			this, ELayer::eObject,
			VERT_POS_4,
			VERT_POS_3,
			VERT_POS_2,
			VERT_POS_1,
			true
		);
		// 少しずらす
		mpVineCol->Position(-VectorZ());
		// オブジェクトとプレイヤーと敵と
		// 接続オブジェクトの探知用とだけ衝突判定
		mpVineCol->SetCollisionLayers({ ELayer::eObject,
			ELayer::ePlayer,ELayer::eEnemy,ELayer::eConnectSearch });
		break;
	}
}

// 燃えたときの処理
void CGrass::Burning()
{
	// 落とすアイテムの座標を設定
	SetDropItemPos(Position());
	// アイテムを落とす
	Drop();
}

// 全て削除
void CGrass::Delete()
{
	// 炎生成も削除
	mpFlamethrower->Kill();
	mpFlamethrower = nullptr;
	// 草の画像も削除
	mpGrassImage->Kill();
	mpGrassImage = nullptr;
	// 削除
	Kill();
}

// 画像を生成
void CGrass::CreateImg()
{
	switch (mGrassType)
	{
	case EGrassType::eGrass:
	{
		// 草の画像を生成
		mpGrassImage = new CBillBoardImage
		(
			"Field/Grass/Grass.png", ETag::eConnectObject,
			ETaskPauseType::eGame
		);
		// サイズを取得
		CVector2 size = mpGrassImage->GetSize();
		size *= GRASS_SIZE;
		// サイズを設定
		mpGrassImage->SetSize(size);
		// サイズの半分強上に上げる
		mpGrassImage->SetOffsetPos(CVector2(0.0f, size.Y() * 0.8f));
		break;
	}
	case EGrassType::eVine:
		// ツタの画像を生成
		mpGrassImage = new CImage3D(
			"Field/Grass/Vine.png", ETag::eConnectObject,
			ETaskPriority::eDefault, 0,
			ETaskPauseType::eGame
		);
		// 裏面描画
		mpGrassImage->SetBackFace(true);
		// サイズを取得
		CVector2 size = mpGrassImage->GetSize();
		size *= VINE_SIZE;
		// サイズを設定
		mpGrassImage->SetSize(size);
		// サイズ分上に上げる
		mpGrassImage->SetOffsetPos(CVector2(0.0f, size.Y()));
		break;
	}
	// 親に設定
	mpGrassImage->SetParent(this);
}
