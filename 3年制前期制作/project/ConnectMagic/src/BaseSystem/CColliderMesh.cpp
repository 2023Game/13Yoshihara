#include "CColliderMesh.h"

CColliderMesh::CColliderMesh(CObjectBase* owner, ELayer layer, CModel* model,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eMesh, isKinematic, weight)
{
	Set(model);
}

CColliderMesh::~CColliderMesh()
{
	mVertices.clear();
}

void CColliderMesh::Set(CModel* model)
{
	mVertices.clear();
	if (model == nullptr) return;

	auto triangles = model->Triangles();
	int count = triangles.size();
	for (auto& tri : triangles)
	{
		STVertex v(tri.V0(), tri.V1(), tri.V2());
		mVertices.push_back({ v, v });
	}
}

const std::list<STVertexData>& CColliderMesh::Get() const
{
	return mVertices;
}

// �R���C�_�\�`��
void CColliderMesh::Render()
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// �A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	// �u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ΐF�ݒ�
	CColor col = CColor::green;
	if (!IsEnable()) col = CColor::gray;
	col.A(0.2f);
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// �O�p�`��`��
	glBegin(GL_TRIANGLES);
	for (const auto& v : mVertices)
	{
		glVertex3f(v.wv.V[0].X(), v.wv.V[0].Y(), v.wv.V[0].Z());
		glVertex3f(v.wv.V[1].X(), v.wv.V[1].Y(), v.wv.V[1].Z());
		glVertex3f(v.wv.V[2].X(), v.wv.V[2].Y(), v.wv.V[2].Z());
	}
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_ALPHA);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// �R���C�_�[�̏����X�V
void CColliderMesh::UpdateCol()
{
	// �s��𔽉f�����e���_�̍��W���v�Z
	// �e�O�p�`�̃o�E���f�B���O�{�b�N�X��ۑ�
	CMatrix m = Matrix();
	for (auto& v : mVertices)
	{
		v.wv.V[0] = v.lv.V[0] * m;
		v.wv.V[1] = v.lv.V[1] * m;
		v.wv.V[2] = v.lv.V[2] * m;
		v.bounds = CBounds::GetTriangleBounds(v.wv.V[0], v.wv.V[1], v.wv.V[2]);
	}
}
