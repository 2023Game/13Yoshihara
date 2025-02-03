#include "CSelectJobUI.h"
#include "CTextUI2D.h"
#include "CJobStatusManager.h"
#include "JobType.h"

// 選択中の仕事の文字列
#define TRASH		"選択中：ゴミ拾い"
#define DELIVERY	"選択中：配達"
#define NONE		"選択中：未選択"

// テキストUIのオフセット座標
#define TEXT_OFFSET_POS CVector(WINDOW_WIDTH * 0.725f,0.0f,0.0f)

// コンストラクタ
CSelectJobUI::CSelectJobUI()
{
	// UIを生成
	mpSelectJobUI = new CTextUI2D(ETaskPauseType::eMenu, true, nullptr);
	mpSelectJobUI->Position(TEXT_OFFSET_POS);
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
		mpSelectJobUI->SetStr(TRASH);
	}
	// 選択中の仕事が配達なら
	else if (selectJob == EJobType::eDelivery)
	{
		// 描画する文字列を設定
		mpSelectJobUI->SetStr(DELIVERY);
	}
	// 選択されていないなら
	else
	{
		// 描画する文字列を設定
		mpSelectJobUI->SetStr(NONE);
	}
}
