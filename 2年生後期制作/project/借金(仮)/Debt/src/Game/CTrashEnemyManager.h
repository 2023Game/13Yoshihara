#pragma once

class CTrashEnemy;
class CPunisherTrashEnemy;

// ゴミ拾いの敵管理クラス
class CTrashEnemyManager
{
public:
	// 敵管理クラスのインスタンスを取得
	static CTrashEnemyManager* Instance();
	// コンストラクタ
	CTrashEnemyManager();
	// デストラクタ
	~CTrashEnemyManager();

	// 更新
	void Update();

	// お仕置き用の敵が出現しているか
	bool GetPopPunisherEnemy();
	// お仕置き用の敵を出現させる
	void PopPunisherEnemy();

private:
	// ゴミ拾いの敵管理クラスのインスタンスへのポインタ
	static CTrashEnemyManager* spInstance;

	// 通常の敵
	CTrashEnemy* mpEnemy;
	// お仕置き用の敵
	CPunisherTrashEnemy* mpPunisherEnemy;

	// 経過時間
	float mElapsedTime;
};