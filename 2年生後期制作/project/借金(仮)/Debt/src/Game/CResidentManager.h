#pragma once

class CResident;
class CModelX;

// 住人の管理クラス
class CResidentManager
{
public:
	// インスタンスのポインタの取得
	static CResidentManager* Instance();
	// コンストラクタ
	CResidentManager();
	// デストラクタ
	~CResidentManager();
private:
	// 住人を全て生成
	void CreateResident();
	// 経路探索用のノードを作成
	void CreateNavNodes();
	// 住人管理クラスのインスタンス
	static CResidentManager* spInstance;

	// 住人のモデル
	CModelX* mpResidentModel;
	// 住人のリスト
	std::vector<CResident*> mpResidents;
	// 左の歩道の巡回ポイント
	std::vector<CNavNode*> mpPatrolPointsLeft;
	// 右の歩道の巡回ポイント
	std::vector<CNavNode*> mpPatrolPointsRight;

	// フィールド内にあるゴミ袋の数
	int mFieldBagNum;
};