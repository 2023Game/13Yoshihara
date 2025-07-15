#include "CPortal.h"
#include "CSceneManager.h"
#include "CColliderSphere.h"

// ��]���x
#define ROT_SPEED 200.0f

// �R���C�_�[�̔��a
#define RADIUS 5.0f

// �傫��
#define SIZE 2.0f

// �R���X�g���N�^
CPortal::CPortal()
	: CImage3D("Effect/Portal.png",
		ETag::ePortal, ETaskPriority::eEffect, 0,
		ETaskPauseType::eGame)
{
	// ���ʕ`�悷��
	SetBackFace(true);
	// �R���C�_�[�𐶐�
	CreateCol();

	// ��]����
	SetRotate(true);
	// ��]���x��ݒ�
	SetRotSpeed(ROT_SPEED);

	// �傫����ݒ�
	SetSize(GetSize() * SIZE);
}

// �f�X�g���N�^
CPortal::~CPortal()
{
	SAFE_DELETE(mpCol);
}

// �Փˏ���
void CPortal::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCol)
	{
		// �v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �V�[���Ǘ��N���X
			CSceneManager* SceneMgr = CSceneManager::Instance();
			// ���݂̃V�[��
			EScene CurrentScene = SceneMgr->GetCurrentScene();
			// ���̃V�[��
			EScene NextScene = EScene::eMap1;
			// ���݂̃V�[�����玟�̃V�[��������
			switch (CurrentScene)
			{
			case EScene::eMap1:		NextScene = EScene::eMap2;  break;
			case EScene::eMap2:		NextScene = EScene::eMap3;	break;
			case EScene::eMap3:		NextScene = EScene::eMap1;	break;
			case EScene::eGameTest:	NextScene = EScene::eGameTest;	break;
			}

			// ���̃X�e�[�W��
			CSceneManager::Instance()->LoadScene(NextScene);
		}
	}
}

// �R���C�_�[�𐶐�
void CPortal::CreateCol()
{
	mpCol = new CColliderSphere
	(
		this, ELayer::ePortal,
		RADIUS, true
	);
	// �v���C���[�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::ePlayer });
}
