#include "CDeliveryFieldManager.h"
#include "CDeliveryField.h"
#include "CDeliveryPlayer.h"
#include "CSky.h"

// 配達のフィールド管理クラスのインスタンス
CDeliveryFieldManager* CDeliveryFieldManager::spInstance = nullptr;

// 生成するフィールドの数
#define FIELD_NUM 5

// フィールド同士の距離
#define FIELD_DIST 400.0f

// プレイヤーとの距離の最高値、これより遠いフィールドはプレイヤーの前方へ
#define PLAYER_DIST 500.0f

// インスタンスを取得
CDeliveryFieldManager* CDeliveryFieldManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CDeliveryFieldManager::CDeliveryFieldManager()
{
	spInstance = this;

	// フィールドを生成
	CreateField();
}

// デストラクタ
CDeliveryFieldManager::~CDeliveryFieldManager()
{
	SAFE_DELETE(spInstance);
}

// 更新
void CDeliveryFieldManager::Update()
{
	// プレイヤーの座標を取得
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();

	// 全てのフィールドとプレイヤーの距離を計算する
	for (CDeliveryField* field : mFields)
	{
		CVector fieldPos = field->Position();
		// プレイヤーより奥にあるなら次へ
		if (fieldPos.Z() < playerPos.Z()) continue;

		// プレイヤーとの距離
		float dist = fieldPos.Z() - playerPos.Z();
		// プレイヤーとの距離が最高値以上なら
		if (dist > PLAYER_DIST)
		{
			// 一番奥のフィールドのZ座標から
			// フィールド同士の距離分さらに奥のZ座標
			mFieldPosZ = mFieldPosZ - FIELD_DIST;
			// 座標を設定
			field->Position(0.0f, 0.0f, mFieldPosZ);
		}
	}
}

// 指定のフィールドを取得する
CDeliveryField* CDeliveryFieldManager::GetField(int num, bool& end)
{
	if (num == mFields.size() - 1) end = true;

	return mFields[num];
}

// フィールドを生成
void CDeliveryFieldManager::CreateField()
{
	for (int i = 0; i < FIELD_NUM; i++)
	{
		CDeliveryField* field = new CDeliveryField();
		field->Position(0.0f, 0.0f, -FIELD_DIST * (i - 1));
		mFields.push_back(field);
		// 一番最後のフィールドのZ座標を保存
		if (i == FIELD_NUM - 1) mFieldPosZ = field->Position().Z();
	}
}
