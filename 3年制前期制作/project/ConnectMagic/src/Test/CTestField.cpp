#include "CTestField.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CBox.h"
#include "CAirConnectObj.h"
#include "CPortal.h"
#include "CRespawnArea.h"
#include "CGrass.h"
#include "CFire.h"
#include "CBridge.h"
#include "CUpgradeItem.h"
#include "CWater.h"
#include "CPortalFragment.h"
#include "CFragmentMergeArea.h"

// ポータルの設定
#define PORTAL_POS	CVector(100.0f, 0.0f + 5.0f * PORTAL_SIZE, -100.0f)
#define PORTAL_SIZE 2.0f

// リスポーン地点の設定
#define RESPAWN_POS_1	CVector(  0.0f,-5.0f,-180.0f)
#define RESPAWN_POS_2	CVector(-90.0f,35.0f,-430.0f)
#define RESPAWN_RADIUS	25.0f

// 松明の炎のオフセット座標
#define TORCH_FIRE_OFFSET_POS CVector(0.0f,9.0f,0.0f)
// 松明のオフセット座標
#define TORCH_OFFSET_POS_1 CVector(0.0f,0.0f,-50.0f)
#define TORCH_OFFSET_POS_2 CVector(10.0f,0.0f,-50.0f)

// 草のオフセット座標
#define GRASS_OFFSET_POS_1 CVector(-50.0f,0.0f,-50.0f)
#define GRASS_OFFSET_POS_2 CVector(-100.0f,0.0f,-50.0f)

// 橋のオフセット座標
#define BRIDGE_OFFSET_POS_1 CVector(50.0f,0.0f,-50.0f)
#define BRIDGE_OFFSET_POS_2 CVector(50.0f,0.0f,-100.0f)
// 橋のオフセット回転
#define BRIDGE_OFFSET_ROT_2 CVector(0.0f,180.0f,0.0f)

// 箱のオフセット座標
#define BOX_OFFSET_POS CVector(0.0f,0.0f,-25.0f)

// 水の座標
#define WATER_OFFSET_POS CVector(300.0f,1.0f,0.0f)

// アイテムの座標
#define ITEM_OFFSET_POS CVector(0.0f,10.0f,30.0f)

// キャンプファイヤーのオフセット座標
#define CAMPFIRE_OFFSET_POS CVector(0.0f,0.0f,-70.0f)
// 炎のオフセット座標
#define CAMPFIRE_FIRE_OFFSET_POS CVector(0.0f,3.0f,0.0f)
// 炎の大きさ
#define CAMPFIRE_FIRE_SCALE 2.5f
// 炎の移動速度
#define CAMPFIRE_FIRE_SPEED 10.0f
// 接続ターゲットのオフセット座標
#define CAMPFIRE_TARGET_OFFSET_POS CVector(0.0f,15.0f,0.0f)

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
	CFragmentMergeArea* mergeArea = new CFragmentMergeArea();
	mergeArea->Position(0.0f, 0.0f, 100.0f);
	
	// ポータルの欠片
	CPortalFragment* fragment = new CPortalFragment(EFragmentType::eFragment1);
	fragment->Position(20.0f, 0.0f, 50.0f);
	
	fragment = new CPortalFragment(EFragmentType::eFragment2);
	fragment = new CPortalFragment(EFragmentType::eFragment3);
	fragment = new CPortalFragment(EFragmentType::eFragment4);

	// 強化アイテムを生成
	CUpgradeItem* item = new CUpgradeItem();
	item->Position(ITEM_OFFSET_POS);

	// 水を生成
	CWater* water = new CWater(CVector(100.0f,1.0f,100.0f));
	water->Position(WATER_OFFSET_POS);

	// 箱を生成
	CBox* box = new CBox();
	box->Position(BOX_OFFSET_POS);

	// 橋を生成
	CBridge* bridge = new CBridge();
	// 位置設定
	bridge->Position(BRIDGE_OFFSET_POS_1);

	// 橋を生成
	bridge = new CBridge();
	// 位置設定
	bridge->Position(BRIDGE_OFFSET_POS_2);
	// 回転設定
	bridge->Rotation(BRIDGE_OFFSET_ROT_2);

	// 草を生成
	CGrass* grass = new CGrass();
	// 位置調整
	grass->Position(GRASS_OFFSET_POS_1);

	// 草を生成
	grass = new CGrass();
	// 位置調整
	grass->Position(GRASS_OFFSET_POS_2);

	// 松明を生成
	CFire* fire = new CFire("Torch", TORCH_FIRE_OFFSET_POS);
	// 位置調整
	fire->Position(TORCH_OFFSET_POS_1);
	fire->SetFire(true);

	// 松明を生成
	fire = new CFire("Torch", TORCH_FIRE_OFFSET_POS);
	// 位置調整
	fire->Position(TORCH_OFFSET_POS_2);

	// キャンプファイヤー
	fire = new CFire("CampFire", CAMPFIRE_FIRE_OFFSET_POS,
		CAMPFIRE_FIRE_SCALE, CAMPFIRE_FIRE_SPEED,
		CAMPFIRE_TARGET_OFFSET_POS);
	fire->Position(CAMPFIRE_OFFSET_POS);

	// ポータルを生成
	CPortal* portal = new CPortal();
	// 位置調整
	portal->Position(PORTAL_POS);
	// サイズ調整
	portal->SetSize(portal->GetSize() * PORTAL_SIZE);

	// リスポーン地点を生成
	new CRespawnArea(RESPAWN_POS_1, RESPAWN_RADIUS);
	new CRespawnArea(RESPAWN_POS_2, RESPAWN_RADIUS);
}
