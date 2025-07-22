#include "CColliderBox.h"
#include "glut.h"
#include "Maths.h"
#include "CColor.h"
#include "CColliderRectangle.h"

CColliderBox::CColliderBox(CObjectBase* owner, ELayer layer, 
	CVector min,CVector max,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eBox, isKinematic, weight)
{
	Set(min, max);
}

CColliderBox::~CColliderBox()
{
	mVertices.clear();
}

// �{�b�N�X�R���C�_�\���\�����钸�_�̐ݒ�
void CColliderBox::Set(CVector min, CVector max)
{
	mVertices.clear();
	// �O�ʁ@�E��
	CVector v0 = CVector(max.X(), max.Y(), max.Z());
	// �O�ʁ@����
	CVector v1 = CVector(min.X(), max.Y(), max.Z());
	// �O�ʁ@����
	CVector v2 = CVector(min.X(), min.Y(), max.Z());
	// �O�ʁ@�E��
	CVector v3 = CVector(max.X(), min.Y(), max.Z());
	// �w�ʁ@�E��
	CVector v4 = CVector(max.X(), max.Y(), min.Z());
	// �w�ʁ@����
	CVector v5 = CVector(min.X(), max.Y(), min.Z());
	// �w�ʁ@����
	CVector v6 = CVector(min.X(), min.Y(), min.Z());
	// �w�ʁ@�E��
	CVector v7 = CVector(max.X(), min.Y(), min.Z());

	// �O��
	SRVertex rv0(v0, v1, v2, v3);
	mVertices.push_back({ rv0,rv0 });
	// �w��
	SRVertex rv1(v4, v7, v6, v5);
	mVertices.push_back({ rv1,rv1 });
	// ����
	SRVertex rv2(v1, v5, v6, v2);
	mVertices.push_back({ rv2,rv2 });
	// �E��
	SRVertex rv3(v4, v0, v3, v7);
	mVertices.push_back({ rv3,rv3 });
	// ���
	SRVertex rv4(v4, v5, v1, v0);
	mVertices.push_back({ rv4,rv4 });
	// ����
	SRVertex rv5(v7, v3, v2, v6);
	mVertices.push_back({ rv5,rv5 });
}

const std::vector<SRVertexData>& CColliderBox::Get() const
{
	return mVertices;
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

	// �l�p�`��`��
	glBegin(GL_QUADS);
	for (const auto& v : mVertices)
	{
		glVertex3f(v.wv.V[0].X(), v.wv.V[0].Y(), v.wv.V[0].Z());
		glVertex3f(v.wv.V[1].X(), v.wv.V[1].Y(), v.wv.V[1].Z());
		glVertex3f(v.wv.V[2].X(), v.wv.V[2].Y(), v.wv.V[2].Z());
		glVertex3f(v.wv.V[3].X(), v.wv.V[3].Y(), v.wv.V[3].Z());
	}
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_ALPHA);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// �R���C�_�\�̏����X�V
void CColliderBox::UpdateCol(bool isInit)
{
	// �O��̍X�V�Ɠ����s��ł���΁A�������Ȃ�
	CMatrix m = Matrix();
	if (!isInit && m == mLastMtx) return;
	mLastMtx = m;

	// �s��𔽉f�����e���_�̍��W���v�Z
	// �e�l�p�`�̃o�E���f�B���O�{�b�N�X��ۑ�
	for (auto& v : mVertices)
	{
		v.wv.V[0] = v.lv.V[0] * m;
		v.wv.V[1] = v.lv.V[1] * m;
		v.wv.V[2] = v.lv.V[2] * m;
		v.wv.V[3] = v.lv.V[3] * m;
		v.bounds = CBounds::GetRectangleBounds(v.wv.V[0], v.wv.V[1], v.wv.V[2], v.wv.V[3]);
	}
}
