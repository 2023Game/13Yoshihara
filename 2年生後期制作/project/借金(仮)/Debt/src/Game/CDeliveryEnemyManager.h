#pragma once

class CDeliveryEnemy;

// 配達の敵管理クラス
class CDeliveryEnemyManager
{
public:
	// コンストラクタ
	CDeliveryEnemyManager();
	// デストラクタ
	~CDeliveryEnemyManager();

	// 更新
	void Update();

private:
	// ランダムな道に敵を生成
	void PopEnemyRandomRoad();
	// 敵
	CDeliveryEnemy* mpEnemy;
	// 経過時間
	float mElapsedTime;
};