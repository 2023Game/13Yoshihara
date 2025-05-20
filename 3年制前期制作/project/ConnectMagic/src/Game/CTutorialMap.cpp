#include "CTutorialMap.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CBox.h"
#include "CAirConnectObj.h"
#include "CPortal.h"
#include "CRespawnArea.h"

// スイッチの座標
#define SWITCH_POS	CVector(20.0f,0.0f,20.0f)
// ドアの座標
#define DOOR_POS	CVector(8.5f,0.0f,-42.5f)
// 箱の座標
#define BOX_POS		CVector(-25.0f,0.0f,-25.0f)

// 空中の接続オブジェクトの座標
#define AIR_POS_1	CVector( -2.5f, 30.0f, -90.0f)
#define AIR_POS_2	CVector(-42.5f, 50.0f,-250.0f)
#define AIR_POS_3	CVector(-62.5f, 90.0f,-360.0f)
#define AIR_POS_4	CVector(-22.5f,100.0f,-420.0f)
#define AIR_POS_5	CVector( 82.5f,130.0f,-440.0f)
#define AIR_POS_6	CVector(182.5f,150.0f,-440.0f)
#define AIR_POS_7	CVector(272.5f,180.0f,-480.0f)
#define AIR_POS_8	CVector(262.5f,210.0f,-540.0f)

// 空中の接続オブジェクトの回転値
#define AIR_ROT_2	CVector(0.0f,- 25.0f,0.0f)
#define AIR_ROT_4	CVector(0.0f,- 90.0f,0.0f)
#define AIR_ROT_5	CVector(0.0f,  90.0f,0.0f)
#define AIR_ROT_6	CVector(0.0f,-115.0f,0.0f)
#define AIR_ROT_7	CVector(0.0f,-205.0f,0.0f)
#define AIR_ROT_8	CVector(0.0f,  90.0f,0.0f)

// ポータルの設定
#define PORTAL_POS	CVector(90.0f, 220.0f + 5.0f * PORTAL_SIZE, -700.0f)
#define PORTAL_SIZE 2.0f

// リスポーン地点の設定
#define RESPAWN_POS_1	CVector(  0.0f,-5.0f,-180.0f)
#define RESPAWN_POS_2	CVector(-90.0f,35.0f,-430.0f)
#define RESPAWN_RADIUS	25.0f

// コンストラクタ
CTutorialMap::CTutorialMap()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TutorialMap");

	// オブジェクトを生成
	CreateFieldObjects();
	// コライダ―を生成
	CreateCol("TutorialMap_Ground_Col", "TutorialMap_Wall_Col", "");
}

// デストラクタ
CTutorialMap::~CTutorialMap()
{
}

// フィールドのオブジェクトを生成
void CTutorialMap::CreateFieldObjects()
{
	// ドアを生成
	CDoor* door = new CDoor(CVector::zero);
	// ドアの位置調整
	door->Position(DOOR_POS);

	// スイッチを生成
	CSwitch* doorSwitch = new CSwitch(SWITCH_POS);
	// ドアをスイッチの作用オブジェクトに設定
	doorSwitch->SetActionObj(door);

	// 箱を生成
	CBox* box = new CBox();
	// 箱の位置調整
	box->Position(BOX_POS);

	// 空中の接続オブジェクトを生成
	CAirConnectObj* airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置調整
	airConnectObj->Position(AIR_POS_1);

	// 空中の接続オブジェクトを生成
	airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置回転調整
	airConnectObj->Position(AIR_POS_2);
	airConnectObj->Rotation(AIR_ROT_2);

	// 空中の接続オブジェクトを生成
	airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置調整
	airConnectObj->Position(AIR_POS_3);

	// 空中の接続オブジェクトを生成
	airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置回転調整
	airConnectObj->Position(AIR_POS_4);
	airConnectObj->Rotation(AIR_ROT_4);

	// 空中の接続オブジェクトを生成
	airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置回転調整
	airConnectObj->Position(AIR_POS_5);
	airConnectObj->Rotation(AIR_ROT_5);

	// 空中の接続オブジェクトを生成
	airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置回転調整
	airConnectObj->Position(AIR_POS_6);
	airConnectObj->Rotation(AIR_ROT_6);

	// 空中の接続オブジェクトを生成
	airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置回転調整
	airConnectObj->Position(AIR_POS_7);
	airConnectObj->Rotation(AIR_ROT_7);

	// 空中の接続オブジェクトを生成
	airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置回転調整
	airConnectObj->Position(AIR_POS_8);
	airConnectObj->Rotation(AIR_ROT_8);

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