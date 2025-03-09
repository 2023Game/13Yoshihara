#include "CComputer.h"
#include "CColliderSphere.h"
#include "CBuyMenu.h"
#include "CInput.h"

#define UI_OFFSET_Y 2.5f

CComputer::CComputer(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject()
	, mpModel(model)
{
	// UI�̃I�t�Z�b�g�ʒu��ݒ�
	mUIOffsetPos = CVector(0.0f, UI_OFFSET_Y, 0.0f);

	mpCollider = new CColliderSphere
	{
		this,ELayer::eInteractObj,
		4.0f
	};
	// �R���C�_�̈ʒu����
	mpCollider->Position(1.0f, 1.0f, 0.0f);
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer,ELayer::eInteractSearch });

	Position(pos);
	Scale(scale);
	Rotate(rotation);

	mInteractStr = "�p�\�R�����g�p";

	// �w�����j���[���쐬
	mpBuyMenu = new CBuyMenu();
}

CComputer::~CComputer()
{
	SAFE_DELETE(mpCollider);
}

// ���ׂ�
void CComputer::Interact()
{
	// �w�����j���[���J���Ă��Ȃ����
	if (!mpBuyMenu->IsOpened())
	{
		// �J��
		mpBuyMenu->Open();
	}
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}
