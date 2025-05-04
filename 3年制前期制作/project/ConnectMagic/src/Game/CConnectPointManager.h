#pragma once

class CConnectPoint;

// 接続部の管理クラス
class CConnectPointManager : CTask
{
public:
	// インスタンスを取得
	static CConnectPointManager* Instance();

	// コンストラクタ
	CConnectPointManager();
	// デストラクタ
	~CConnectPointManager();

	// 更新
	void Update();

	// 描画
	void Render();

	/// <summary>
	/// 視点からのレイと設定されているコライダーとの衝突判定を行う
	/// </summary>
	/// <param name="hitPos">衝突した一番近い位置</param>
	/// <returns>衝突したか</returns>
	bool Ray(CVector& hitPos);

	/// <summary>
	/// 視点からターゲットまでのレイと設定されているコライダーとの衝突判定を行う
	/// </summary>
	/// <param name="targetPos">ターゲットの座標</param>
	/// <returns>衝突したか</returns>
	bool RayTarget(CVector targetPos);

	// 衝突判定を行うコライダーをリストに追加
	void AddCollider(CCollider* col);
	// 衝突判定を行うコライダーをリストから取り除く
	void RemoveCollider(CCollider* col);

	// 接続部を生成
	void CreateConnectPoint(CVector createPos);

	/// <summary>
	/// 接続部を消去
	/// </summary>
	/// <param name="num">消去する要素番号</param>
	void DeleteConnectPoint(int num);

	// 最後の要素を消去する
	void DeleteLastConnectPoint();

	// 杖の接続部の有効無効を設定
	void SetWandConnect(bool isOnOff);
	// 杖が接続されているか
	bool GetWandConnect();
private:
	// インスタンス
	static CConnectPointManager* spInstance;

	// 接続部のリスト
	std::vector<CConnectPoint*> mPoints;
	// 杖用の接続部
	CConnectPoint* mpPoint;

	// 視点方向のレイで衝突判定するコライダーのリスト
	std::list<CCollider*> mColliders;

	// 接続できる数の最大値
	int mConnectMaxNum;
};