#include "CTestField.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CWeight.h"
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
#include "CConnectRoom.h"
#include "CRoom1.h"
#include "CSwitchMoveFloor.h"
#include "CSwitchMoveAirObj.h"
#include "CSwitchPushWall.h"
#include "CSwitchShield.h"

// ポータルの設定
const CVector PORTAL_POS =				CVector( 100.0f,	 10.0f,				-100.0f);

// リスポーン地点の設定
const CVector RESPAWN_POS_1 =			CVector( 0.0f,		-5.0f,				-180.0f);
const CVector RESPAWN_POS_2 =			CVector(-90.0f,		 35.0f,				-430.0f);
constexpr float RESPAWN_RADIUS =		25.0f;

// 松明の座標
const CVector TORCH_POS_1 =				CVector( 0.0f,		 0.0f,				-50.0f);
const CVector TORCH_POS_2 =				CVector( 10.0f,		 0.0f,				-50.0f);

// 草の座標
const CVector GRASS_POS_1 =				CVector(-50.0f,		 0.0f,				-50.0f);
const CVector GRASS_POS_2 =				CVector(-100.0f,	 0.0f,				-50.0f);

// ツタの炎のオフセット座標
const CVector VINE_FIRE_OFFSET_POS =	CVector( 0.0f,		 VINE_HEIGHT / 2.0f, 0.0f);
// ツタの炎の大きさ
constexpr float VINE_FIRE_SCALE =		DEFAULT_FIRE_SCALE * 15.0f;

// 橋の座標
const CVector BRIDGE_POS_1 =			CVector( 50.0f,		 0.0f,				-50.0f);
const CVector BRIDGE_POS_2 =			CVector( 50.0f,		 0.0f,				-100.0f);
// 橋の回転
const CVector BRIDGE_ROT_2 =			CVector( 0.0f,		 180.0f,			 0.0f);

// 重りの座標
const CVector WEIGHT_POS =				CVector( 0.0f,		 0.0f,				-25.0f);

// 水の座標
const CVector WATER_POS =				CVector( 300.0f,	 1.0f,				 0.0f);

// アイテムの座標
const CVector ITEM_POS =				CVector( 0.0f,		 5.0f,				 30.0f);

// キャンプファイヤーの座標
const CVector CAMPFIRE_POS =			CVector( 0.0f,		 0.0f,				-70.0f);

// ポータルの欠片の数
constexpr int PORTAL_FRAGMENT_NUM =		4;

// 下がる壁の座標
const CVector DOWN_WALL_POS_1 =			CVector( 150.0f,	 0.0f,				 50.0f);
const CVector DOWN_WALL_POS_2 =			CVector(-225.5f,	 0.0f,				 468.0f);
// スイッチの座標
const CVector SWITCH_POS =				CVector( 0.0f,		 0.0f,				 50.0f);
// 3連スイッチの座標
const CVector SWITCHS_POS_1 =			CVector(-200.0f,	 0.0f,				 418.0f);
const CVector SWITCHS_POS_2 =			CVector(-200.0f,	 0.0f,				 368.0f);
const CVector SWITCHS_POS_3 =			CVector(-200.0f,	 0.0f,				 318.0f);

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
	//CRoom1* room1 = new CRoom1(CVector(0.0f, 0.0f, 700.0f));
	CConnectRoom* connectRoom = new CConnectRoom(CVector(0.0f, 0.0f, 700.0f));

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

	// 重りを生成
	CWeight* weight = new CWeight(WEIGHT_POS);

	// 空中オブジェクトを生成
	CAirConnectObj* air = new CAirConnectObj();
	air->Position(0.0f, 40.0f, 0.0f);

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

	// クリスタル
	CSwitch* crystal1 = new CSwitch(CVector(20.0f, 0.0f, 0.0f), false, ESwitchType::eBatteries);
	crystal1->SetOnOff(true);
	CSwitch* crystal2 = new CSwitch(CVector(-20.0f, 0.0f, 0.0f), false, ESwitchType::eBatteries);
	CSwitch* crystal3 = new CSwitch(CVector(-40.0f, 0.0f, 0.0f), false, ESwitchType::eBatteries);

	CSwitchMoveAirObj* switchAirObj =
		new CSwitchMoveAirObj(CVector(40.0f, 40.0f, 0.0f), { CVector(40.0f, 40.0f, 0.0f), CVector(80.0f,80.0f,0.0f) });
	crystal1->SetActionObj(switchAirObj);

	// 押し出す壁
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CSwitchPushWall* switchPushWall = 
		new CSwitchPushWall(model,model,CVector(80.0f,0.0f,0.0f),
			CVector(1.0f, 4.0f, 1.0f), CVector(100.0f, 0.0f, 0.0f), 12.0f);
	crystal2->SetActionObj(switchPushWall);

	// シールド
	CSwitchShield* switchShield =
		new CSwitchShield(CVector(1.0f,1.0f,1.0f));
	switchShield->Position(-40.0f, 0.0f, 10.0f);
	crystal3->SetActionObj(switchShield);

	//// スイッチを生成
	//std::vector<CSwitch*> switchs = { new CSwitch(SWITCH_POS) };
	//CModel* model = CResourceManager::Get<CModel>("MoveObject");
	//// 下がる壁を生成
	//CSwitchMoveFloor* moveFloor = new CSwitchMoveFloor(model, model, 
	//	CVector(50.0f, 10.0f, 0.0f), CVector(2.0f,1.0f,1.0f), CVector(0.0f, -50.0f, 0.0f), 2.0f);
	//moveFloor->SetSwitchs(switchs);
	//int size = switchs.size();
	//for (int i = 0; i < size; i++)
	//{
	//	// 作用するオブジェクトに下がる壁を設定
	//	switchs[i]->SetActionObj(moveFloor);
	//}
}
