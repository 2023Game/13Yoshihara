#include "CConnectObject.h"
#include "CConnectTarget.h"

// �R���X�g���N�^
CConnectObject::CConnectObject(float weight, ETaskPriority prio,
	int sortOrder,ETaskPauseType pause)
	: CObjectBase(ETag::eConnectObject, prio, 0, pause)
	, mWeight(weight)
{
}

// �f�X�g���N�^
CConnectObject::~CConnectObject()
{
}

// �I�u�W�F�N�g�폜��`����֐�
void CConnectObject::DeleteObject(CObjectBase* obj)
{
}

// �X�V
void CConnectObject::Update()
{
}

// �Փˏ���
void CConnectObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// ��������ꂽ���̏���
void CConnectObject::Pull()
{
}

// �ڑ��^�[�Q�b�g�̍쐬
void CConnectObject::CreateTarget(CVector pos)
{
	// �^�[�Q�b�g����
	CConnectTarget* target = new CConnectTarget();
	// �ʒu�ݒ�
	target->Position(pos);
	// ���X�g�ɒǉ�
	mTargets.push_back(target);
}
