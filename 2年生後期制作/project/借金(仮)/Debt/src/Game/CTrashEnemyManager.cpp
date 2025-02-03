#include "CTrashEnemyManager.h"
#include "CTrashEnemy.h"
#include "CPunisherTrashEnemy.h"
#include "CTrashPlayer.h"
#include "CTrashField.h"

// ゴミ拾いの敵管理クラスのインスタンス
CTrashEnemyManager* CTrashEnemyManager::spInstance = nullptr;

// 通常敵の初期座標
#define ENEMY_POS CVector(0.0f,0.0f,-50.0f)
// 通常敵の初期方向
#define ENEMY_ROT CVector(0.0f,180.0f,0.0f)

// お仕置き用の敵の生成時の座標
#define PUNISHER_POS_P CVector(0.0f,0.0f, 300.0f)	// Zがプラス方向の場合
#define PUNISHER_POS_M CVector(0.0f,0.0f,-300.0f)	// Zがマイナス方向の場合
// お仕置き用の敵の生成時の方向
#define PUNISHER_ROT_P CVector(0.0f,180.0f,0.0f)	// Zがプラス方向の場合
#define PUNISHER_ROT_M CVector(0.0f,  0.0f,0.0f)	// Zがマイナス方向の場合

// 出現までの時間
#define PUNISHER_POP_TIME 3.0f

// 敵管理クラスのインスタンスを取得
CTrashEnemyManager* CTrashEnemyManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CTrashEnemyManager::CTrashEnemyManager()
	: mElapsedTime(0.0f)
{
	spInstance = this;
	// 通常の敵の生成
	mpEnemy = new CTrashEnemy();
	mpEnemy->Position(ENEMY_POS);
	mpEnemy->Rotation(ENEMY_ROT);

	// お仕置き用の敵の生成
	mpPunisherEnemy = new CPunisherTrashEnemy();
	mpPunisherEnemy->Position(PUNISHER_POS_P);
	mpPunisherEnemy->Rotation(PUNISHER_ROT_P);
	// 最初は無効
	mpPunisherEnemy->SetEnable(false);
	mpPunisherEnemy->SetShow(false);
}

// デストラクタ
CTrashEnemyManager::~CTrashEnemyManager()
{
	SAFE_DELETE(spInstance);
}

// 更新
void CTrashEnemyManager::Update()
{
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	// エリア外の場合
	if (player->AreaOutZ())
	{
		mElapsedTime += Times::DeltaTime();
	}
	// エリア内の場合
	else
	{
		mElapsedTime = 0.0f;
	}

	// 出現までの時間が経過かつ、
	// 既にお仕置き用の敵が出現していない場合
	if (mElapsedTime >= PUNISHER_POP_TIME &&
		!GetPopPunisherEnemy())
	{
		// お仕置き用の敵が出現
		PopPunisherEnemy();
		mElapsedTime = 0.0f;
	}
}

// お仕置き用の敵が出現しているか
bool CTrashEnemyManager::GetPopPunisherEnemy()
{
	return mpPunisherEnemy->IsEnable();
}

// お仕置き用の敵を出現させる
void CTrashEnemyManager::PopPunisherEnemy()
{
	// プレイヤー取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	CVector playerPos = player->Position();
	// プレイヤーがプラス方向にいるなら
	if (playerPos.Z() >= ROAD_Z_AREA)
	{
		mpPunisherEnemy->Position(PUNISHER_POS_P);
		mpPunisherEnemy->Rotation(PUNISHER_ROT_P);
	}
	// マイナス方向にいるなら
	else
	{
		mpPunisherEnemy->Position(PUNISHER_POS_M);
		mpPunisherEnemy->Rotation(PUNISHER_ROT_M);
	}
	// 有効にする
	mpPunisherEnemy->SetEnable(true);
	mpPunisherEnemy->SetShow(true);
}
