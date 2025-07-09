#pragma once
#include "CEnemy.h"

// 敵の管理クラス
class CEnemyManager
{
public:
	// インスタンスのポインタの取得
	static CEnemyManager* Instance();

	// コンストラクタ
	CEnemyManager();
	// デストラクタ
	~CEnemyManager();

	// 更新
	void Update();

	// 敵の数を増加
	void AddEnemy();
	// 敵を削除
	void DeleteEnemy(CEnemy* enemy);

	// ターゲット候補を取得
	std::vector<CEnemy*> GetTargets();

private:
	// 敵の管理クラスのインスタンス
	static CEnemyManager* spInstance;

	// 敵の出現位置を設定
	void SetPopPos();

	// 敵の最大数
	int mEnemyMaxNum;
	// 一回で出現する敵の数
	int mEnemyOnceNum;
	// ウェーブ数
	int mWaveNum;
	// 時間経過
	float mElapsedTime;

	// 敵のリスト
	std::vector<CEnemy*> mEnemys;
	// 敵の出現位置
	std::vector<CVector> mPopPos;

	// ロックオンターゲットの候補
	std::vector<CEnemy*> mTargets;
};