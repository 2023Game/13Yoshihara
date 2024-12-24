#include "CDoor.h"
#include "CColliderMesh.h"
#include "CStageSelectMenu.h"
#include "CJobStatusManager.h"
#include "CSceneManager.h"

#define UI_OFFSET_Y 10.0f

CDoor::CDoor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject()
	, mpModel(model)
{
	// UI�̃I�t�Z�b�g�ʒu��ݒ�
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

	mInteractStr = "�d���֍s��";

	// �X�e�[�W�I�����j���[���쐬
	mpStageSelectMenu = new CStageSelectMenu();
}

CDoor::~CDoor()
{
	SAFE_DELETE(mpCollider);
}

// ���ׂ�
void CDoor::Interact()
{
	auto* jobStatusManager = CJobStatusManager::Instance();
	EJobType jobType = jobStatusManager->GetSelectJob();

	// �I������Ă���d�����������Ă��Ȃ����
	if (!jobStatusManager->GetUnlock(jobType))
	{
		// TODO�F�u�U�[�����Đ����Ă���ȉ����������Ȃ�

		return;
	}

	auto* sceneManager = CSceneManager::Instance();
	// �I������Ă���d�����������Ă���Ȃ�d���V�[���֑J��
	switch (jobType)
	{
	case EJobType::eTrash:
		
		sceneManager->LoadScene(EScene::eTrashGame);

		break;
	case EJobType::eDelivery:
		
		sceneManager->LoadScene(EScene::eDeliveryGame);

		break;
	// ����ȊO
	default:
		// TODO: �u�U�[���Đ�
		break;
	}
}

void CDoor::Render()
{
	mpModel->Render(Matrix());
}
