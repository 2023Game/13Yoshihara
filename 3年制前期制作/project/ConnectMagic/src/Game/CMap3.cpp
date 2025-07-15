#include "CMap3.h"
#include "CBridge.h"
#include "CWater.h"
#include "CFragmentMergeArea.h"
#include "CPortalFragment.h"
#include "CCampFire.h"
#include "CTorch.h"
#include "CGrass.h"
#include "CUpgradeItem.h"
#include "Maths.h"

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
#define TORCH_POS_1 CVector(-50.0f,0.0f,150.0f)
#define TORCH_POS_2	CVector(-65.0f,0.0f,245.0f)
#define TORCH_POS_3 CVector(-145.0f,0.0f,335.0f)
#define TORCH_POS_4 CVector(-230.0f,0.0f,250.0f)
#define TORCH_POS_5 CVector(-250.0f,0.0f,120.0f)
#define TORCH_POS_6 CVector(-275.0f,0.0f,-90.0f)
#define TORCH_POS_7 CVector(-250.0f,0.0f,40.0f)
#define TORCH_POS_8 CVector(-375.0f,0.0f,80.0f)
#define TORCH_POS_9 CVector(-465.0f,0.0f,80.0f)
#define TORCH_POS_10 CVector(-465.0f,0.0f,-40.0f)
#define TORCH_POS_11 CVector(-70.0f,0.0f,-125.0f)
#define TORCH_POS_12 CVector(-170.0f,0.0f,-10.0f)
#define TORCH_POS_13 CVector(-380.0f,0.0f,-135.0f)
#define TORCH_POS_14 CVector(-180.0f,0.0f,-210.0f)

// 草の座標
#define GRASS_POS_1 CVector(-370.0f,0.0f,-140.0f)
#define GRASS_POS_2 CVector(-200.0f,0.0f,-160.0f)
#define GRASS_POS_3 CVector(-5.0f,0.0f,-180.0f)
#define GRASS_POS_4 CVector(-460.0f,0.0f,25.0f)
#define GRASS_POS_5 CVector(20.0f,0.0f,-150.0f)
#define GRASS_POS_6 CVector(-230.0f,0.0f,-280.0f)

// ツタの座標
#define VINE_POS_1	CVector(-15.0f,0.0f,170.0f)
#define VINE_POS_2	CVector(-135.0f,0.0f,295.0f)
// ツタの回転
#define VINE_ROT_2	CVector(0.0f,70.0f,0.0f)
// ツタの炎のオフセット座標
#define VINE_FIRE_OFFSET_POS CVector(0.0f,VINE_HEIGHT/2.0f,0.0f)
// ツタの炎の大きさ
#define VINE_FIRE_SCALE FIRE_SCALE * 15.0f

// 最初に配置するアイテムの座標
#define ITEM_POS_1	CVector(0.0f,0.0f,50.0f)
#define ITEM_POS_2	CVector(0.0f,0.0f,270.0f)
#define ITEM_POS_3	CVector(-270.0f,0.0f,90.0f)

// コンストラクタ
CMap3::CMap3()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("Map3");

	// コライダーを生成
	CreateCol("Map3_Ground_Col", "Map3_Wall_Col", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CMap3::~CMap3()
{
}

// フィールドオブジェクトを生成
void CMap3::CreateFieldObjects()
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

	// 松明の座標のリスト
	CVector torchPosList[] = {
		TORCH_POS_1,
		TORCH_POS_2,
		TORCH_POS_3,
		TORCH_POS_4,
		TORCH_POS_5,
		TORCH_POS_6,
		TORCH_POS_7,
		TORCH_POS_8,
		TORCH_POS_9,
		TORCH_POS_10,
		TORCH_POS_11,
		TORCH_POS_12,
		TORCH_POS_13,
		TORCH_POS_14,
	};
	// サイズ
	int size = ARRAY_SIZE(torchPosList);

	// 松明
	CTorch* torch;
	for (int i = 0; i < size; i++)
	{
		// 松明を生成
		torch = new CTorch();
		torch->Position(torchPosList[i]);
		// 最初の一つだけ火をつける
		if (i == 0)
		{
			torch->SetFire(true);
		}
	}

	// 草の座標のリスト
	CVector grassPosList[] = {
		GRASS_POS_1,
		GRASS_POS_2,
		GRASS_POS_3,
		GRASS_POS_4,
		GRASS_POS_5,
		GRASS_POS_6,
	};
	// サイズ
	size = ARRAY_SIZE(grassPosList);

	// 草
	CGrass* grass;
	for (int i = 0; i < size; i++)
	{
		// 草を生成
		grass = new CGrass();
		// 位置調整
		grass->Position(grassPosList[i]);
		switch (i)
		{
		case 0:
			grass->SetDropItem(mergeArea->GetFragment(1));
			break;
		case 1:
			grass->SetDropItem(mergeArea->GetFragment(2));
			break;
		case 2:
			grass->SetDropItem(mergeArea->GetFragment(3));
			break;
		}
	}

	// ツタを生成
	grass = new CGrass(EGrassType::eVine, VINE_FIRE_OFFSET_POS, VINE_FIRE_SCALE);
	// 位置調整
	grass->Position(VINE_POS_1);
	// ツタを生成
	grass = new CGrass(EGrassType::eVine, VINE_FIRE_OFFSET_POS, VINE_FIRE_SCALE);
	// 位置調整
	grass->Position(VINE_POS_2);
	// 回転
	grass->Rotation(VINE_ROT_2);

	// アイテムを生成
	CUpgradeItem* item;
	item = new CUpgradeItem();
	// 座標を設定
	item->Position(ITEM_POS_1);
	item = new CUpgradeItem();
	// 座標を設定
	item->Position(ITEM_POS_2);
	item = new CUpgradeItem();
	// 座標を設定
	item->Position(ITEM_POS_2);
	item = new CUpgradeItem();
	// 座標を設定
	item->Position(ITEM_POS_3);
	item = new CUpgradeItem();
	// 座標を設定
	item->Position(ITEM_POS_3);
	item = new CUpgradeItem();
	// 座標を設定
	item->Position(ITEM_POS_3);
}

// 経路探索用のノードを生成
void CMap3::CreateNavNodes()
{
}
