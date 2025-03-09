#pragma once
#include "CCollector.h"

class CPunisherCollector : public CCollector
{
public:
	// コンストラクタ
	CPunisherCollector(CObjectBase* owner,
		std::vector<CNavNode*> patrolPoints);
	// デストラクタ
	~CPunisherCollector();

private:
	// 追跡状態への移行の条件をチェック
	bool ChangeChase() override;
	// 追跡状態から他の状態へ移行の条件をチェック
	bool ChangeChaseToOther() override;
};