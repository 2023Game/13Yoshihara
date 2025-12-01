#include "CSaveManager.h"
#include "CPlayer.h"
#include "CWeight.h"
#include "CMoveObj.h"
#include "CSwitchMoveFloor.h"
#include "CSwitchMoveWall.h"
#include "CSwitchMoveAirObj.h"
#include "CSwitch.h"
#include "CTaskManager.h"
#include "CInput.h"
#include "CConnectPointManager.h"

// 保存するフレーム数
#define SAVE_FRAMES 600

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

	// 巻き戻しUI
	mpRewindUI = new CRewindUI();
	mpRewindUI->SetEnable(false);
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
		// セーブ
	case CSaveManager::EState::eSave:
		Save();
		break;

		// ロード
	case CSaveManager::EState::eLoad:

		if (CInput::Key(VK_RBUTTON)) {
			// テキスト非表示
			mpRewindUI->SetTextEnable(false);
			// 画像の色を青に設定
			mpRewindUI->SetImgColor(CColor::blue);
			CTaskManager::Instance()->Pause(PAUSE_GAME);
			// ロード
			Load();
			CTaskManager::Instance()->UnPause(PAUSE_GAME);
		}
		// キーを離すか
		// データが空になったら
		if (CInput::PullKey(VK_RBUTTON) || mData.empty()) {
			// ロード終了
			ChangeState(EState::eSave);
			// ポーズ解除
			CTaskManager::Instance()->UnPause(PAUSE_GAME);
			// プレイヤーを待機状態に戻す
			CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
			player->ChangeState(CPlayer::EState::eIdle);
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
	// 接続しているターゲット
	data.player.target = CConnectPointManager::Instance()->GetConnectWandTarget();
	// アニメーション番号
	data.player.animationType = player->AnimationIndex();
	// アニメーションの再生時間
	data.player.animationFrame = player->GetAnimationFrame();

	// 重りを取得
	for (CWeight* weight : mWeights)
	{
		// リストに追加
		data.weightes.emplace_back(weight->Position(), weight);
	}

	// 移動オブジェクトを取得
	for (CMoveObj* obj : mMoveObjs)
	{
		// リストに追加
		data.moveObjs.emplace_back(obj->Position(),
			obj->GetState(), obj->GetPreState(),
			obj->GetElapsedTime(), obj);
	}

	// 床を取得
	for (CSwitchMoveFloor* floor : mMoveFloors) 
	{
		// リストに追加
		data.moveFloor.emplace_back(floor->Position(),
			floor->GetState(), floor->GetPreState(),
			floor->GetElapsedTime(), floor);
	}

	// 壁を取得
	for (CSwitchMoveWall* wall: mMoveWalls) 
	{
		// リストに追加
		data.moveWall.emplace_back(wall->Position(),
			wall->GetState(),
			wall->GetElapsedTime(), wall);
	}

	// 空中オブジェクトを取得
	for (CSwitchMoveAirObj* airObj : mMoveAirObjs) 
	{
		// リストに追加
		data.moveAirObj.emplace_back(airObj->Position(),
			airObj->GetState(),
			airObj->GetElapsedTime(), airObj);
	}

	// スイッチを取得
	for (CSwitch* switchObj : mSwitchs)
	{
		// リストに追加
		data.switchData.emplace_back(switchObj->GetOnOff(), switchObj);
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
		// 接続を有効
		CConnectPointManager::Instance()->EnableConnect(playerData.target);
		// アニメーション設定
		player->ChangeAnimationTime(playerData.animationType, playerData.animationFrame);
	}

	CWeight* weight = nullptr;
	for (auto& WeightData : data.weightes)
	{
		// 重りを取得
		weight = WeightData.weight;
		if (weight)
		{
			// 座標をロード
			weight->Position(WeightData.pos);
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

	CSwitchMoveWall* moveWall = nullptr;
	for (auto& wallData : data.moveWall)
	{
		// 壁を取得
		moveWall = wallData.moveWall;
		if (moveWall)
		{
			// 座標をロード
			moveWall->Position(wallData.pos);
			// 状態をロード
			moveWall->SetState(wallData.state);
			// 経過時間をロード
			moveWall->SetElapsedTime(wallData.elapsedTime);
		}
	}

	CSwitchMoveAirObj* moveAirObj = nullptr;
	for (auto& airData : data.moveAirObj)
	{
		// 空中オブジェクトを取得
		moveAirObj = airData.moveAirObj;
		if (moveAirObj)
		{
			// 座標をロード
			moveAirObj->Position(airData.pos);
			// 状態をロード
			moveAirObj->SetState(airData.state);
			// 経過時間をロード
			moveAirObj->SetElapsedTime(airData.elapsedTime);
		}
	}

	CSwitch* switchObj = nullptr;
	for (auto& data : data.switchData)
	{
		// スイッチを取得
		switchObj = data.switchObj;
		if (switchObj)
		{
			// オンオフを設定
			switchObj->SetOnOff(data.isOn);
		}
	}
	// 一番新しい状態を削除
	DeleteNew();
}

// 保存する重りに追加
void CSaveManager::AddWeight(CWeight* weight)
{
	mWeights.push_back(weight);
}

// 保存する重りから削除
void CSaveManager::DeleteWeight(CWeight* weight)
{
	mWeights.erase(
		std::remove(mWeights.begin(), mWeights.end(), weight),
		mWeights.end()
	);
}

// 保存する移動オブジェクトに追加
void CSaveManager::AddMoveObj(CMoveObj* obj)
{
	mMoveObjs.push_back(obj);
}

// 保存する移動オブジェクトから削除
void CSaveManager::DeleteMoveObj(CMoveObj* obj)
{
	mMoveObjs.erase(
		std::remove(mMoveObjs.begin(), mMoveObjs.end(), obj),
		mMoveObjs.end()
	);
}

// 保存するスイッチで移動する床に追加
void CSaveManager::AddMoveFloor(CSwitchMoveFloor* floor)
{
	mMoveFloors.push_back(floor);
}

// 保存するスイッチで移動する床から削除
void CSaveManager::DeleteMoveFloor(CSwitchMoveFloor* floor)
{
	mMoveFloors.erase(
		std::remove(mMoveFloors.begin(), mMoveFloors.end(), floor),
		mMoveFloors.end()
	);
}

// 保存するスイッチで移動する壁に追加
void CSaveManager::AddMoveWall(CSwitchMoveWall* wall)
{
	mMoveWalls.push_back(wall);
}

// 保存するスイッチで移動する壁から削除
void CSaveManager::DeleteMoveFloor(CSwitchMoveWall* wall)
{
	mMoveWalls.erase(
		std::remove(mMoveWalls.begin(), mMoveWalls.end(), wall),
		mMoveWalls.end()
	);
}

// 保存するスイッチで移動する空中オブジェクトに追加
void CSaveManager::AddMoveAirObj(CSwitchMoveAirObj* air)
{
	mMoveAirObjs.push_back(air);
}

// 保存するスイッチで移動する空中オブジェクトから削除
void CSaveManager::DeleteMoveAirObj(CSwitchMoveAirObj* air)
{
	mMoveAirObjs.erase(
		std::remove(mMoveAirObjs.begin(), mMoveAirObjs.end(), air),
		mMoveAirObjs.end()
	);
}

// 保存するスイッチに追加
void CSaveManager::AddMoveAirObj(CSwitch* switchObj)
{
	mSwitchs.push_back(switchObj);
}

// 保存するスイッチから削除
void CSaveManager::DeleteSwitch(CSwitch* switchObj)
{
	mSwitchs.erase(
		std::remove(mSwitchs.begin(), mSwitchs.end(), switchObj),
		mSwitchs.end()
	);
}

// 状態切り替え
void CSaveManager::ChangeState(EState state)
{
	// 同じなら処理しない
	if (mState == state) return;

	if (state == EState::eLoad) {
		mpRewindUI->SetEnable(true);
		mpRewindUI->SetTextEnable(true);
		mpRewindUI->SetImgColor(CColor::red);
	}
	else {
		mpRewindUI->SetEnable(false);
	}

	mState = state;
}
