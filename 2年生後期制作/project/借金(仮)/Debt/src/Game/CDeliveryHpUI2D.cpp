#include "CDeliveryHpUI2D.h"
#include "CImage.h"
#include "CTextUI2D.h"

// �z�B���̉摜�̃p�X
#define DELIVERY_ITEM_PATH "UI/deliveryItem.png"
// �z�B���̉摜�̃T�C�Y�̔{��
#define DELIVERY_ITEM_SIZE 0.5f
// �e�L�X�g�̃t�H���g�T�C�Y
#define TEXT_FONT_SIZE 64
// �e�L�X�g�̃I�t�Z�b�g���W
#define TEXT_OFFSET_POS CVector2(WINDOW_WIDTH * 0.125f,WINDOW_HEIGHT * 0.1f)

// �R���X�g���N�^
CDeliveryHpUI2D::CDeliveryHpUI2D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpDeliveryItemImg(nullptr)
	, mCurrPoint(0)
{
	// �z�B���̃C���[�W��ǂݍ���
	mpDeliveryItemImg = new CImage
	(
		DELIVERY_ITEM_PATH,
		ETaskPriority::eUI,0,
		ETaskPauseType::eGame,
		false,false
	);
	// �T�C�Y�{����K�p
	CVector2 size = mpDeliveryItemImg->GetSize();
	mpDeliveryItemImg->SetSize(size * DELIVERY_ITEM_SIZE);

	// �����Ă���z�B���̐��̃e�L�X�g�𐶐�
	mpNumText = new CTextUI2D
	(
		ETaskPauseType::eGame,
		false
	);
	// �e�L�X�g�̃t�H���g�T�C�Y��ݒ�
	mpNumText->SetFontSize(TEXT_FONT_SIZE);
	// ���W��ݒ�
	mpNumText->Position(TEXT_OFFSET_POS);
}

// �f�X�g���N�^
CDeliveryHpUI2D::~CDeliveryHpUI2D()
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
void CDeliveryHpUI2D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ���ݒl��ݒ�
void CDeliveryHpUI2D::SetCurrPoint(int point)
{
	mCurrPoint = point;
}

// �X�V
void CDeliveryHpUI2D::Update()
{
	mpDeliveryItemImg->Update();
	// �e�L�X�g��ݒ�
	mpNumText->ChangeToStr("�~%d\n", mCurrPoint);
	mpNumText->Update();
}

// �`��
void CDeliveryHpUI2D::Render()
{
	mpDeliveryItemImg->Render();
	mpNumText->Render();
}