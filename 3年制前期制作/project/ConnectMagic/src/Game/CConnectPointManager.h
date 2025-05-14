#pragma once

class CConnectPoint;
class CConnectTarget;

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

	// 引っ張る処理を実行
	void Pull();

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

	/// <summary>
	/// 接続部同士を繋いだレイと設定されているコライダーとの衝突判定を行い
	/// 衝突していたら削除する
	/// </summary>
	void RayPoint();

	// 衝突判定を行うコライダーをリストに追加
	void AddCollider(CCollider* col);
	// 衝突判定を行うコライダーをリストから取り除く
	void RemoveCollider(CCollider* col);
	// 衝突判定を行うコライダーをリセット
	void ResetCollider();

	// 接続部を生成
	void CreateConnectPoint(CConnectTarget* connectTarget);

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

	// 杖と接続中のターゲットを設定
	void SetConnectWandTarget(CConnectTarget* connectTarget);
	// 杖と接続中のターゲットを取得
	CConnectTarget* GetConnectWandTarget();

	// 杖が接続している接続部とプレイヤーの距離を設定
	void SetWandConnectDistance();
	// 杖が接続している接続部とプレイヤーの距離を取得
	float GetWandConnectDistance();

	/// <summary>
	/// 杖が接続している接続部とプレイヤーの距離を設定
	/// </summary>
	/// <param name="sign">符号</param>
	void SetWandConnectDistance(int sign);

	// 杖と接続しているオブジェクトが空中の接続オブジェクトか
	bool IsWandConnectAirObject();

private:
	// インスタンス
	static CConnectPointManager* spInstance;

	// 接続部のリスト
	std::vector<CConnectPoint*> mPoints;
	// 杖用の接続部
	CConnectPoint* mpPoint;
	// 杖と接続中のターゲット
	CConnectTarget* mpConnectWandTarget;

	// 視点方向のレイで衝突判定するコライダーのリスト
	std::list<CCollider*> mColliders;

	// 接続できる数の最大値
	int mConnectMaxNum;

	// 杖の先の接続部の位置を特定
	void WandPos();

	// 杖が接続している接続部とプレイヤーの距離
	float mWandConnectDistance;
};