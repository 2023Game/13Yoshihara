#include "CMap1.h"
#include "CWater.h"
#include "CBox.h"
#include "CUpgradeItem.h"
#include "Maths.h"
#include "CSwitch.h"

// 水面の座標
#define WATER_POS CVector(-250.0f,-10.0f,50.0f)
// 水面のスケール
#define WATER_SCALE CVector(500.0f,1.0f,300.0f)

// 最初に配置するアイテムの座標
#define ITEM_POS_1	CVector(0.0f,0.0f,50.0f)
#define ITEM_POS_2	CVector(0.0f,0.0f,270.0f)
#define ITEM_POS_3	CVector(-270.0f,0.0f,90.0f)

// 箱の座標
#define BOX_POS_1 CVector(-125.0f,100.0f,85.0f)
#define BOX_POS_2 CVector(61.5f,0.0f,413.0f)
#define BOX_POS_3 CVector(161.5f,0.0f,523.0f)
// でかい箱の座標
#define BIG_BOX_POS CVector(177.5,0.0f,128.0f)
// デカい箱のスケール倍率
#define BIG_BOX_SCALE 9.0f

// 下がる壁の座標
#define DOWN_WALL_POS_1 CVector(116.5f,0.0f,128.0f)
#define DOWN_WALL_POS_2 CVector(-225.5f,0.0f,468.0f)
// スイッチの座標
#define SWITCH_POS CVector(-70.0f,0.0f,50.0f)
// 3連スイッチの座標
#define SWITCHS_POS_1 CVector(-150.0f,0.0f,518.0f)
#define SWITCHS_POS_2 CVector(-150.0f,0.0f,468.0f)
#define SWITCHS_POS_3 CVector(-150.0f,0.0f,418.0f)

// コンストラクタ
CMap1::CMap1()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("Map1");

	// コライダーを生成
	CreateCol("Map1_Ground_Col", "Map1_Wall_Col", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CMap1::~CMap1()
{
}

// フィールドオブジェクトを生成
void CMap1::CreateFieldObjects()
{
	// 箱を生成
	CBox* box = new CBox(BOX_POS_1);
	// 箱を生成
	box = new CBox(BOX_POS_2);
	// 箱を生成
	box = new CBox(BOX_POS_3);

	// デカい箱を生成
	CBox* bigBox = new CBox(BIG_BOX_POS, BIG_BOX_SCALE);

	//// スイッチを生成
	//std::vector<CSwitch*> switchs = { new CSwitch(SWITCH_POS) };

	//// 下がる壁を生成
	//CDownWall* downWall = new CDownWall(DOWN_WALL_POS_1, switchs);

	//int size = switchs.size();
	//for (int i = 0; i < size; i++)
	//{
	//	// 作用するオブジェクトに下がる壁を設定
	//	switchs[i]->SetActionObj(downWall);
	//}

	//// スイッチを生成
	//switchs =
	//{ new CSwitch(SWITCHS_POS_1), new CSwitch(SWITCHS_POS_2), new CSwitch(SWITCHS_POS_3) };

	//// 下がる壁を生成
	//downWall = new CDownWall(DOWN_WALL_POS_2, switchs);

	//size = switchs.size();
	//for (int i = 0; i < size; i++)
	//{
	//	// 作用するオブジェクトに下がる壁を設定
	//	switchs[i]->SetActionObj(downWall);
	//}
}

// 経路探索用のノードを生成
void CMap1::CreateNavNodes()
{
}
