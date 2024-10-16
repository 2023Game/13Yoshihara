#include "CComputer.h"
#include "CGameMenu.h"
#include "CPlayer.h"

CComputer::CComputer(CModel* model, const CVector& pos, const CVector& scale, const CVector& rotation)
	: mpModel(model)
{
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);
	Rotate(rotation);

	//�d���I�����j���[���쐬
	mpJobMenu = new CGameMenu();
}

CComputer::~CComputer()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CComputer::Update()
{
	//�v���C���[�̃C���^���N�g�I�u�W�F�N�g���R���s���[�^�Ȃ�
	if (CPlayer::Instance()->GetInteractObject() == "eComputer")
	{
		Open();
	}
	else
	{
		Close();
	}
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}

void CComputer::Open()
{
	//�d���I�����j���[���J���Ă��Ȃ��Ȃ�J��
	if (!mpJobMenu->IsOpened())
	{
		mpJobMenu->Open();
	}
}

void CComputer::Close()
{
	//�d���I�����j���[���J���Ă��������
	if (mpJobMenu->IsOpened())
	{
		mpJobMenu->Close();
	}
}
