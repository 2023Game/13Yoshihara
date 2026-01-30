#pragma once
#include "SaveData.h"
#include <deque>
#include "CSavable.h"
#include <unordered_map>

class CRewindUI;

// 保存の管理クラス
class CSaveManager : CTask
{
public:
	// インスタンスを取得
	static CSaveManager* Instance();

	// コンストラクタ
	CSaveManager();
	// デストラクタ
	~CSaveManager();

	// 更新
	void Update() override;

	// 全て削除する
	void AllDelete();

	// 保存可能オブジェクトを登録・削除
	void AddSavableObj(CSavable* obj);
	void DeleteSavableObj(CSavable* obj);

	// 保存管理クラスの状態
	enum class EState
	{
		eSave,	// セーブ
		eLoad,	// ロード
	};
	// 状態切り替え
	void ChangeState(EState state);
	// 状態を取得
	EState GetState() const;

private:
	// 状態を保存する
	void Save();
	// データをロード
	void Load();
	// 一番古い状態を削除する
	void DeleteOld();
	// 一番新しい状態を削除
	void DeleteNew();

	// 物理の影響を有効
	void AllPhysicsOn();
	// 物理の影響を無効
	void AllPhysicsOff();

	// 剛体の物理影響を有効
	void PhysicsOn(btRigidBody* body);
	// 剛体の物理影響を無効
	void PhysicsOff(btRigidBody* body);

	// インスタンス
	static CSaveManager* spInstance;

	EState mState;	// 状態

	// 巻き戻しUI
	CRewindUI* mpRewindUI;

	// 保存データ
	// deque：前後への追加と削除が多いときに使う
	std::deque<GameData> mData;

	// 保存するオブジェクト
	std::unordered_map<unsigned int, CSavable*> mSavableInstanceMap;
	// 本来のフラグを保存する用
	std::map<btRigidBody*, int> mOriginalFlags;
};