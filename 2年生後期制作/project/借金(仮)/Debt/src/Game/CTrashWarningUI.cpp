#include "CTrashWarningUI.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashEnemyManager.h"
#include "CTrashVehicleManager.h"
#include "CSound.h"

// アルファの変化間隔
#define CHANGE_ALPHA_INTERVAL 0.01f
// 変動するアルファ値
#define CHANGE_ALPHA 0.05f
// アルファの最低値
#define MIN_ALPHA 0.0f
// アルファの最高値
#define MAX_ALPHA 0.50f
// 待機時間
#define WAIT_TIME 0.2f

#define SE_VOLUME 0.5f
// 再生間隔
#define SE_INTERVAL 3.0f

// 警告画像のパス
#define WARNING_PATH "UI/trash_game_warning.png"

// コンストラクタ
CTrashWarningUI::CTrashWarningUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIntervalTime(0.0f)
{
	// 警告音を設定
	mpAlarmSE = CResourceManager::Get<CSound>("AlarmSE");

	// エリア外の警告画像
	mpWarningImg = new CImage
	(
		WARNING_PATH,
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
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
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// 待機
	case 0:
		// 待機時間が経過したら
		if (mElapsedTime >= WAIT_TIME)
		{
			mElapsedTime = 0.0f;
			// 次へ
			mStateStep++;
		}
	
		break;

		// アルファの減少
	case 1:
		// 変化の間隔の時間が過ぎたら
		if (mElapsedTime >= CHANGE_ALPHA_INTERVAL)
		{
			mElapsedTime = 0.0f;
			// アルファを減少
			mpWarningImg->SetAlpha(mpWarningImg->GetAlpha() - CHANGE_ALPHA);
			// アルファが最低値以下なら
			if (mpWarningImg->GetAlpha() <= MIN_ALPHA)
			{
				// 次へ
				mStateStep++;
			}
		}
		break;

		// アルファの増加
	case 2:
		// 変化の間隔の時間が過ぎたら
		if (mElapsedTime >= CHANGE_ALPHA_INTERVAL)
		{
			mElapsedTime = 0.0f;
			// アルファを増加
			mpWarningImg->SetAlpha(mpWarningImg->GetAlpha() + CHANGE_ALPHA);
			// アルファが最高値以上なら
			if (mpWarningImg->GetAlpha() >= MAX_ALPHA)
			{
				// 0へ
				mStateStep = 0;
			}
		}
		break;
	}
}

// 描画
void CTrashWarningUI::Render()
{
	mIntervalTime += Times::DeltaTime();
	// プレイヤーを取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());

	// 敵管理クラス取得
	CTrashEnemyManager* enemyMgr = CTrashEnemyManager::Instance();
	// 車両管理クラス取得
	CTrashVehicleManager* vehicleMgr = CTrashVehicleManager::Instance();
	// プレイヤーがZのエリア外かつ
	// お仕置き用の敵が出現していない場合描画
	if (player->AreaOutZ() &&
		!enemyMgr->GetPopPunisherEnemy())
	{
		// 警告音再生
			// 再生間隔が経過したら
		if (mIntervalTime >= SE_INTERVAL)
		{
			mIntervalTime = 0.0f;
			mpAlarmSE->Play(SE_VOLUME, true);
		}
		mpWarningImg->Render();
	}	
	// プレイヤーがXのエリア外かつ
	// お仕置き用のトラックが出現していない場合描画
	else if (player->AreaOutX() &&
		!vehicleMgr->GetPopPunisherTruck())
	{
		// 警告音再生
		// 再生間隔が経過したら
		if (mIntervalTime >= SE_INTERVAL)
		{
			mIntervalTime = 0.0f;
			mpAlarmSE->Play(SE_VOLUME, true);
		}
		mpWarningImg->Render();
	}
	// 警告が出ていないなら
	else
	{
		// 次範囲外に出たときすぐ再生できるように設定
		mIntervalTime = SE_INTERVAL;
		// 警告音を停止
		mpAlarmSE->Stop();
	}
}
