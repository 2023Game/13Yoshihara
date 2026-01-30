#include "CRoom2.h"
#include "CWeight.h"
#include "CSwitch.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CSaveManager.h"

// 部屋の長さ
constexpr float ROOM_LENGTH =			160.0f;

// 重りの座標
const CVector WEIGHT_OFFSET_POS =		CVector( 30.0f,  0.0f,	-20.0f);

// 動く床のオフセット座標
const CVector MOVE_FLOOR_OFFSET_POS =	CVector( 0.0f, -10.0f,	-60.0f);
// 動く床のスケール
const CVector MOVE_FLOOR_SCALE =		CVector( 4.0f,  1.0f,	 4.0f);
// 動く床の移動
const CVector MOVE_FLOOR_MOVE =			CVector( 0.0f,  0.0f,	-40.0f);
// 動く床の移動時間
constexpr float MOVE_FLOOR_MOVE_TIME =	2.5f;

// 水のオフセット座標
const CVector WATER_OFFSET_POS =		CVector( 0.0f, -5.0f,	-85.0f);
// 水のスケール
const CVector WATER_SCALE =				CVector( 50.0f, 1.0f,	 100.0f);

// リスポーン地点
const CVector RESPAWN_OFFSET_POS =		CVector( 0.0f,  0.0f,	-30.0f);
// リスポーン地点の半径
constexpr float RESPAWN_RADIUS =		30.0f;

// コンストラクタ
CRoom2::CRoom2(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room2");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("Room2_Col");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CRoom2::~CRoom2()
{
}

// 部屋の有効無効を設定
void CRoom2::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);

	// 保存管理クラス
	CSaveManager* saveMgr = CSaveManager::Instance();
	// 有効時のみ
	if (enable) {
		// 重りのフラグを変更
		mpWeight->SetEnable(enable);
		// 重りを追加
		saveMgr->AddSavableObj(mpWeight);
		// 移動床を追加
		saveMgr->AddSavableObj(mpMoveFloor);
	}
	// 無効時
	else
	{
		// 重りがスイッチに張り付いていないなら
		if (!mpWeight->GetIsAttach())
		{
			// 重りのフラグを変更
			mpWeight->SetEnable(enable);
		}
		// 重りを削除
		saveMgr->DeleteSavableObj(mpWeight);
		// 移動床を削除
		saveMgr->DeleteSavableObj(mpMoveFloor);
	}
}

// フィールドオブジェクトを生成
void CRoom2::CreateFieldObjects()
{
	// 重りを生成
	mpWeight = new CWeight(Position() + WEIGHT_OFFSET_POS);
	// 最初は無効
	mpWeight->SetEnable(false);

	// 動く床を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CModel* colModel = CResourceManager::Get<CModel>("MoveObject_Col");
	mpMoveFloor = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eField);
	// リストに追加
	mObjs.push_back(mpMoveFloor);

	// 水を生成
	mpWater = new CWater(Position() + WATER_OFFSET_POS, WATER_SCALE);
	// リストに追加
	mObjs.push_back(mpWater);
}