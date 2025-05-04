#pragma once
#include <array>

class CDesk;
class CClassRoom;

// 机の行と列の数
#define DESK_ROW 5
#define DESK_COL 5

// フィールド管理クラス
class CFieldManager
{
public:
	// フィールド管理クラスのインスタンスを取得
	static CFieldManager* Instance();
	// コンストラクタ
	CFieldManager();
	// デストラクタ
	~CFieldManager();

	// 地面のコライダ―取得
	CCollider* GetGroundCol();
	// 壁のコライダ―取得
	CCollider* GetWallCol();
	// オブジェクトのコライダ―取得
	CCollider* GetObjCol();

	// リスポーン地点座標を取得
	CVector GetSpawnPos();

private:
	// フィールド管理クラスのインスタンス
	static CFieldManager* spInstance;

	// プレイヤーのスポーン地点にする行列
	CVector2 mSpawnColRow;

	// 机のリスト
	std::vector<CDesk*> mpDesks;

	// 机の配置の行列
	// 0＝生成しない
	// 1＝生成予定
	// 2＝生成済み
	int mMap[DESK_ROW][DESK_COL] = {};

	// 教室
	CClassRoom* mpClassRoom;

	// マップを生成
	void CreateField();
	// 机を生成

	/// <summary>
	/// 机を生成
	/// </summary>
	/// <param name="row">行</param>
	/// <param name="col">列</param>
	void CreateDesk(int row, int col);
	// リストの範囲内か
	bool InList(int row, int col);
};