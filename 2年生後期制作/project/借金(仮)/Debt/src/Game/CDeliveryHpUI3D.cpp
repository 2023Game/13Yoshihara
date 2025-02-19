#include "CDeliveryHpUI3D.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "CTextUI3D.h"
#include "Maths.h"

#define WORLD_UNIT_PER_PIXEL 32.0f

// �z�B���̉摜�̃p�X
#define DELIVERY_ITEM_PATH "UI/deliveryItem.png"
// �z�B���̉摜�̃T�C�Y�̔{��
#define DELIVERY_ITEM_SIZE 1.0f

// �e�L�X�g�̃t�H���g�T�C�Y
#define TEXT_FONT_SIZE 32
// �e�L�X�g�̃I�t�Z�b�g���W
#define TEXT_OFFSET_POS CVector2(50.0f, -1500.0f)


// �R���X�g���N�^
CDeliveryHpUI3D::CDeliveryHpUI3D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpDeliveryItemImg(nullptr)
	, mpNumText(nullptr)
	, mCurrPoint(1)
{
	// �z�B���̃C���[�W��ǂݍ���
	mpDeliveryItemImg = new CImage3D
	(
		DELIVERY_ITEM_PATH,
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	// �k�ڐݒ�
	mpDeliveryItemImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	// �T�C�Y�{����K�p
	CVector2 size = mpDeliveryItemImg->GetSize();
	mpDeliveryItemImg->SetSize(size * DELIVERY_ITEM_SIZE);

	// �����Ă���z�B���̐��̃e�L�X�g�𐶐�
	mpNumText = new CTextUI3D
	(
		ETaskPauseType::eGame,
		false
	);
	// �k�ڐݒ�
	mpNumText->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	// �e�L�X�g�̃t�H���g�T�C�Y��ݒ�
	mpNumText->SetFontSize(TEXT_FONT_SIZE);
	// �e�L�X�g�̃I�t�Z�b�g���W��ݒ�
	mpNumText->SetOffsetPos(TEXT_OFFSET_POS);
}

//�f�X�g���N�^
CDeliveryHpUI3D::~CDeliveryHpUI3D()
{
	// �ǂݍ��񂾃C���[�W���폜
	SAFE_DELETE(mpDeliveryItemImg);
	// �e�L�X�g���폜
	SAFE_DELETE(mpNumText);

	// ������ɍ폜���ꂽ���Ƃ�`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// �������ݒ�
void CDeliveryHpUI3D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ���ݒl��ݒ�
void CDeliveryHpUI3D::SetCurrPoint(int point)
{
	mCurrPoint = point;
}

// �X�V
void CDeliveryHpUI3D::Update()
{
	mpDeliveryItemImg->Update();
	// �e�L�X�g��ݒ�
	mpNumText->ChangeToStr("�~%d\n", mCurrPoint);
	mpNumText->Update();
}

// �`��
void CDeliveryHpUI3D::Render()
{	
	// �s��̕ۑ�
	glPushMatrix();

	// ��ɃJ�����̕�������������
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	mpDeliveryItemImg->Render();
	mpNumText->Render();

	// �s���߂�
	glPopMatrix();
}
