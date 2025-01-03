#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include <assert.h>
#include "CFieldBase.h"
#include "CVehicleManager.h"

CNavManager* CNavManager::spInstance = nullptr;

// 経路管理クラスのインスタンスを取得
CNavManager* CNavManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CNavManager::CNavManager()
	: CTask(ETaskPriority::eNavigation, 0, ETaskPauseType::eGame)
	, mIsRender(false)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// デストラクタ
CNavManager::~CNavManager()
{
	spInstance = nullptr;

	// 探索ノードを全て削除
	auto itr = mNodes.begin();
	auto end = mNodes.end();
	while (itr != end)
	{
		CNavNode* del = *itr;
		itr = mNodes.erase(itr);
		delete del;
	}
}

// 経路探索用のノードを追加
void CNavManager::AddNode(CNavNode* node)
{
	mNodes.push_back(node);
}

// 経路探索用のノードを取り除く
void CNavManager::RemoveNode(CNavNode* node)
{
	mNodes.remove(node);
}

// 指定したノードに接続できるノードを検索して設定
int CNavManager::FindConnectNavNodes(CNavNode* node, float distance)
{
	// 現在の接続先の情報をすべてクリアしておく
	node->ClearConnects();

	for (CNavNode* findNode : mNodes)
	{
		// 自分自身であれば、スルー
		if (findNode == node) continue;

		// 指定された距離の限界値を超える場合は、スルー
		float dist = (findNode->GetPos() - node->GetPos()).Length();
		if (dist > distance) continue;

		// フィールドと車両とのレイ判定で遮蔽物チェックを行う
		CVector start = node->GetOffsetPos();
		CVector end = findNode->GetOffsetPos();
		CHitInfo hit;

		CFieldBase* fieldBase = CFieldBase::Instance();
		CVehicleManager* vehicleMgr = CVehicleManager::Instance();

		// フィールドがあるなら衝突判定をする
		if (fieldBase != nullptr)
		{
			if (fieldBase->CollisionRay(start, end, &hit))
			{
				// 何かにヒットした場合は、遮蔽物があるので接続できない
				continue;
			}
		}
		// 車両管理クラスがあるなら衝突判定をする
		if (vehicleMgr != nullptr)
		{
			if (vehicleMgr->CollisionRay(start, end, &hit))
			{
				// 何かにヒットした場合は、遮蔽物があるので接続できない
				continue;
			}
		}

		// 両方の条件を満たしたノードを接続リストに追加
		node->AddConnect(findNode);
	}

	return node->mConnectData.size();
}

// 全てのノードと経路を描画
void CNavManager::Render()
{
	// Nキーで経路探索ノードの描画モードを切り替え
	if (CInput::PushKey('N'))
	{
		mIsRender = !mIsRender;
	}

	// 経路探索ノードを描画しないならば、以降処理しない
	if (!mIsRender) return;

	// リスト内のノードを全て描画
	for (CNavNode* node : mNodes)
	{
		node->Render();
	}
}