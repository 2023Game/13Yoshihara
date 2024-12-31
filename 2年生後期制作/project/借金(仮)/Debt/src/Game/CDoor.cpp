#include "CDoor.h"
#include "CColliderMesh.h"
#include "CJobStatusManager.h"
#include "CSceneManager.h"

#define UI_OFFSET_Y 10.0f

// コンストラクタ
CDoor::CDoor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject()
	, mpModel(model)
{
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

	// 選択されている仕事が解放されていなければ
	if (!jobStatusManager->GetUnlock(jobType))
	{
		// TODO：ブザー音を再生してこれ以下を処理しない

		return;
	}

	auto* sceneManager = CSceneManager::Instance();
	// 選択されている仕事が解放されているなら仕事シーンへ遷移
	switch (jobType)
	{
	case EJobType::eTrash:
		
		sceneManager->LoadScene(EScene::eTrashGame);

		break;
	case EJobType::eDelivery:
		
		sceneManager->LoadScene(EScene::eDeliveryGame);

		break;
	// それ以外
	default:
		// TODO: ブザー音再生
		break;
	}
}

// 描画
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
