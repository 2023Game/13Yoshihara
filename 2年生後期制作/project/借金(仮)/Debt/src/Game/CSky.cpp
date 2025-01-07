#include "CSky.h"
#include "CModel.h"

// �R���X�g���N�^
CSky::CSky(CModel* model,
	const CVector& pos, const CVector& scale, const CVector& rotation)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mpModel(model)
{
	Position(pos);
	Scale(scale);
	Rotate(rotation);
}

// �f�X�g���N�^
CSky::~CSky()
{
}

// �X�V
void CSky::Update()
{
}

// �`��
void CSky::Render()
{
	mpModel->Render(Matrix());
}
