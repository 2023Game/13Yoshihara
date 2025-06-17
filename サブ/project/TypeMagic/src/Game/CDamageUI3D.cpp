#include "CDamageUI3D.h"
#include "Maths.h"

// ���W�̂Ԃ�
#define POS_RAND 5.0f

// �_���[�W�\�L�̍폜�܂ł̎���
#define DAMAGE_TEXT_DELETE_TIME 1.0f

// �e�L�X�g�̍ŏ��t�H���g�T�C�Y
#define TEXT_FONT_SIZE_MIN 8
// �e�L�X�g�̍ő�t�H���g�T�C�Y
#define TEXT_FONT_SIZE_MAX 32

// �e�L�X�g�T�C�Y���ŏ��ɂȂ鋗��
#define TEXT_FONT_SIZE_MIN_DIST 500.0f
// �e�L�X�g�T�C�Y���ő�ɂȂ鋗��
#define TEXT_FONT_SIZE_MAX_DIST 50.0f

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

	CCamera* camera = CCamera::CurrentCamera();
	// ����
	float dist = CVector::Distance(mWorldPos, camera->Position());
	// �����ɂ���ăT�C�Y�����߂�
	float size = Math::Lerp(TEXT_FONT_SIZE_MAX, TEXT_FONT_SIZE_MIN, (dist - TEXT_FONT_SIZE_MAX_DIST) / (TEXT_FONT_SIZE_MIN_DIST - TEXT_FONT_SIZE_MAX_DIST));
	// �t�H���g�T�C�Y��ݒ�
	SetFontSize(size);

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