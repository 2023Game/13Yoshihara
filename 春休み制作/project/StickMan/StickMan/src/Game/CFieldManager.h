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
private:
	// フィールド管理クラスのインスタンス
	static CFieldManager* spInstance;
	// 机のリスト
	std::vector<CDesk*> mpDeskList;
	// 机の配置の行列
	int mMap[DESK_ROW][DESK_COL] = {};
	// 教室
	CClassRoom* mpClassRoom;

	// マップを生成
	void CreateField();
	// リストの範囲内か
	bool InList(int row, int col);
};