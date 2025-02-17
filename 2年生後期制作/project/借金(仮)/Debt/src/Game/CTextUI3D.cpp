#include "CTextUI3D.h"
#include "CFont.h"
#include FT_FREETYPE_H

// 3D��Ԃł̏k�ځi3D��Ԃł�1�̃T�C�Y�ɑ�������s�N�Z�����j
#define WORLD_UNIT_PER_PIXEL 256.0f

// �f�t�H���g�T�C�Y
#define DEFAULT_SIZE CVector2(512.0f,64.0f)

// �R���X�g���N�^
CTextUI3D::CTextUI3D(ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CTextUI2D(pauseType, addTaskList, fontPath)
	, mOffsetPos(0.0f, 0.0f)
	, mUV(0.0f, 0.0f, 1.0f, 1.0f)
	, mWolrdUnitPerPixel(WORLD_UNIT_PER_PIXEL)
	, mIsBillboard(false)
	, mIsDepthTest(true)
	, mIsDepthMask(false)
	, mIsLighting(false)
{
	// �e�N�X�`���ǂݍ���
	//mFontMaterial.LoadTexture(fontPath, fontPath, false);
	
	// �f�t�H���g�T�C�Y��ݒ�
	SetSize(DEFAULT_SIZE);

	SetColor(CColor::white);

	// UV�ݒ�
	SetUV(CRect(0.0f, 0.0f, 1.0f, 1.0f));
}

// �f�X�g���N�^
CTextUI3D::~CTextUI3D()
{
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

// �T�C�Y�擾
const CVector2& CTextUI3D::GetSize() const
{
	return mSize;
}

// �T�C�Y�ݒ�
void CTextUI3D::SetSize(const float& x, const float& y)
{
	SetSize(CVector2(x, y));
}

// �T�C�Y�ݒ�iCVector2�Łj
void CTextUI3D::SetSize(const CVector2& size)
{
	mSize = size;
	CVector2 s = mSize * (1.0f / mWolrdUnitPerPixel);

	// �O�p�`�̒��_���W�ݒ�
	mT[0].Vertex
	(
		CVector(s.X(), s.Y(), 0.0f),
		CVector(-s.X(), -s.Y(), 0.0f),
		CVector(s.X(), -s.Y(), 0.0f)
	);
	mT[1].Vertex
	(
		CVector(-s.X(), s.Y(), 0.0f),
		CVector(-s.X(), -s.Y(), 0.0f),
		CVector(s.X(), s.Y(), 0.0f)
	);
	// �@����Z������
	mT[0].Normal(CVector(0.0f, 0.0f, 1.0f));
	mT[1].Normal(CVector(0.0f, 0.0f, 1.0f));
}

// UV�擾
const CRect& CTextUI3D::GetUV() const
{
	return mUV;
}

// UV�ݒ�
void CTextUI3D::SetUV(const float& left, const float& top, const float& right, const float& bottom)
{
	CRect rect;
	rect.SetPos(left, top);
	rect.W(right - left);
	rect.H(bottom - top);
	SetUV(rect);
}

// UV�ݒ�iCRect�Łj
void CTextUI3D::SetUV(const CRect& uv)
{
	mUV = uv;

	float l = uv.X();
	float t = 1.0f - uv.Y();
	float r = l + uv.W();
	float b = t - uv.H();

	mT[0].UV
	(
		CVector(r, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, b, 0.0f)
	);
	mT[1].UV
	(
		CVector(l, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, t, 0.0f)
	);
}

// 3D��Ԃł̏k�ڂ�ݒ�
void CTextUI3D::SetWorldUnitPerPixel(float pixel)
{
	mWolrdUnitPerPixel = pixel;
	SetSize(mSize);
}

// �r���{�[�h�̃I���I�t��ݒ�
void CTextUI3D::SetBillboard(bool enable)
{
	mIsBillboard = enable;
}

// �f�v�X�e�X�g�̃I���I�t��ݒ�
void CTextUI3D::SetDepthTest(bool enable)
{
	mIsDepthTest = enable;
}

// �f�v�X�e�X�g�̃I���I�t��ݒ�
void CTextUI3D::SetDepthMask(bool enable)
{
	mIsDepthMask = enable;
}

// ���C�e�B���O�̃I���I�t��ݒ�
void CTextUI3D::SetLighting(bool enable)
{
	mIsLighting = enable;
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

	// ���g�̍s����擾
	CMatrix m = Matrix();

	// �r���{�[�h���L���Ȃ��
	if (mIsBillboard)
	{
		// ��ɃJ�����̕�������������
		CCamera* cam = CCamera::CurrentCamera();
		CMatrix camMtx = cam->GetViewMatrix().Inverse();
		camMtx.Position(CVector::zero);
		m = camMtx * m;
	}

	// �s��𔽉f
	glMultMatrixf(m.M());

	// �I�t�Z�b�g���W�𔽉f
	CVector2 offset = mOffsetPos * (1.0f / mWolrdUnitPerPixel);
	glTranslatef(offset.X(), offset.Y(), 0.0f);

	// �e�ݒ�̃t���O�̏�Ԃɍ��킹�āA�I�t�ɂ���
	if (!mIsDepthTest) glDisable(GL_DEPTH_TEST);	// �f�v�X�e�X�g
	if (!mIsDepthMask) glDepthMask(false);		// �f�v�X��������
	if (!mIsLighting) glDisable(GL_LIGHTING);		// ���C�e�B���O

	CTextUI2D::Render();

	// �e�ݒ�����ɖ߂�
	glEnable(GL_DEPTH_TEST);	// �f�v�X�e�X�g
	glDepthMask(true);			// �f�v�X��������
	glEnable(GL_LIGHTING);		// ���C�e�B���O


	// �s���߂�
	glPopMatrix();
	//Render(&mFontMaterial);
}

// �`��i�}�e���A���w��Łj
void CTextUI3D::Render(CMaterial* mpMaterial)
{
	// �s��̕ۑ�
	glPushMatrix();

	// ���g�̍s����擾
	CMatrix m = Matrix();

	// �r���{�[�h���L���Ȃ��
	if (mIsBillboard)
	{
		// ��ɃJ�����̕�������������
		CCamera* cam = CCamera::CurrentCamera();
		CMatrix camMtx = cam->GetViewMatrix().Inverse();
		camMtx.Position(CVector::zero);
		m = camMtx * m;
	}

	// �s��𔽉f
	glMultMatrixf(m.M());

	// �I�t�Z�b�g���W�𔽉f
	CVector2 offset = mOffsetPos * (1.0f / mWolrdUnitPerPixel);
	glTranslatef(offset.X(), offset.Y(), 0.0f);

	// �e�ݒ�̃t���O�̏�Ԃɍ��킹�āA�I�t�ɂ���
	if (!mIsDepthTest) glDisable(GL_DEPTH_TEST);	// �f�v�X�e�X�g
	if (!mIsDepthMask) glDepthMask(false);		// �f�v�X��������
	if (!mIsLighting) glDisable(GL_LIGHTING);		// ���C�e�B���O

	// �}�e���A���K�p
	mpMaterial->Enabled(mColor);
	//// �O�p�`�̕`��
	//mT[0].Render();
	//mT[1].Render();
	CTextUI2D::Render();
	// �}�e���A������
	mpMaterial->Disabled();

	// �e�ݒ�����ɖ߂�
	glEnable(GL_DEPTH_TEST);	// �f�v�X�e�X�g
	glDepthMask(true);			// �f�v�X��������
	glEnable(GL_LIGHTING);		// ���C�e�B���O


	// �s���߂�
	glPopMatrix();
}
