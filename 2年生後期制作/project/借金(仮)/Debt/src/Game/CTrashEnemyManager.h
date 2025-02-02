#pragma once
#include "CEnemy.h"

class CTrashEnemyManager
{
public:
	CTrashEnemyManager();
	~CTrashEnemyManager();

	// 敵を生成
	void CreateEnemy();
private:
	std::vector<CEnemy*> mEnemies	// 敵のリスト
};