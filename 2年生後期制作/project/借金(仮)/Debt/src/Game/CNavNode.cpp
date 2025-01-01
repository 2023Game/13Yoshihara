#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

// �m�[�h��Y���W�̃I�t�Z�b�g�l
#define NODE_OFFSET_Y 5.0f

// �R���X�g���N�^
CNavNode::CNavNode(const CVector& pos, bool isDestNode)
	: mIsDestNode(isDestNode)
	, mPosition(pos)
	, mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
	// ���W��ݒ�
	SetPos(mPosition);

}

// �f�X�g���N�^
CNavNode::~CNavNode()
{
}

// �m�[�h�̍��W���擾
const CVector& CNavNode::GetPos() const
{
	return mPosition;
}

// �Օ����`�F�b�N��\���p�ɁA������ɏグ���m�[�h�̍��W��ς���
CVector CNavNode::GetOffsetPos() const
{
	// �n�ʂƏd�Ȃ�Ȃ��悤�ɁA�m�[�h�̈ʒu���班����ɂ������ʒu��Ԃ�
	return mPosition + CVector(0.0f, NODE_OFFSET_Y, 0.0f);
}

// �m�[�h�̍��W��ݒ�
void CNavNode::SetPos(const CVector& pos)
{
	// �m�[�h�̍��W���X�V
	mPosition = pos;

	// TODO�F�m�[�h�̍��W���ς�����̂ŁA�ڑ����Ă���m�[�h�𒲂ג���
}

// �m�[�h�̐F�ݒ�i�f�o�b�O�p�j
void CNavNode::SetColor(const CColor& color)
{
	mColor.Set(color.R(), color.G(), color.B());
}

// �m�[�h��`��i�f�o�b�O�p�j
void CNavNode::Render()
{
	// �m�[�h�̍��W�ɋ���`��
	CMatrix m;
	m.Translate(GetOffsetPos());
	Primitive::DrawWireSphere(m, 1.0f, mColor);
}
