#include "CFieldBase.h"
#include "CSceneManager.h"
#include <assert.h>

// �R���X�g���N�^
CFieldBase::CFieldBase()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpSkyModel(nullptr)
	, mpGroundColliderMesh(nullptr)
	, mpWallColliderMesh(nullptr)
	, mpObjectColliderMesh(nullptr)
{
}

// �f�X�g���N�^
CFieldBase::~CFieldBase()
{
	SAFE_DELETE(mpGroundColliderMesh);
	SAFE_DELETE(mpWallColliderMesh);
	SAFE_DELETE(mpObjectColliderMesh);
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

// �R���C�_�\�𐶐�����
void CFieldBase::CreateCol(std::string groundCol, std::string wallCol, std::string objCol)
{
	if (groundCol != "")
	{
		mpGroundColliderMesh = new CColliderMesh
		{
			this,ELayer::eGround,
			CResourceManager::Get<CModel>(groundCol),
			true
		};
	}
	if (wallCol != "")
	{
		mpWallColliderMesh = new CColliderMesh
		{
			this,ELayer::eWall,
			CResourceManager::Get<CModel>(wallCol),
			true
		};
	}
	if (objCol != "")
	{
		mpObjectColliderMesh = new CColliderMesh
		{
			this,ELayer::eObject,
			CResourceManager::Get<CModel>(objCol),
			true
		};
	}
}
