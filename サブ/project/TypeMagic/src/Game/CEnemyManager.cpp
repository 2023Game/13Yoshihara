#include "CEnemyManager.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "Maths.h"
#include "CGameCamera2.h"

#define DEFAULT_ENEMY_MAX_NUM 1		// 最大で出現する敵の数の初期値
#define DEFAULT_ENEMY_ONCE_NUM 1	// 一回で出現する敵の数の初期値	

// 出現する間隔
#define POP_RATE 5.0f

// 生成される距離
#define ENEMY_DIST 750.0f

// メインの出現方向
#define MAIN_POP_DIR 8
// サブの出現方向
#define SUB_POP_DIR 3

// 敵の管理クラスのインスタンス
CEnemyManager* CEnemyManager::spInstance = nullptr;

// インスタンスのポインタの取得
CEnemyManager* CEnemyManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CEnemyManager::CEnemyManager()
	: mEnemyMaxNum(DEFAULT_ENEMY_MAX_NUM)
	, mEnemyOnceNum(DEFAULT_ENEMY_ONCE_NUM)
	, mWaveNum(1)
	, mElapsedTime(POP_RATE)
{
	spInstance = this;
	// 出現位置を設定
	SetPopPos();
}

// デストラクタ
CEnemyManager::~CEnemyManager()
{
	if (spInstance == this)
	{
		SAFE_DELETE(spInstance);
	}
}

// 更新
void CEnemyManager::Update()
{
	mElapsedTime += Times::DeltaTime();

	// 生成間隔が経過しているかつ
	// 敵の数が最大値より少ない場合
	if (mElapsedTime >= POP_RATE &&
		mEnemys.size() < mEnemyMaxNum)
	{
		// 敵を追加
		AddEnemy();
		mElapsedTime -= POP_RATE;
	}
#if _DEBUG
	CDebugPrint::ForcePrint("EnemyNum:%d\n", mEnemys.size());
#endif
}

// 敵の数を増加
void CEnemyManager::AddEnemy()
{
	// 出現位置を設定
	SetPopPos();
	// プレイヤーを取得
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
	// 敵を生成
	CEnemy* enemy = new CEnemy();
	enemy->SetOpponent(player);
	// 出現場所からランダム
	int rand = Math::Rand(0, MAIN_POP_DIR + SUB_POP_DIR);
	CVector pos = mPopPos[rand];
	// 座標を設定
	enemy->Position(pos);
	// 敵のリストに追加
	mEnemys.push_back(enemy);
	// 敵の数が1になった時はプレイヤーのターゲットに設定する
	if (mEnemys.size() == 1)
	{
		player->SetOpponent(enemy);
		CGameCamera2* camera = dynamic_cast<CGameCamera2*>(CCamera::CurrentCamera());

		if (camera != nullptr)
		{
			// 敵をロックオンターゲットに設定
			camera->SetLockOnTarget(enemy);
		}
	}
}

// 敵を削除
void CEnemyManager::DeleteEnemy(CEnemy* enemy)
{
	// ターゲットを変更するか
	bool targetChange = false;
	// カメラを取得
	CGameCamera2* camera = dynamic_cast<CGameCamera2*>(CCamera::CurrentCamera());
	if (camera != nullptr)
	{
		// 削除する敵とロックオンターゲットが一致したら
		if (camera->GetLockOnTarget() == enemy)
		{
			// ターゲットを変更する
			targetChange = true;
		}
	}

	auto it = mEnemys.begin();

	while (it != mEnemys.end())
	{
		// 一致するなら
		if (*it == enemy)
		{
			// タスクから削除
			(*it)->Kill();
			// リストから削除
			it = mEnemys.erase(it);
		}
		// 一致しないなら
		else
		{
			// ターゲットを変更するなら
			if (targetChange)
			{
				// プレイヤーを取得
				CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
				// 相手を設定
				player->SetOpponent((*it));
				// ターゲットを設定
				camera->SetLockOnTarget((*it));
				// もう変更しない
				targetChange = false;
			}
			// 次へ
			it++;
		}
	}
}

// 敵の出現位置を設定
void CEnemyManager::SetPopPos()
{
	// リストをクリア
	mPopPos.clear();
	CVector playerPos = CPlayer::Instance()->Position();
	// 敵が出現する方向を保存
	for (int i = 0; i < MAIN_POP_DIR; i++)
	{
		// メインの出現方向の角度
		float baseAngle = i * (360 / MAIN_POP_DIR);
		for (int j = -1; j <= 1; j++)
		{
			// メインとサブを足した出現方向
			float angle = baseAngle + j * (360 / (MAIN_POP_DIR * SUB_POP_DIR));
			// ラジアン角に変換
			angle = Math::DegreeToRadian(angle);
			// 生成される方向
			CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
			// 生成座標
			CVector pos = playerPos + dir * ENEMY_DIST;
			// 保存
			mPopPos.push_back(pos);
		}
	}
}

// ターゲット候補を取得
std::vector<CEnemy*> CEnemyManager::GetTargets()
{
	// ターゲット候補をクリア
	mTargets.clear();
	// カメラを取得
	CCamera* camera = CCamera::CurrentCamera();
	for (CEnemy* enemy : mEnemys)
	{
		// スクリーン座標に設定
		CVector2 screenPos = camera->WorldToScreenPos(enemy->Position());

		// 画面内なら
		if (screenPos.X() >= 0.0f && screenPos.X() <= WINDOW_WIDTH &&
			screenPos.Y() >= 0.0f && screenPos.Y() <= WINDOW_HEIGHT)
		{
			// ターゲット候補のリストに追加
			mTargets.push_back(enemy);
		}
	}

	return mTargets;
}
