#pragma once
#include "CEnemyStateBase.h"

// 敵の詠唱中状態クラス
class CEnemyCastState : public CEnemyStateBase
{
public:
	// インスタンス
	static CEnemyCastState* Instance();

	// コンストラクタ
	CEnemyCastState();
	// デストラクタ
	~CEnemyCastState();

	// 開始時の処理
	void Start(CEnemy* enemy);
	// 更新中の処理
	void Update(CEnemy* enemy);
	// 終了時の処理
	void End(CEnemy* enemy);
	// 名前を取得
	std::string GetName() const;
};