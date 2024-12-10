#pragma once
#include "CEnemy.h"

class CTrashEnemyManager
{
public:
	CTrashEnemyManager();
	~CTrashEnemyManager();

	// “G‚ğ¶¬
	void CreateEnemy();
private:
	std::vector<CEnemy*> mEnemies	// “G‚ÌƒŠƒXƒg
};