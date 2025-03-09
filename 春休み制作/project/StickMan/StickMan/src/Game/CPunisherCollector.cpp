#include "CPunisherCollector.h"
#include "CTrashPlayer.h"

// コンストラクタ
CPunisherCollector::CPunisherCollector(CObjectBase* owner, std::vector<CNavNode*> patrolPoints)
	: CCollector(true, owner, patrolPoints)
{
}

// デストラクタ
CPunisherCollector::~CPunisherCollector()
{
}

// 追跡状態への移行の条件をチェック
bool CPunisherCollector::ChangeChase()
{
	// プレイヤーを取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// 回収員がくっついていない場合
	if (!player->GetStickCollector())
	{
		// 追跡状態へ移行
		ChangeState(EState::eChase);
		return true;
	}
	return false;
}

// 追跡状態から他の状態へ移行の条件をチェック
bool CPunisherCollector::ChangeChaseToOther()
{
	// プレイヤークラス取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// 既に回収員がくっついている場合
	// 追いかけるのをやめる
	if (player->GetStickCollector())
	{
		ChangeState(EState::eIdle);
		return true;
	}
	return false;
}