#include "CTrashWarningUI.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashEnemyManager.h"
#include "CVehicleManager.h"

// 警告画像のパス
#define WARNING_PATH "UI/trash_game_warning.png"
// 警告テキストの座標
#define TEXT_POS CVector(0.0f, WINDOW_HEIGHT * 0.4f, 0.0f)

// コンストラクタ
CTrashWarningUI::CTrashWarningUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// エリア外の警告画像
	mpWarningImg = new CImage
	(
		WARNING_PATH,
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpWarningImg->SetAlpha(0.5f);
	mpWarningImg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// デストラクタ
CTrashWarningUI::~CTrashWarningUI()
{
	SAFE_DELETE(mpWarningImg);
}

// 更新
void CTrashWarningUI::Update()
{
}

// 描画
void CTrashWarningUI::Render()
{
	// プレイヤーを取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	// 車両管理クラス取得
	CVehicleManager* vehicleMgr = CVehicleManager::Instance();
	// プレイヤーがXのエリア外かつ
	// お仕置き用のトラックが出現していない場合描画
	if (player->AreaOutX() &&
		!vehicleMgr->GetPopPunisherTruck())
	{
		// TODO：警告音再生
		mpWarningImg->Render();
		return;
	}
	// 敵管理クラス取得
	CTrashEnemyManager* enemyMgr = CTrashEnemyManager::Instance();
	// プレイヤーがZのエリア外かつ
	// お仕置き用の敵が出現していない場合描画
	if (player->AreaOutZ() &&
		!enemyMgr->GetPopPunisherEnemy())
	{
		// TODO：警告音再生
		mpWarningImg->Render();
		return;
	}
}
