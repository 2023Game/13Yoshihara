#include "CSelectJobUI.h"
#include "CTextUI2D.h"
#include "CJobStatusManager.h"
#include "JobType.h"
#include "CImage.h"

// 選択中の仕事の文字列
#define TRASH		"選択中：ゴミ拾い"
#define DELIVERY	"選択中：配達"
#define NONE		"選択中：未選択"

// テキストUIのオフセット座標
#define TEXT_OFFSET_POS CVector2(WINDOW_WIDTH * 0.7f, 50.0f)

// コンストラクタ
CSelectJobUI::CSelectJobUI()
	: CHomeUIBase()
{
	// テキストの座標を設定
	mpTextUI->Position(TEXT_OFFSET_POS);

	// 背景削除
	SAFE_DELETE(mpBackground);
}

// デストラクタ
CSelectJobUI::~CSelectJobUI()
{
}

// 更新
void CSelectJobUI::Update()
{
	// 選択中の仕事を取得
	EJobType selectJob = CJobStatusManager::Instance()->GetSelectJob();
	// 選択中の仕事がゴミ拾いなら
	if (selectJob == EJobType::eTrash)
	{
		// 描画する文字列を設定
		mpTextUI->SetStr(TRASH);
	}
	// 選択中の仕事が配達なら
	else if (selectJob == EJobType::eDelivery)
	{
		// 描画する文字列を設定
		mpTextUI->SetStr(DELIVERY);
	}
	// 選択されていないなら
	else
	{
		// 描画する文字列を設定
		mpTextUI->SetStr(NONE);
	}

	CHomeUIBase::Update();
}

// 描画
void CSelectJobUI::Render()
{
	CHomeUIBase::Render();
}
