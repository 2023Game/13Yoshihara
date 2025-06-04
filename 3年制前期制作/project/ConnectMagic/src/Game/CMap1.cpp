#include "CMap1.h"
#include "CBridge.h"
#include "CWater.h"
#include "CFragmentMergeArea.h"
#include "CPortalFragment.h"
#include "CCampFire.h"
#include "CTorch.h"
#include "CGrass.h"
#include "CUpgradeItem.h"

// 橋の座標
#define BRIDGE_POS_1 CVector(-325.0f,0.0f,252.5f)
#define BRIDGE_POS_2 CVector(-325.0f,0.0f,127.5f)

// 橋の回転
#define BRIDGE_ROT_2 CVector(0.0f,180.0f,0.0f)

// 橋のデフォルト角度
#define BRIDGE_ANGLE_1 90.0f

// 水面の座標
#define WATER_POS CVector(-250.0f,-10.0f,50.0f)
// 水面のスケール
#define WATER_SCALE CVector(500.0f,1.0f,300.0f)

// ポータルの欠片の数
#define PORTAL_FRAGMENT_NUM 4
// 合成地の座標
#define MERGE_AREA_POS CVector(-420.0f,10.0f,-200.0f)

// キャンプファイヤーの座標
#define CAMPFIRE_POS CVector(-260.0f,0.0f,-290.0f)

// 松明の座標
#define TORCH_POS_1 CVector(-240.0f,0.0f,-280.0f)

// 草の座標
#define GRASS_POS_1 CVector(-230.0f,0.0f,-280.0f)
#define GRASS_POS_2 CVector(-230.0f,0.0f,-280.0f)
#define GRASS_POS_3 CVector(-230.0f,0.0f,-280.0f)
// ツタの座標
#define VINE_POS_1 CVector(-15.0f,0.0f,170.0f)
// ツタの炎のオフセット座標
#define VINE_FIRE_OFFSET_POS CVector(0.0f,VINE_HEIGHT/2.0f,0.0f)
// ツタの炎の大きさ
#define VINE_FIRE_SCALE FIRE_SCALE * 15.0f

// 最初に配置するアイテムの座標
#define FIRST_ITEM_POS	15.0f
// 最初に配置するアイテムの数
#define FIRST_ITEM_NUM	10

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
	// 欠片の合成場所
	CFragmentMergeArea* mergeArea = new CFragmentMergeArea(PORTAL_FRAGMENT_NUM);
	// 座標を設定
	mergeArea->Position(MERGE_AREA_POS);

	// 水面を生成
	CWater* water = new CWater(WATER_SCALE);
	// 座標設定
	water->Position(WATER_POS);

	// 橋を生成
	CBridge* bridge = new CBridge();
	// 位置設定
	bridge->Position(BRIDGE_POS_1);
	// 初期角度を設定
	bridge->SetAngle(BRIDGE_ANGLE_1);

	// 橋を生成
	bridge = new CBridge();
	// 位置設定
	bridge->Position(BRIDGE_POS_2);
	// 回転設定
	bridge->Rotate(BRIDGE_ROT_2);

	// キャンプファイヤー
	CCampFire* campFire = new CCampFire();
	// 座標設定
	campFire->Position(CAMPFIRE_POS);
	// 落とすアイテムを設定
	campFire->SetDropItem(mergeArea->GetFragment(0));	
	
	// 松明を生成
	CTorch* torch = new CTorch();
	// 位置調整
	torch->Position(TORCH_POS_1);
	torch->SetFire(true);

	// 草を生成
	CGrass* grass = new CGrass();
	// 位置調整
	grass->Position(GRASS_POS_1);
	grass->SetDropItem(mergeArea->GetFragment(1));
	// 草を生成
	grass = new CGrass();
	// 位置調整
	grass->Position(GRASS_POS_2);
	grass->SetDropItem(mergeArea->GetFragment(2));
	// 草を生成
	grass = new CGrass();
	// 位置調整
	grass->Position(GRASS_POS_3);
	grass->SetDropItem(mergeArea->GetFragment(3));

	// ツタを生成
	grass = new CGrass(EGrassType::eVine, VINE_FIRE_OFFSET_POS, VINE_FIRE_SCALE);
	// 位置調整
	grass->Position(VINE_POS_1);

	// アイテムを生成
	CUpgradeItem* item;
	for (int i = 0; i < FIRST_ITEM_NUM; i++)
	{
		item = new CUpgradeItem();
		// 座標を設定
		item->Position(0.0f, 0.0f, FIRST_ITEM_POS * i);
	}
}

// 経路探索用のノードを生成
void CMap1::CreateNavNodes()
{
}
