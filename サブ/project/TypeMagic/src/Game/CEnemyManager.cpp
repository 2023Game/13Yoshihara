#include "CEnemyManager.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "Maths.h"

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
}

// 敵の数を増加
void CEnemyManager::AddEnemy()
{
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
	}
}

// 敵を削除
void CEnemyManager::DeleteEnemy(CEnemy* enemy)
{
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
			// 次へ
			it++;
		}
	}
}
