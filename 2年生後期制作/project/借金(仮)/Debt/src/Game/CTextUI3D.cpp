#include "CTextUI3D.h"
#include "CFont.h"
#include <stdarg.h>
#include <glut.h>

// �R���X�g���N�^
CTextUI3D::CTextUI3D(ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CTextUI2D(pauseType, addTaskList, fontPath)
	, mOffsetPos(0.0f,0.0f)
{

}

// �f�X�g���N�^
CTextUI3D::~CTextUI3D()
{
}

// �X�V
void CTextUI3D::Update()
{
	CTextUI2D::Update();
}

// �`��
void CTextUI3D::Render()
{
	// �s��̕ۑ�
	glPushMatrix();

	// �I�t�Z�b�g���W�𔽉f
	CVector2 offset = mOffsetPos;
	glTranslatef(offset.X(), offset.Y(), 0.0f);

	CTextUI2D::Render();

	// �s���߂�
	glPopMatrix();
}

// 2D��Ԃł̃I�t�Z�b�g���W���擾
const CVector2& CTextUI3D::GetOffsetPos() const
{
	return mOffsetPos;
}

// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�
void CTextUI3D::SetOffsetPos(const float& x, const float& y)
{
	SetOffsetPos(CVector2(x, y));
}

// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�iCVector2�Łj
void CTextUI3D::SetOffsetPos(const CVector2& pos)
{
	mOffsetPos = pos;
}
