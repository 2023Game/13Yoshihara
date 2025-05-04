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

// �ǂ̃I�u�W�F�N�g�����ɕ`�悷��
void CSky::Render()
{
	glDepthRange(0.99, 1.0); // �����ʒu�ɕ`��
	
	mpModel->Render(Matrix());

	glDepthRange(0.0, 1.0); // �ʏ�̕`��ݒ�ɖ߂�
}
