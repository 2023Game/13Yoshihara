#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

// ノードのY座標のオフセット値
#define NODE_OFFSET_Y 5.0f
// 探すノードの距離の限界値
#define FIND_NODE_DISTANCE 70.0f

// コンストラクタ
CNavNode::CNavNode(const CVector& pos, bool isDestNode)
	: mIsDestNode(isDestNode)
	, mPosition(pos)
	, mIsEnable(true)
	, mCalcMoveCost(-1.0f)
	, mpCalcFromNode(nullptr)
	, mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
	// 管理クラスのリストに自身を追加
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->AddNode(this);
	}
	// 座標を設定
	SetPos(mPosition);
}

// デストラクタ
CNavNode::~CNavNode()
{
	// 管理クラスのリストから自信を取り除く
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->RemoveNode(this);
	}
}

// 最短経路計算用のデータをリセット
void CNavNode::ResetCalcData()
{
	mCalcMoveCost = -1.0f;
	mpCalcFromNode = nullptr;
}

// ノードの座標を取得
const CVector& CNavNode::GetPos() const
{
	return mPosition;
}

// 遮蔽物チェックや表示用に、少し上に上げたノードの座標を変えす
CVector CNavNode::GetOffsetPos() const
{
	// 地面と重ならないように、ノードの位置から少し上にあげた位置を返す
	return mPosition + CVector(0.0f, NODE_OFFSET_Y, 0.0f);
}

// ノードの座標を設定
void CNavNode::SetPos(const CVector& pos)
{
	// ノードの座標を更新
	mPosition = pos;

	// ノードの座標が変わったので、接続しているノードを調べ直す
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->FindConnectNavNodes(this, FIND_NODE_DISTANCE);
	}
}

// ノードが有効かどうかを取得
bool CNavNode::IsEnable()
{
	return mIsEnable;
}

// ノードが有効かどうかを設定
void CNavNode::SetEnable(bool isEnable)
{
	// 同じなら処理をしない
	if (mIsEnable == isEnable) return;
	// 無効になるなら接続している全てのノードを解除
	if (isEnable == false)
	{
		ClearConnects();
	}

	mIsEnable = isEnable;
}

// 接続するノードを追加
void CNavNode::AddConnect(CNavNode* node)
{
	// 既に接続リストに登録してあるノードであれば、スルー
	for (CNavConnectData& connect : mConnectData)
	{
		if (connect.node == node) return;
	}

	// 接続するノードまでの距離をコストとする
	float cost = (node->GetPos() - mPosition).Length();

	// 自信と相手それぞれの接続しているノードリストにお互いを設定
	mConnectData.push_back(CNavConnectData(node, cost));
	node->mConnectData.push_back(CNavConnectData(this, cost));
}

// 接続しているノードを取り除く
void CNavNode::RemoveConnect(CNavNode* node)
{
	auto itr = mConnectData.begin();
	auto end = mConnectData.end();
	while (itr != end)
	{
		// 一致するノードが見つかれば、リストから取り除く
		if (itr->node == node)
		{
			itr = mConnectData.erase(itr);
			continue;
		}
		itr++;
	}
}

// 接続している全てのノードを解除
void CNavNode::ClearConnects()
{
	// 接続相手の接続リストから自信を取り除く
	for (CNavConnectData& connect : mConnectData)
	{
		connect.node->RemoveConnect(this);
	}
	// 自身の接続リストをクリア
	mConnectData.clear();
}

// ノードの色設定（デバッグ用）
void CNavNode::SetColor(const CColor& color)
{
	mColor.Set(color.R(), color.G(), color.B());
}

// ノードを描画（デバッグ用）
void CNavNode::Render()
{
	// 有効なら描画
	if (mIsEnable)
	{
		// 接続先のノードまでのラインを描画
		for (CNavConnectData& connect : mConnectData)
		{
			Primitive::DrawLine
			(
				GetOffsetPos(),
				connect.node->GetOffsetPos(),
				CColor(0.11f, 0.1f, 0.1f, 1.0f),
				2.0f
			);
		}
		// ノードの座標に球を描画
		CMatrix m;
		m.Translate(GetOffsetPos());
		Primitive::DrawWireSphere(m, 1.0f, mColor);
	}
}
