#include "CColliderBox.h"
#include "glut.h"
#include "Maths.h"
#include "CColor.h"
#include "CColliderRectangle.h"

CColliderBox::CColliderBox(CObjectBase* owner, ELayer layer, 
	const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
	const CVector& v4, const CVector& v5, const CVector& v6, const CVector& v7,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eBox, isKinematic, weight)
{
	Set(owner, layer, v0, v1, v2, v3, v4, v5, v6, v7, isKinematic, weight);
}

// �{�b�N�X�R���C�_�\���\������l�p�`�R���C�_�̐ݒ�
void CColliderBox::Set(CObjectBase* owner, ELayer layer, 
	const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
	const CVector& v4, const CVector& v5, const CVector& v6, const CVector& v7,
	bool isKinematic, float weight)
{
	// �O��
	mRectangles.push_back(CColliderRectangle(owner, layer, v0, v1, v2, v3, isKinematic, weight));
	// �w��
	mRectangles.push_back(CColliderRectangle(owner, layer, v4, v5, v6, v7, isKinematic, weight));
	// ����
	mRectangles.push_back(CColliderRectangle(owner, layer, v4, v5, v1, v0, isKinematic, weight));
	// �E��
	mRectangles.push_back(CColliderRectangle(owner, layer, v3, v2, v6, v7, isKinematic, weight));
	// ���
	mRectangles.push_back(CColliderRectangle(owner, layer, v4, v0, v3, v7, isKinematic, weight));
	// ���
	mRectangles.push_back(CColliderRectangle(owner, layer, v1, v5, v6, v2, isKinematic, weight));
}

// �l�p�`�R���C�_�[�̔z���Ԃ�
const std::vector<CColliderRectangle>& CColliderBox::Get() const
{
	return mRectangles;
}

// �R���C�_�\�`��
void CColliderBox::Render()
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// ���g�̍s���K�p
	glMultMatrixf(Matrix().M());

	// �A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	// �u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	CColor col = CColor::red;
	if (!IsEnable()) col = CColor::gray;
	col.A(0.2f);
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// �{�b�N�X��`��
	glBegin(GL_QUADS);
	for (int i = 0; i < mRectangles.size(); i++)
	{
		CVector v0, v1, v2, v3;
		mRectangles[i].Get(&v0, &v1, &v2, &v3);
		glVertex3fv(v0.X(), v0.Y(), v0.Z());
	}
	// �O��
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// �O�ʍ���
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// �O�ʍ���
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// �O�ʉE��
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// �O�ʉE��

	// �w��
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// �w�ʍ���
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// �w�ʉE��
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// �w�ʉE��
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// �w�ʍ���

	// ������
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// �w�ʍ���
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// �w�ʍ���
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// �O�ʍ���
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// �O�ʍ���

	// �E����
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// �O�ʉE��
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// �O�ʉE��
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// �w�ʉE��
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// �w�ʉE��

	// ���
	glVertex3f(mV[4].X(), mV[4].Y(), mV[4].Z());	// �w�ʍ���
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());	// �O�ʍ���
	glVertex3f(mV[3].X(), mV[3].Y(), mV[3].Z());	// �O�ʉE��
	glVertex3f(mV[7].X(), mV[7].Y(), mV[7].Z());	// �w�ʉE��

	// ���
	glVertex3f(mV[5].X(), mV[5].Y(), mV[5].Z());	// �w�ʍ���
	glVertex3f(mV[6].X(), mV[6].Y(), mV[6].Z());	// �w�ʉE��
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());	// �O�ʉE��
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());	// �O�ʍ���
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_ALPHA);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// �R���C�_�\�̏����X�V
void CColliderBox::UpdateCol()
{
	// �s��𔽉f�����e���_�̍��W���v�Z
	CMatrix m = Matrix();
	mWV[0] = mV[0] * m;
	mWV[1] = mV[1] * m;
	mWV[2] = mV[2] * m;
	mWV[3] = mV[3] * m;
	mWV[4] = mV[4] * m;
	mWV[5] = mV[5] * m;
	mWV[6] = mV[6] * m;
	mWV[7] = mV[7] * m;

	// �o�E���e�B���O�{�b�N�X���X�V
	mBounds = CBounds::GetBoxBounds(
		mWV[0], mWV[1], mWV[2], mWV[3],
		mWV[4], mWV[5], mWV[6], mWV[7]);
}
