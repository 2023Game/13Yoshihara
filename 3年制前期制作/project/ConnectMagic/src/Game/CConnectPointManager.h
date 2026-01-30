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
	/// 視点からターゲットまでのレイと地形の衝突判定を行う
	/// </summary>
	/// <param name="targetPos">ターゲットの座標</param>
	/// <returns>衝突したか</returns>
	bool RayTarget(CVector targetPos);

	/// <summary>
	/// 接続部と繋いだレイと設定されているコライダーとの衝突判定を行い
	/// 衝突していたら無効
	/// </summary>
	void RayPoint();

	// 接続部との距離が最大値より遠ければ接続を無効にする
	void FarDist();

	// 接続を有効
	void EnableConnect(CConnectTarget* connectTarget);

	// 接続を無効
	void DisableConnect(CConnectTarget* connectTarget);

	// 杖と接続中のターゲットを設定
	void SetConnectWandTarget(CConnectTarget* connectTarget);
	// 杖と接続中のターゲットを取得
	CConnectTarget* GetConnectWandTarget();

	// 接続しているもの同士の距離を設定
	void SetConnectDistance();
	void SetConnectDistance(const float& dist);
	// 接続しているもの同士の距離を取得
	float GetConnectDistance();

	// 杖と接続しているオブジェクトが空中の接続オブジェクトか
	bool IsWandConnectAirObject();

	// 杖の先端の座標を取得
	CVector GetWandPointPos() const;

private:
	// 杖の接続部の有効無効を設定
	void SetWandConnect(bool isOnOff, CConnectTarget* target);
	// 杖が接続されているか
	bool GetWandConnect();

	// 現在の杖が接続している接続部とプレイヤーの距離を取得
	float GetNowConnectDistance();

	// インスタンス
	static CConnectPointManager* spInstance;

	// 接続先の接続部
	CConnectPoint* mpConnectPoint1;
	CConnectPoint* mpConnectPoint2;
	// 杖用の接続部
	CConnectPoint* mpWandPoint;
	// 杖と接続中のターゲット
	CConnectTarget* mpConnectWandTarget;

	// 杖の先の接続部の位置を特定
	void WandPos();

	// 杖が接続している接続部とプレイヤーの距離
	float mConnectDistance;
};