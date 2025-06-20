#pragma once
#include "CEnemyStateBase.h"

// 敵の待機状態クラス
class CEnemyIdleState : public CEnemyStateBase
{
public:
	// インスタンス
	static CEnemyIdleState* Instance();

	// コンストラクタ
	CEnemyIdleState();
	// デストラクタ
	~CEnemyIdleState();

	// 開始時の処理
	void Start(CEnemy* enemy);
	// 更新中の処理
	void Update(CEnemy* enemy);
	// 終了時の処理
	void End(CEnemy* enemy);
	// 名前を取得
	std::string GetName() const;
};