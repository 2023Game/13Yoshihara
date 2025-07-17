#include "CTestField.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CBox.h"
#include "CAirConnectObj.h"
#include "CPortal.h"
#include "CRespawnArea.h"
#include "CGrass.h"
#include "CTorch.h"
#include "CCampFire.h"
#include "CBridge.h"
#include "CUpgradeItem.h"
#include "CWater.h"
#include "CPortalFragment.h"
#include "CFragmentMergeArea.h"
#include "CDownWall.h"

// ポータルの設定
#define PORTAL_POS	CVector(100.0f, 10.0f, -100.0f)

// リスポーン地点の設定
#define RESPAWN_POS_1	CVector(  0.0f,-5.0f,-180.0f)
#define RESPAWN_POS_2	CVector(-90.0f,35.0f,-430.0f)
#define RESPAWN_RADIUS	25.0f

// 松明の座標
#define TORCH_POS_1 CVector(0.0f,0.0f,-50.0f)
#define TORCH_POS_2 CVector(10.0f,0.0f,-50.0f)

// 草の座標
#define GRASS_POS_1 CVector(-50.0f,0.0f,-50.0f)
#define GRASS_POS_2 CVector(-100.0f,0.0f,-50.0f)

// ツタの炎のオフセット座標
#define VINE_FIRE_OFFSET_POS CVector(0.0f,VINE_HEIGHT/2.0f,0.0f)
// ツタの炎の大きさ
#define VINE_FIRE_SCALE FIRE_SCALE * 15.0f

// 橋の座標
#define BRIDGE_POS_1 CVector(50.0f,0.0f,-50.0f)
#define BRIDGE_POS_2 CVector(50.0f,0.0f,-100.0f)
// 橋の回転
#define BRIDGE_ROT_2 CVector(0.0f,180.0f,0.0f)

// 箱の座標
#define BOX_POS CVector(0.0f,0.0f,-25.0f)

// 水の座標
#define WATER_POS CVector(300.0f,1.0f,0.0f)

// アイテムの座標
#define ITEM_POS CVector(0.0f,5.0f,30.0f)

// キャンプファイヤーの座標
#define CAMPFIRE_POS CVector(0.0f,0.0f,-70.0f)

// ポータルの欠片の数
#define PORTAL_FRAGMENT_NUM 4

// 下がる壁の座標
#define DOWN_WALL_POS CVector(150.0f,0.0f,50.0f)
// スイッチの座標
#define SWITCH_POS CVector(0.0f,0.0f,50.0f)

// コンストラクタ
CTestField::CTestField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TestField");

	// オブジェクトを生成
	CreateFieldObjects();
	// コライダ―を生成
	CreateCol("TestField", "", "");
}

// デストラクタ
CTestField::~CTestField()
{
}

// フィールドのオブジェクトを生成
void CTestField::CreateFieldObjects()
{
	// 欠片の合成場所
	CFragmentMergeArea* mergeArea = new CFragmentMergeArea(PORTAL_FRAGMENT_NUM);
	mergeArea->Position(0.0f, 0.0f, 100.0f);

	// 強化アイテムを生成
	CUpgradeItem* item = new CUpgradeItem();
	item->Position(ITEM_POS);

	for (int i = 0; i < 10; i++)
	{
		item = new CUpgradeItem();
		item->Position(10.0f + 10.0f * i, 5.0f, 0.0f);
	}

	// 水を生成
	CWater* water = new CWater(CVector(100.0f,1.0f,100.0f));
	water->Position(WATER_POS);

	// 箱を生成
	CBox* box = new CBox();
	box->Position(BOX_POS);

	// 橋を生成
	CBridge* bridge = new CBridge();
	// 位置設定
	bridge->Position(BRIDGE_POS_1);

	// 橋を生成
	bridge = new CBridge();
	// 位置設定
	bridge->Position(BRIDGE_POS_2);
	// 回転設定
	bridge->Rotation(BRIDGE_ROT_2);

	// 草を生成
	CGrass* grass = new CGrass();
	// 位置調整
	grass->Position(GRASS_POS_1);
	grass->SetDropItem(mergeArea->GetFragment(3));

	// 草を生成
	grass = new CGrass(EGrassType::eVine, VINE_FIRE_OFFSET_POS, VINE_FIRE_SCALE);
	// 位置調整
	grass->Position(GRASS_POS_2);

	// 松明を生成
	CTorch* torch = new CTorch();
	// 位置調整
	torch->Position(TORCH_POS_1);
	torch->SetFire(true);

	// 松明を生成
	torch = new CTorch();
	// 位置調整
	torch->Position(TORCH_POS_2);

	// キャンプファイヤー
	CCampFire* campFire = new CCampFire();
	campFire->Position(CAMPFIRE_POS);
	campFire->SetDropItem(mergeArea->GetFragment(0));

	campFire = new CCampFire();
	campFire->Position(0.0f,0.0f,-80.0f);
	campFire->SetDropItem(mergeArea->GetFragment(1));

	campFire = new CCampFire();
	campFire->Position(0.0f,0.0f,-100.0f);
	campFire->SetDropItem(mergeArea->GetFragment(2));

	// ポータルを生成
	CPortal* portal = new CPortal();
	// 位置調整
	portal->Position(PORTAL_POS);

	// リスポーン地点を生成
	new CRespawnArea(RESPAWN_POS_1, RESPAWN_RADIUS);
	new CRespawnArea(RESPAWN_POS_2, RESPAWN_RADIUS);

	// 下がる壁を生成
	CDownWall* downWall = new CDownWall(DOWN_WALL_POS);
	
	// スイッチを生成
	CSwitch* switchObj = new CSwitch(SWITCH_POS);
	// 作用するオブジェクトに下がる壁を設定
	switchObj->SetActionObj(downWall);

	CBox* bigBox = new CBox(9.0f);
	bigBox->Position(0.0f, 0.0f, 100.0f);
}
