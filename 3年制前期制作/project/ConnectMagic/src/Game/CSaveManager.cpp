#include "CSaveManager.h"
#include "CPlayer.h"
#include "CBox.h"
#include "CMoveObj.h"
#include "CSwitchMoveFloor.h"
#include "CTaskManager.h"
#include "CInput.h"

// 保存するフレーム数
#define SAVE_FRAMES 120

// インスタンス
CSaveManager* CSaveManager::spInstance = nullptr;

// インスタンスを取得
CSaveManager* CSaveManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CSaveManager::CSaveManager()
	: CTask(ETaskPriority::eSystem, 0, ETaskPauseType::eSystem, false, true)
	, mState(EState::eSave)
{
	spInstance = this;
}

// デストラクタ
CSaveManager::~CSaveManager()
{
}

// 更新
void CSaveManager::Update()
{
	switch (mState)
	{
	case CSaveManager::EState::eSave:
		// セーブ
		Save();
		break;
	case CSaveManager::EState::eLoad:

		if (CInput::Key(VK_RBUTTON)) {
			CTaskManager::Instance()->Pause(PAUSE_GAME);
			// ロード
			Load();
			CTaskManager::Instance()->UnPause(PAUSE_GAME);
		}
		// キーを離したら
		else if (CInput::PullKey(VK_RBUTTON)) {
			// ロード終了
			ChangeState(EState::eSave);
			// ポーズ解除
			CTaskManager::Instance()->UnPause(PAUSE_GAME);
		}
		break;
	}
}

// 状態を保存する
void CSaveManager::Save()
{
	// ゲームポーズ中なら処理しない
	if (CTaskManager::Instance()->IsPaused(PAUSE_GAME)) return;

	// データ
	GameData data;
	// プレイヤーを取得
	CPlayerBase* player = CPlayerBase::Instance();
	// 座標
	data.player.pos = player->Position();
	// 方向
	data.player.vec = player->EulerAngles();

	// 箱を取得
	for (CBox* box : mBox) {
		// リストに追加
		data.boxes.emplace_back(box->Position(), box);
	}

	// 移動オブジェクトを取得
	for (CMoveObj* obj : mMoveObj) {
		// リストに追加
		data.moveObjs.emplace_back(obj->Position(),
			obj->GetState(), obj->GetPreState(),
			obj->GetElapsedTime(), obj);
	}

	// 床を取得
	for (CSwitchMoveFloor* floor : mMoveFloor) {
		// リストに追加
		data.moveFloor.emplace_back(floor->Position(),
			floor->GetState(), floor->GetPreState(),
			floor->GetElapsedTime(), floor);
	}

	// データのリストに追加
	mData.push_back(data);

	// 保存するフレーム数より多いなら
	if (mData.size() > SAVE_FRAMES)
	{
		// 一番古い状態を削除
		DeleteOld();
	}
}

// 一番古い状態を削除する
void CSaveManager::DeleteOld()
{
	mData.pop_front();
}

// 一番新しい状態を削除
void CSaveManager::DeleteNew()
{
	mData.pop_back();
}

// 全て削除する
void CSaveManager::AllDelete()
{
	mData.clear();
}

// データをロード
void CSaveManager::Load()
{
	// データがないなら
	// 処理しない
	if (mData.empty()) return;
	// ロードするデータ
	const GameData& data = mData[mData.size() - 1];

	// プレイヤーデータ
	auto& playerData = data.player;
	// プレイヤーを取得
	CPlayerBase* player = CPlayerBase::Instance();
	if (player)
	{
		// 座標をロード
		player->Position(playerData.pos);
		// 方向をロード
		player->Rotation(playerData.vec);
	}

	CBox* box = nullptr;
	for (auto& boxData : data.boxes)
	{
		// 箱を取得
		box = boxData.box;
		if (box)
		{
			// 座標をロード
			box->Position(boxData.pos);
		}
	}

	CMoveObj* moveObj = nullptr;
	for (auto& objData : data.moveObjs)
	{
		// 動くオブジェクトを取得
		moveObj = objData.moveObj;
		if (moveObj)
		{
			// 座標をロード
			moveObj->Position(objData.pos);
			// 状態をロード
			moveObj->SetState(objData.state);
			// 前の状態をロード
			moveObj->SetPreState(objData.preState);
			// 経過時間をロード
			moveObj->SetElapsedTime(objData.elapsedTime);
		}
	}

	CSwitchMoveFloor* moveFloor = nullptr;
	for (auto& floorData : data.moveFloor)
	{
		// 床を取得
		moveFloor = floorData.moveFloor;
		if (moveFloor)
		{
			// 座標をロード
			moveFloor->Position(floorData.pos);
			// 状態をロード
			moveFloor->SetState(floorData.state);
			// 前の状態をロード
			moveFloor->SetPreState(floorData.preState);
			// 経過時間をロード
			moveFloor->SetElapsedTime(floorData.elapsedTime);
		}
	}

	// 一番新しい状態を削除
	DeleteNew();
}

// 保存する箱に追加
void CSaveManager::AddBox(CBox* box)
{
	mBox.push_back(box);
}

// 保存する箱から削除
void CSaveManager::DeleteBox(CBox* box)
{
	mBox.erase(
		std::remove(mBox.begin(), mBox.end(), box),
		mBox.end()
	);
}

// 保存する移動オブジェクトに追加
void CSaveManager::AddMoveObj(CMoveObj* obj)
{
	mMoveObj.push_back(obj);
}

// 保存する移動オブジェクトから削除
void CSaveManager::DeleteMoveObj(CMoveObj* obj)
{
	mMoveObj.erase(
		std::remove(mMoveObj.begin(), mMoveObj.end(), obj),
		mMoveObj.end()
	);
}

// 保存するスイッチで移動する床に追加
void CSaveManager::AddMoveFloor(CSwitchMoveFloor* floor)
{
	mMoveFloor.push_back(floor);
}

// 保存するスイッチで移動する床から削除
void CSaveManager::DeleteMoveFloor(CSwitchMoveFloor* floor)
{
	mMoveFloor.erase(
		std::remove(mMoveFloor.begin(), mMoveFloor.end(), floor),
		mMoveFloor.end()
	);
}

// 状態切り替え
void CSaveManager::ChangeState(EState state)
{
	// 同じなら処理しない
	if (mState == state) return;

	mState = state;
}
