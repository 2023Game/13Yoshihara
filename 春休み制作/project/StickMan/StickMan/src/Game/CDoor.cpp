#include "CDoor.h"
#include "CColliderMesh.h"
#include "CJobStatusManager.h"
#include "CSceneManager.h"
#include "CSound.h"
#include "CMoneyManager.h"

#define UI_OFFSET_Y 10.0f

#define SE_VOLUME 0.5f

// コンストラクタ
CDoor::CDoor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject()
	, mpModel(model)
{
	// 効果音を設定
	mpBuzzerSE = CResourceManager::Get<CSound>("BuzzerSE");
	// UIのオフセット位置を設定
	mUIOffsetPos = CVector(0.0f, UI_OFFSET_Y, 0.0f);

	mpCollider = new CColliderMesh
	{
		this,ELayer::eInteractObj,
		model,true
	};
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer,ELayer::eInteractSearch });

	Position(pos);
	Scale(scale);
	Rotate(rotation);

	mInteractStr = "仕事へ行く";
}

// デストラクタ
CDoor::~CDoor()
{
	SAFE_DELETE(mpCollider);
}

// 調べる
void CDoor::Interact()
{
	auto* jobStatusManager = CJobStatusManager::Instance();
	EJobType jobType = jobStatusManager->GetSelectJob();

	bool isLoad = false;
	auto* sceneManager = CSceneManager::Instance();
	// 選択されている仕事が解放されているなら仕事シーンへ遷移
	switch (jobType)
	{
	case EJobType::eTrash:
		
		sceneManager->LoadScene(EScene::eTrashGame);
		isLoad = true;

		break;
	case EJobType::eDelivery:
		
		sceneManager->LoadScene(EScene::eDeliveryGame);
		isLoad = true;

		break;
	// それ以外
	default:
		// ブザー音再生
		mpBuzzerSE->Play(SE_VOLUME, true);
		break;
	}

	// 仕事シーンをロードしているなら
	if (isLoad)
	{
		// お金の管理クラスを取得
		auto* moneyMgr = CMoneyManager::Instance();
		// 日数を経過
		moneyMgr->DayPass();
		// 今日の返済額を設定
		moneyMgr->SetDebtMoney(moneyMgr->GetDay());
		// 返済していない
		moneyMgr->SetDid(false);
	}
}

// 描画
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
