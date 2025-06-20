#pragma once
#include "CEnemyStateBase.h"

// 敵の回避状態クラス
class CEnemyDodgeState : public CEnemyStateBase
{
public:
	// インスタンス
	static CEnemyDodgeState* Instance();

	// コンストラクタ
	CEnemyDodgeState();
	// デストラクタ
	~CEnemyDodgeState();

	// 開始時の処理
	void Start(CEnemy* enemy);
	// 更新中の処理
	void Update(CEnemy* enemy);
	// 終了時の処理
	void End(CEnemy* enemy);
	// 名前を取得
	std::string GetName() const;
};