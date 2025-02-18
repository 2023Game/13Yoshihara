#include "CDoor.h"
#include "CColliderMesh.h"
#include "CJobStatusManager.h"
#include "CSceneManager.h"
#include "CSound.h"
#include "CMoneyManager.h"

#define UI_OFFSET_Y 10.0f

#define SE_VOLUME 0.5f

// �R���X�g���N�^
CDoor::CDoor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject()
	, mpModel(model)
{
	// ���ʉ���ݒ�
	mpBuzzerSE = CResourceManager::Get<CSound>("BuzzerSE");
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
}

// �f�X�g���N�^
CDoor::~CDoor()
{
	SAFE_DELETE(mpCollider);
}

// ���ׂ�
void CDoor::Interact()
{
	auto* jobStatusManager = CJobStatusManager::Instance();
	EJobType jobType = jobStatusManager->GetSelectJob();

	bool isLoad = false;
	auto* sceneManager = CSceneManager::Instance();
	// �I������Ă���d�����������Ă���Ȃ�d���V�[���֑J��
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
	// ����ȊO
	default:
		// �u�U�[���Đ�
		mpBuzzerSE->Play(SE_VOLUME, true);
		break;
	}

	// �d���V�[�������[�h���Ă���Ȃ�
	if (isLoad)
	{
		// �����̊Ǘ��N���X���擾
		auto* moneyMgr = CMoneyManager::Instance();
		// �������o��
		moneyMgr->DayPass();
		// �����̕ԍϊz��ݒ�
		moneyMgr->SetDebtMoney(moneyMgr->GetDay());
		// �ԍς��Ă��Ȃ�
		moneyMgr->SetDid(false);
	}
}

// �`��
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
