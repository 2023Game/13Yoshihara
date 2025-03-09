#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include <assert.h>
#include "CFieldBase.h"
#include "CTrashVehicleManager.h"
#include "Primitive.h"

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
		// 無効であれば、スルー
		if (!findNode->IsEnable()) continue;

		// 目的地専用ノードは距離を考慮しない
		if (!node->mIsDestNode)
		{
			// 指定された距離の限界値を超える場合は、スルー
			float dist = (findNode->GetPos() - node->GetPos()).Length();
			if (dist > distance) continue;
		}
		// フィールドと車両とのレイ判定で遮蔽物チェックを行う
		CVector start = node->GetOffsetPos();
		CVector end = findNode->GetOffsetPos();
		CHitInfo hit;
		bool isHit = false;

		CFieldBase* fieldBase = CFieldBase::Instance();
		CTrashVehicleManager* vehicleMgr = CTrashVehicleManager::Instance();

		// フィールドがあるなら衝突判定をする
		if (fieldBase != nullptr)
		{
			if (fieldBase->CollisionRay(start, end, &hit))
			{
				isHit = true;
			}
		}
		// 車両管理クラスがあるなら衝突判定をする
		if (vehicleMgr != nullptr)
		{
			if (vehicleMgr->NavCollisionRay(start, end, &hit, isHit))
			{
				isHit = true;
			}
		}

		// 何かにヒットした場合は、遮蔽物があるので接続できない
		if (isHit)
		{
			continue;
		}

		// 両方の条件を満たしたノードを接続リストに追加
		node->AddConnect(findNode);
	}

	return node->mConnectData.size();
}

// 最短経路計算用のデータをリセット
void CNavManager::ResetCalcData()
{
	// 全てのノードの計算用のデータをリセット
	for (CNavNode* node : mNodes)
	{
		node->ResetCalcData();
	}
}

// 指定したノードから次のノードへの移動コストを計算
void CNavManager::CalcNextMoveCost(CNavNode* node, CNavNode* goal)
{
	// 指定されたノードまたは目的地ノードが空だった場合は、移動コスト計算不可
	if (node == nullptr || goal == nullptr) return;

	// 接続している全てのノードへの移動コストを求める
	for (CNavConnectData& connect : node->mConnectData)
	{
		if (!connect.node->IsEnable()) continue;
		// 接続しているノードが目的地専用ノードの場合は、
		// 今回の経路探索の目的地ノード以外は経由しないため、スルー
		if (connect.node->mIsDestNode && connect.node != goal) continue;

		// 移動コスト＝ここまでの移動コスト＋接続先のノードまでの移動コスト
		float cost = node->mCalcMoveCost + connect.cost;
		// 接続先のノードまでの移動コストがまだ計算されていないか、
		// 求めた移動コストの方が現在の接続先のノードまでの移動コストより小さい場合
		if (connect.node->mCalcMoveCost < 0 || cost < connect.node->mCalcMoveCost)
		{
			// 接続先のノードの移動コストと1つ前のノードのポインタを更新
			connect.node->mCalcMoveCost = cost;
			connect.node->mpCalcFromNode = node;

			// 接続先のノードが目的地のノードでなければ、
			// 接続先のノードから接続されているノードへの移動コストを計算する
			if (connect.node != goal)
			{
				CalcNextMoveCost(connect.node, goal);
			}
		}
	}
}


// 指定した開始ノードから目的地ノードまでの最短経路を求める
bool CNavManager::Navigate(CNavNode* start, CNavNode* goal, std::vector<CNavNode*>& route)
{
	// 開始ノードまたは目的地ノードが空だった場合は、経路探索不可
	if (start == nullptr || goal == nullptr) return false;
	// 開始ノードまたは目的地ノードが無効だった場合は、経路探索不可
	if (!start->IsEnable() || !goal->IsEnable()) return false;

	// 全てのノードの最短経路計算用のデータをクリア
	ResetCalcData();

	// 開始ノードへの移動コストを0に設定
	start->mCalcMoveCost = 0.0f;
	// 開始ノードから順番に、
	// 各ノードが接続しているノードまでの移動コストを計算
	CalcNextMoveCost(start, goal);

	// 最短経路のリストをクリアして、最初に目的地ノードを登録
	route.clear();
	route.push_back(goal);

	// 目的地ノードから順番に、
	// 最短経路での1つ前のノードが存在する限りループして、
	// 最短経路のリストにノードを登録していく
	CNavNode* node = goal;
	while (node->mpCalcFromNode != nullptr)
	{
		route.insert(route.begin(), node->mpCalcFromNode);
		node = node->mpCalcFromNode;
	}

	// 求めた最短経路をデバッグ表示用の最短経路リストにコピー
	mLastCalcRoute = route;

	// 求めた最短経路の1番最初のノードが開始ノードであれば、
	// 開始ノードから目的地ノードまで経路がつながっている
	return route[0] == start;
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

	// 最後に求めた最短経路にラインを引く
	glDisable(GL_DEPTH_TEST);
	int size = mLastCalcRoute.size();
	for (int i = 0; i < size - 1; i++)
	{
		CVector start = mLastCalcRoute[i]->GetOffsetPos();
		CVector end = mLastCalcRoute[i + 1]->GetOffsetPos();
		Primitive::DrawLine(start, end, CColor::cyan, 4.0f);
	}
	glEnable(GL_DEPTH_TEST);
}