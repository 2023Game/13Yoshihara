#include "CConnectTarget.h"
#include "CConnectObject.h"

// �^�[�Q�b�g�����̃f�o�b�O�\��
#if _DEBUG
#include "Primitive.h"
#endif

// �R���X�g���N�^
CConnectTarget::CConnectTarget(CConnectObject* connectObj)
	: mpConnectObj(connectObj)
{
}

// �f�X�g���N�^
CConnectTarget::~CConnectTarget()
{
}

// �`��
void CConnectTarget::Render()
{
	// �^�[�Q�b�g�����̃f�o�b�O�\��
#if _DEBUG
	Primitive::DrawWireBox
	(
		Position(),
		Scale(),
		CColor::red
	);
#endif
}

// �ڑ��������Ă���I�u�W�F�N�g���擾
CConnectObject* CConnectTarget::GetConnectObj()
{
	return mpConnectObj;
}
