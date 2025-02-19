#include "CDeliveryHpUI3D.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "CTextUI3D.h"
#include "Maths.h"

#define WORLD_UNIT_PER_PIXEL 32.0f

// 配達物の画像のパス
#define DELIVERY_ITEM_PATH "UI/deliveryItem.png"
// 配達物の画像のサイズの倍率
#define DELIVERY_ITEM_SIZE 1.0f

// テキストのフォントサイズ
#define TEXT_FONT_SIZE 32
// テキストのオフセット座標
#define TEXT_OFFSET_POS CVector2(50.0f, -1500.0f)


// コンストラクタ
CDeliveryHpUI3D::CDeliveryHpUI3D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpDeliveryItemImg(nullptr)
	, mpNumText(nullptr)
	, mCurrPoint(1)
{
	// 配達物のイメージを読み込み
	mpDeliveryItemImg = new CImage3D
	(
		DELIVERY_ITEM_PATH,
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	// 縮尺設定
	mpDeliveryItemImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	// サイズ倍率を適用
	CVector2 size = mpDeliveryItemImg->GetSize();
	mpDeliveryItemImg->SetSize(size * DELIVERY_ITEM_SIZE);

	// 持っている配達物の数のテキストを生成
	mpNumText = new CTextUI3D
	(
		ETaskPauseType::eGame,
		false
	);
	// 縮尺設定
	mpNumText->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	// テキストのフォントサイズを設定
	mpNumText->SetFontSize(TEXT_FONT_SIZE);
	// テキストのオフセット座標を設定
	mpNumText->SetOffsetPos(TEXT_OFFSET_POS);
}

//デストラクタ
CDeliveryHpUI3D::~CDeliveryHpUI3D()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpDeliveryItemImg);
	// テキストを削除
	SAFE_DELETE(mpNumText);

	// 持ち主に削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 持ち主を設定
void CDeliveryHpUI3D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 現在値を設定
void CDeliveryHpUI3D::SetCurrPoint(int point)
{
	mCurrPoint = point;
}

// 更新
void CDeliveryHpUI3D::Update()
{
	mpDeliveryItemImg->Update();
	// テキストを設定
	mpNumText->ChangeToStr("×%d\n", mCurrPoint);
	mpNumText->Update();
}

// 描画
void CDeliveryHpUI3D::Render()
{	
	// 行列の保存
	glPushMatrix();

	// 常にカメラの方向を向かせる
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	mpDeliveryItemImg->Render();
	mpNumText->Render();

	// 行列を戻す
	glPopMatrix();
}
