#pragma once

class CNavManager;
class CNavNode;

// 経路探索ノードの接続情報
class CNavConnectData
{
public:
	CNavNode* node;	// 繋がっているノード
	float cost;		// 繋がっているノードまでの移動コスト
	CNavConnectData(CNavNode* n, float c)
		: node(n), cost(c) {}
};
// 経路探索ノードクラス
class CNavNode
{
	friend CNavManager;

public:
	// コンストラクタ
	CNavNode(const CVector& pos, bool isDestNode = false);
	// デストラクタ
	~CNavNode();

	// ノードの座標を取得
	const CVector& GetPos() const;
	// 遮蔽物チェックや表示用に、少し上に上げたノードの座標を変えす
	CVector GetOffsetPos() const;
	// ノードの座標を設定
	void SetPos(const CVector& pos);

	/// <summary>
	/// ノードが有効かどうかを取得
	/// </summary>
	/// <returns>trueならば、有効</returns>
	bool IsEnable() const;
	/// <summary>
	/// ノードが有効かどうかを設定
	/// </summary>
	/// <param name="isEnable">有効かどうか</param>
	void SetEnable(bool isEnable);

	// 接続するノードを追加
	void AddConnect(CNavNode* node);
	// 接続しているノードを取り除く
	void RemoveConnect(CNavNode* node);
	// 接続している全てのノードを解除
	void ClearConnects();

	// ノードの色設定（デバッグ用）
	void SetColor(const CColor& color);
	// ノードを描画（デバッグ用）
	void Render();

private:
	// 最短経路計算用のデータをリセット
	void ResetCalcData();
	// 経路探索時に経由できない目的地専用のノードかどうか
	bool mIsDestNode;
	CVector mPosition;	// ノードの座標
	// 接続しているノードの情報リスト
	std::list<CNavConnectData> mConnectData;

	// 最短経路計算時に記憶しておく情報
	float mCalcMoveCost;		// 開始ノードからこのノードまでにかかった移動コスト
	CNavNode* mpCalcFromNode;	// 最短経路での前のノードのポインタ

	bool mIsEnable;	// 有効かどうか

	// デバッグ関連
	CColor mColor;		// ノードの色
};