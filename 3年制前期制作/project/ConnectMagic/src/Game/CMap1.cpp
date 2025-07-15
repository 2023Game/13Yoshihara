#include "CMap1.h"
#include "CWater.h"
#include "CBox.h"
#include "CUpgradeItem.h"
#include "Maths.h"

// 水面の座標
#define WATER_POS CVector(-250.0f,-10.0f,50.0f)
// 水面のスケール
#define WATER_SCALE CVector(500.0f,1.0f,300.0f)

// 最初に配置するアイテムの座標
#define ITEM_POS_1	CVector(0.0f,0.0f,50.0f)
#define ITEM_POS_2	CVector(0.0f,0.0f,270.0f)
#define ITEM_POS_3	CVector(-270.0f,0.0f,90.0f)

// 箱の座標
#define BOX_POS_1 CVector(-65.0f,100.0f,85.0f)
#define BOX_POS_2 CVector(-85.0f,20.0f,85.0f)
#define BOX_POS_3 CVector(-65.0f,60.0f,185.0f)

// コンストラクタ
CMap1::CMap1()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("Map1");

	// コライダーを生成
	CreateCol("", "", "Map1");

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
	CBox* box = new CBox();
	// 座標を設定
	box->Position(BOX_POS_1);
	// 箱を生成
	box = new CBox();
	// 座標を設定
	box->Position(BOX_POS_2);
	// 箱を生成
	box = new CBox();
	// 座標を設定
	box->Position(BOX_POS_3);
}

// 経路探索用のノードを生成
void CMap1::CreateNavNodes()
{
}
