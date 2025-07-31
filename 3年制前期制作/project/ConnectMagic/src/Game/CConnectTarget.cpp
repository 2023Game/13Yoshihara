#include "CConnectTarget.h"
#include "CConnectObject.h"

// �^�[�Q�b�g�����̃f�o�b�O�\��
#if _DEBUG
#include "Primitive.h"
#endif

// �R���X�g���N�^
CConnectTarget::CConnectTarget(CConnectObject* connectObj)
	: CObjectBase(ETag::eConnectTarget, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpConnectObj(connectObj)
{
}

// �f�X�g���N�^
CConnectTarget::~CConnectTarget()
{
	if (mpConnectObj != nullptr)
	{
		mpConnectObj->DeleteObject(this);
		mpConnectObj = nullptr;
	}
}

// �`��
void CConnectTarget::Render()
{
	// �^�[�Q�b�g�����̃f�o�b�O�\��
#if _DEBUG
	//Primitive::DrawWireBox
	//(
	//	Position(),
	//	Scale(),
	//	CColor::red
	//);
#endif
}

// �ڑ��������Ă���I�u�W�F�N�g���擾
CConnectObject* CConnectTarget::GetConnectObj() const
{
	return mpConnectObj;
}

// �ڑ��������Ă���I�u�W�F�N�g��ݒ�
void CConnectTarget::SetConnectObj(CConnectObject* obj)
{
	mpConnectObj = obj;
}
