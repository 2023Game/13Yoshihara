#pragma once

class CConnectPoint;
class CConnectTarget;
class CConnectObject;

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
	void Connect();

	/// <summary>
	/// 視点からのレイと設定されているコライダーとの衝突判定を行う
	/// </summary>
	/// <param name="hitPos">衝突した一番近い位置</param>
	/// <returns>衝突したか</returns>
	bool Ray(CVector& hitPos);

	/// <summary>
	/// 2点を繋いだレイと設定されているコライダーとの衝突判定を行う
	/// </summary>
	/// <param name="start">始まりの点</param>
	/// <param name="end">終わりの点</param>
	/// <param name="hit">衝突情報</param>
	/// <returns></returns>
	bool Ray(const CVector& start, const CVector& end, CHitInfo* hit);

	/// <summary>
	/// 視点からターゲットまでのレイと設定されているコライダーとの衝突判定を行う
	/// </summary>
	/// <param name="targetPos">ターゲットの座標</param>
	/// <returns>衝突したか</returns>
	bool RayTarget(CVector targetPos);

	/// <summary>
	/// 接続部と繋いだレイと設定されているコライダーとの衝突判定を行い
	/// 衝突していたら無効
	/// </summary>
	void RayPoint();

	// 接続部との距離が最大値より遠いか
	// 最小値より近ければ接続を無効にする
	void FarOrNearDist();

	// 衝突判定を行うコライダーをリストに追加
	void AddCollider(CCollider* col);
	// 衝突判定を行うコライダーをリストから取り除く
	void RemoveCollider(CCollider* col);
	// 衝突判定を行うコライダーをリセット
	void ResetCollider();

	// 接続を有効
	void EnableConnect(CConnectTarget* connectTarget);

	// 接続を無効
	void DisableConnect(CConnectTarget* connectTarget);

	// 杖の接続部の有効無効を設定
	void SetWandConnect(bool isOnOff, CConnectTarget* target);
	// 杖が接続されているか
	bool GetWandConnect();

	// 杖と接続中のターゲットを設定
	void SetConnectWandTarget(CConnectTarget* connectTarget);
	// 杖と接続中のターゲットを取得
	CConnectTarget* GetConnectWandTarget();

	// 杖が接続している接続部とプレイヤーの距離を設定
	void SetConnectDistance();
	// 杖が接続している接続部とプレイヤーの距離を取得
	float GetConnectDistance();

	// 現在の杖が接続している接続部とプレイヤーの距離を取得
	float GetNowConnectDistance();

	// 杖と接続しているオブジェクトが空中の接続オブジェクトか
	bool IsWandConnectAirObject();

private:
	// インスタンス
	static CConnectPointManager* spInstance;

	// 接続先の接続部
	CConnectPoint* mpConnectPoint;
	// 杖用の接続部
	CConnectPoint* mpWandPoint;
	// 杖と接続中のターゲット
	CConnectTarget* mpConnectWandTarget;

	// 視点方向のレイで衝突判定するコライダーのリスト
	std::list<CCollider*> mColliders;

	// 杖の先の接続部の位置を特定
	void WandPos();

	// 杖が接続している接続部とプレイヤーの距離
	float mConnectDistance;
};