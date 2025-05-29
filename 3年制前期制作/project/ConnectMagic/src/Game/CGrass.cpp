#include "CGrass.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"
#include "CFlamethrower.h"
#include "CConnectPointManager.h"

// 画像のサイズ
#define SIZE 2.0f

// アルファの減少速度
#define ALPHA_DECREASE_SPEED 0.5f

// コンストラクタ
CGrass::CGrass(CVector fireOffsetPos, float fireScale)
	: CFire("", fireOffsetPos, fireScale)
{
	SetConnectObjTag(EConnectObjTag::eGrass);

	// 草の画像を生成
	mpGrassImage = new CBillBoardImage
	(
		"Field/Grass/Grass.png", ETag::eConnectObject,
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
}

// デストラクタ
CGrass::~CGrass()
{
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

// 描画
void CGrass::Render()
{
	mpGrassImage->Render();
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
