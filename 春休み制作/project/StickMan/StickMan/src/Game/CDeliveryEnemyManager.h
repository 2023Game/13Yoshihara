#pragma once

class CDeliveryEnemy;

// 配達の敵管理クラス
class CDeliveryEnemyManager
{
public:
	// インスタンスを取得
	static CDeliveryEnemyManager* Instance();
	// コンストラクタ
	CDeliveryEnemyManager();
	// デストラクタ
	~CDeliveryEnemyManager();

	// 更新
	void Update();

	// 敵が出現しているか
	bool GetEnamyEnable();

private:
	// インスタンス
	static CDeliveryEnemyManager* spInstance;
	// ランダムな道に敵を生成
	void PopEnemyRandomRoad();
	// 敵
	CDeliveryEnemy* mpEnemy;
	// 経過時間
	float mElapsedTime;
};