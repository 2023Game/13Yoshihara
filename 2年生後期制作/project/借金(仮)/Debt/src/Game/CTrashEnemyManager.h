#pragma once
#include "CEnemy.h"

class CTrashEnemyManager
{
public:
	CTrashEnemyManager();
	~CTrashEnemyManager();

	// �G�𐶐�
	void CreateEnemy();
private:
	std::vector<CEnemy*> mEnemies	// �G�̃��X�g
};