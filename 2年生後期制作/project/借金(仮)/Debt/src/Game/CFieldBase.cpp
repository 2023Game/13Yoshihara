#include "CFieldBase.h"
#include "CSceneManager.h"
#include <assert.h>

// �t�B�[���h�̃C���X�^���X
CFieldBase* CFieldBase::spInstance = nullptr;

// �t�B�[���h�̃C���X�^���X��Ԃ�
CFieldBase* CFieldBase::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CFieldBase::CFieldBase()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpSkyModel(nullptr)
	, mpGroundColliderMesh(nullptr)
	, mpWallColliderMesh(nullptr)
	, mpObjectColliderMesh(nullptr)
{
	//assert(spInstance == nullptr);
	spInstance = this;
}

// �f�X�g���N�^
CFieldBase::~CFieldBase()
{
	SAFE_DELETE(mpGroundColliderMesh);
	SAFE_DELETE(mpWallColliderMesh);
	SAFE_DELETE(mpObjectColliderMesh);

	// �C���X�^���X�ƍ폜���Ă���t�B�[���h������Ȃ�폜
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

//���C�ƃt�B�[���h�I�u�W�F�N�g�̏Փ˔���
bool CFieldBase::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����t���O
	bool isHit = false;
	// �t�B�[���h�̒n�ʂƂ̏Փ˔���
	if (CCollider::CollisionRay(mpGroundColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}
	// �t�B�[���h�̕ǂƂ̏Փ˔���
	if (CCollider::CollisionRay(mpWallColliderMesh, start, end, &tHit))
	{
		// �܂����ɏՓ˂��Ă��Ȃ��ꍇ���A
		// ���ɏՓ˂��Ă���R���C�_�\���߂��ꍇ�́A
		if (!isHit || tHit.dist < hit->dist)
		{
			// �Փˏ����X�V
			*hit = tHit;
			isHit = true;
		}
	}
	// �t�B�[���h�̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpObjectColliderMesh, start, end, &tHit))
	{
		// �܂����ɏՓ˂��Ă��Ȃ��ꍇ���A
		// ���ɏՓ˂��Ă���R���C�_�\���߂��ꍇ�́A
		if (!isHit || tHit.dist < hit->dist)
		{
			// �Փˏ����X�V
			*hit = tHit;
			isHit = true;
		}
	}
	
	return isHit;
}

// �X�V
void CFieldBase::Update()
{
}

// �`��
void CFieldBase::Render()
{
	mpModel->Render(Matrix());
}

// �n�ʂ̃R���C�_�\�擾
CCollider* CFieldBase::GetGroundCol()
{
	return mpGroundColliderMesh;
}

// �ǂ̃R���C�_�\�擾
CCollider* CFieldBase::GetWallCol()
{
	return mpWallColliderMesh;
}

// �I�u�W�F�N�g�̃R���C�_�\�擾
CCollider* CFieldBase::GetObjCol()
{
	return mpObjectColliderMesh;
}

// �o�H�T���p�̃m�[�h���쐬
void CFieldBase::CreateNavNodes()
{
}
