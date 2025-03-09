#include "CDeliveryHpUI2D.h"
#include "CImage.h"
#include "CTextUI2D.h"

// 配達物の画像のパス
#define DELIVERY_ITEM_PATH "UI/deliveryItem.png"
// 配達物の画像のサイズの倍率
#define DELIVERY_ITEM_SIZE 0.5f
// テキストのフォントサイズ
#define TEXT_FONT_SIZE 64
// テキストのオフセット座標
#define TEXT_OFFSET_POS CVector2(WINDOW_WIDTH * 0.125f,WINDOW_HEIGHT * 0.1f)

// コンストラクタ
CDeliveryHpUI2D::CDeliveryHpUI2D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpDeliveryItemImg(nullptr)
	, mCurrPoint(0)
{
	// 配達物のイメージを読み込み
	mpDeliveryItemImg = new CImage
	(
		DELIVERY_ITEM_PATH,
		ETaskPriority::eUI,0,
		ETaskPauseType::eGame,
		false,false
	);
	// サイズ倍率を適用
	CVector2 size = mpDeliveryItemImg->GetSize();
	mpDeliveryItemImg->SetSize(size * DELIVERY_ITEM_SIZE);

	// 持っている配達物の数のテキストを生成
	mpNumText = new CTextUI2D
	(
		ETaskPauseType::eGame,
		false
	);
	// テキストのフォントサイズを設定
	mpNumText->SetFontSize(TEXT_FONT_SIZE);
	// 座標を設定
	mpNumText->Position(TEXT_OFFSET_POS);
}

// デストラクタ
CDeliveryHpUI2D::~CDeliveryHpUI2D()
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
void CDeliveryHpUI2D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 現在値を設定
void CDeliveryHpUI2D::SetCurrPoint(int point)
{
	mCurrPoint = point;
}

// 更新
void CDeliveryHpUI2D::Update()
{
	mpDeliveryItemImg->Update();
	// テキストを設定
	mpNumText->ChangeToStr("×%d\n", mCurrPoint);
	mpNumText->Update();
}

// 描画
void CDeliveryHpUI2D::Render()
{
	mpDeliveryItemImg->Render();
	mpNumText->Render();
}