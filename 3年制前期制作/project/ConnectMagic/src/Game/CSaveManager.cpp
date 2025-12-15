#include "CSaveManager.h"
#include "CTaskManager.h"
#include "CInput.h"
#include "CRewindUI.h"
#include "CPlayer.h"

// 保存するフレーム数
constexpr float SAVE_FRAMES = 600;

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
		// ゲームポーズ中なら処理しない
		if (!CTaskManager::Instance()->IsPaused(PAUSE_GAME))
		{
			Save();
		}
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
		}
		break;
	}
}

// 状態を保存する
void CSaveManager::Save()
{
	// データ
	GameData data;
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
	// プレイヤーデータを保存
	if (player)
	{
		data.playerData = player->SaveState();
	}

	//  マップの値をループする
	for (auto const& element : mSavableInstanceMap)
	{
		unsigned int id = element.first;
		CSavable* obj = element.second;

		std::vector<char> rawData = obj->SaveState();

		// IDをキーとして格納
		data.objects.emplace_back(
			obj->GetUniqueInstanceID(),
			obj->GetTypeID(),
			std::move(rawData)
		);
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

// データをロード
void CSaveManager::Load()
{
	// データがないなら
	// 処理しない
	if (mData.empty()) return;
	// ロードするデータ
	const GameData& data = mData.back();
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());

	// プレイヤーデータをロード
	if (player)
	{
		player->LoadState(data.playerData);
	}

	// 登録されているすべてのオブジェクトの状態を巻き戻し
	for (const auto& objData : data.objects)
	{
		// IDを使ってマップから有効なインスタンスを安全に検索
		auto it = mSavableInstanceMap.find(objData.instanceID);

		// 検索に成功し、オブジェクトが現在も生きている場合のみロードを実行
		if (it != mSavableInstanceMap.end())
		{
			CSavable* targetObj = it->second;
			targetObj->LoadState(objData.rawData);
		}
	}

	// 一番新しい状態を削除
	DeleteNew();
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

// 保存可能オブジェクトを登録
void CSaveManager::AddSavableObj(CSavable* obj)
{
	// IDをキーとしてマップに登録
	mSavableInstanceMap[obj->GetUniqueInstanceID()] = obj;
}

// 保存可能オブジェクトを削除
void CSaveManager::DeleteSavableObj(CSavable* obj)
{
	// IDをキーとしてマップから削除
	mSavableInstanceMap.erase(obj->GetUniqueInstanceID());
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
		// ロード開始時にゲームをポーズ
		CTaskManager::Instance()->Pause(PAUSE_GAME);
	}
	else {
		mpRewindUI->SetEnable(false);
		// ポーズ解除
		CTaskManager::Instance()->UnPause(PAUSE_GAME);
		// プレイヤーを待機状態に戻す
		CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
		if (player)
		{
			player->ChangeState(CPlayer::EState::eIdle);
		}
	}

	mState = state;
}
