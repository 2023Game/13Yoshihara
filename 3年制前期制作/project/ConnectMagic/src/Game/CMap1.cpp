#include "CMap1.h"
#include "CBridge.h"

// 松明の炎のオフセット座標
#define TORCH_FIRE_OFFSET_POS CVector(0.0f,9.0f,0.0f)

// 橋のオフセット座標
#define BRIDGE_OFFSET_POS_1 CVector(-325.0f,0.0f,252.5f)
#define BRIDGE_OFFSET_POS_2 CVector(-325.0f,0.0f,127.5f)

// 橋のオフセット回転
#define BRIDGE_OFFSET_ROT_2 CVector(0.0f,180.0f,0.0f)

// 橋のデフォルト角度
#define BRIDGE_ANGLE_1 90.0f

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
	// 橋を生成
	CBridge* bridge = new CBridge();
	// 位置設定
	bridge->Position(BRIDGE_OFFSET_POS_1);
	// 初期角度を設定
	bridge->SetAngle(BRIDGE_ANGLE_1);

	// 橋を生成
	bridge = new CBridge();
	// 位置設定
	bridge->Position(BRIDGE_OFFSET_POS_2);
	// 回転設定
	bridge->Rotate(BRIDGE_OFFSET_ROT_2);
}

// 経路探索用のノードを生成
void CMap1::CreateNavNodes()
{
}
