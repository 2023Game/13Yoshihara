#include "CDamageUI3D.h"
#include "Maths.h"

// ���W�̂Ԃ�
#define POS_RAND 5.0f

// �_���[�W�\�L�̍폜�܂ł̎���
#define DAMAGE_TEXT_DELETE_TIME 1.0f

// �R���X�g���N�^
CDamageUI3D::CDamageUI3D(CVector pos)
	: CTextUI3D(RandPos(pos))
	, mAutoDeleteTime(DAMAGE_TEXT_DELETE_TIME)
{
}

// �f�X�g���N�^
CDamageUI3D::~CDamageUI3D()
{
}

// �X�V
void CDamageUI3D::Update()
{

	CTextUI3D::Update();

	// �����폜�܂ł̎��Ԃ��o�߂�����
	mAutoDeleteTime -= Times::DeltaTime();
	if (mAutoDeleteTime < 0.0f)
	{
		// �폜
		Kill();
	}
}

// ���W���Ԃ炵�Đݒ�
CVector CDamageUI3D::RandPos(const CVector& pos)
{
	// �Ԃ炵�����W��ݒ�
	CVector randPos = pos;
	randPos.X(randPos.X() + Math::Rand(-POS_RAND, POS_RAND));
	randPos.Y(randPos.Y() + Math::Rand(-POS_RAND, POS_RAND));
	randPos.Z(randPos.Z() + Math::Rand(-POS_RAND, POS_RAND));

	return randPos;
}