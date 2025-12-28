#include "CLastRoom.h"
#include "CPortal.h"

// ポータルのオフセット座標
const CVector PORTAL_OFFSET_POS = CVector(0.0f, 15.0f, -60.0f);

// コンストラクタ
CLastRoom::CLastRoom(const CVector& pos)
	: CRoomBase(0.0f)
{
	mpModel = CResourceManager::Get<CModel>("LastRoom");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("LastRoom_Col");

	// オブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CLastRoom::~CLastRoom()
{
}

// オブジェクトを生成
void CLastRoom::CreateFieldObjects()
{
	// ポータルを生成
	mpPortal = new CPortal();
	// 座標を設定
	mpPortal->Position(Position() + PORTAL_OFFSET_POS);
	// リストに追加
	mObjs.push_back(mpPortal);
}
