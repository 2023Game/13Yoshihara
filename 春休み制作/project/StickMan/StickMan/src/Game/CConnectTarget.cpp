#include "CConnectTarget.h"

// �^�[�Q�b�g�����̃f�o�b�O�\��
#if _DEBUG
#include "Primitive.h"
#endif

// �R���X�g���N�^
CConnectTarget::CConnectTarget()
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
