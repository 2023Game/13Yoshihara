#include "CShield.h"
#include "CColliderRectangle.h"
#include "CImage3D.h"
#include "CConnectPointManager.h"

// ���_
#define VERT_POS_1 -10.0f,	40.0f,	0.0f
#define VERT_POS_2 -10.0f,	0.0f,	0.0f
#define VERT_POS_3  10.0f,	0.0f,	0.0f
#define VERT_POS_4  10.0f,	40.0f,	0.0f

// �R���X�g���N�^
CShield::CShield()
	: CObjectBase(ETag::eField, ETaskPriority::eShield, 0, ETaskPauseType::eGame)
{
	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CShield::~CShield()
{
	SAFE_DELETE(mpCol1);
	SAFE_DELETE(mpCol2);
}

// �`��
void CShield::Render()
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
	// ���ʕ`��
	glDisable(GL_CULL_FACE);

	// DIFFUSE�F�ݒ�
	CColor col = CColor::cyan;
	col.A(0.2f);
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// �l�p�`��`��
	glBegin(GL_QUADS);
	glVertex3f(VERT_POS_1);
	glVertex3f(VERT_POS_2);
	glVertex3f(VERT_POS_3);
	glVertex3f(VERT_POS_4);
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// ���ʕ`��
	glEnable(GL_CULL_FACE);
	// �A���t�@�u�����h����
	glDisable(GL_ALPHA);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// �R���C�_�[�𐶐�
void CShield::CreateCol()
{
	// �l�p�`�R���C�_�[�𐶐�
	mpCol1 = new CColliderRectangle(
		this, ELayer::eWall,
		CVector(VERT_POS_1),
		CVector(VERT_POS_2),
		CVector(VERT_POS_3),
		CVector(VERT_POS_4),
		true
	);
	mpCol2 = new CColliderRectangle(
		this, ELayer::eWall,
		CVector(VERT_POS_4),
		CVector(VERT_POS_3),
		CVector(VERT_POS_2),
		CVector(VERT_POS_1),
		true
	);
	// �������炷
	mpCol1->Position(VectorZ());
	mpCol2->Position(-VectorZ());
	// �I�u�W�F�N�g�Ƃ����Փ˔���
	mpCol1->SetCollisionLayers({ ELayer::eObject });
	mpCol2->SetCollisionLayers({ ELayer::eObject });

	// �ڑ����̊Ǘ��N���X
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// �Փ˔��肷��R���C�_�[�ɒǉ�
	pointMgr->AddCollider(mpCol1);
	pointMgr->AddCollider(mpCol2);
}
