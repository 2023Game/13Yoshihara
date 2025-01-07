#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

// �m�[�h��Y���W�̃I�t�Z�b�g�l
#define NODE_OFFSET_Y 5.0f
// �T���m�[�h�̋����̌��E�l
#define FIND_NODE_DISTANCE 70.0f

// �R���X�g���N�^
CNavNode::CNavNode(const CVector& pos, bool isDestNode)
	: mIsDestNode(isDestNode)
	, mPosition(pos)
	, mIsEnable(true)
	, mCalcMoveCost(-1.0f)
	, mpCalcFromNode(nullptr)
	, mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
	// �Ǘ��N���X�̃��X�g�Ɏ��g��ǉ�
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->AddNode(this);
	}
	// ���W��ݒ�
	SetPos(mPosition);
}

// �f�X�g���N�^
CNavNode::~CNavNode()
{
	// �Ǘ��N���X�̃��X�g���玩�M����菜��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->RemoveNode(this);
	}
}

// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
void CNavNode::ResetCalcData()
{
	mCalcMoveCost = -1.0f;
	mpCalcFromNode = nullptr;
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

	// �m�[�h�̍��W���ς�����̂ŁA�ڑ����Ă���m�[�h�𒲂ג���
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->FindConnectNavNodes(this, FIND_NODE_DISTANCE);
	}
}

// �m�[�h���L�����ǂ������擾
bool CNavNode::IsEnable()
{
	return mIsEnable;
}

// �m�[�h���L�����ǂ�����ݒ�
void CNavNode::SetEnable(bool isEnable)
{
	// �����Ȃ珈�������Ȃ�
	if (mIsEnable == isEnable) return;
	// �����ɂȂ�Ȃ�ڑ����Ă���S�Ẵm�[�h������
	if (isEnable == false)
	{
		ClearConnects();
	}

	mIsEnable = isEnable;
}

// �ڑ�����m�[�h��ǉ�
void CNavNode::AddConnect(CNavNode* node)
{
	// ���ɐڑ����X�g�ɓo�^���Ă���m�[�h�ł���΁A�X���[
	for (CNavConnectData& connect : mConnectData)
	{
		if (connect.node == node) return;
	}

	// �ڑ�����m�[�h�܂ł̋������R�X�g�Ƃ���
	float cost = (node->GetPos() - mPosition).Length();

	// ���M�Ƒ��肻�ꂼ��̐ڑ����Ă���m�[�h���X�g�ɂ��݂���ݒ�
	mConnectData.push_back(CNavConnectData(node, cost));
	node->mConnectData.push_back(CNavConnectData(this, cost));
}

// �ڑ����Ă���m�[�h����菜��
void CNavNode::RemoveConnect(CNavNode* node)
{
	auto itr = mConnectData.begin();
	auto end = mConnectData.end();
	while (itr != end)
	{
		// ��v����m�[�h��������΁A���X�g�����菜��
		if (itr->node == node)
		{
			itr = mConnectData.erase(itr);
			continue;
		}
		itr++;
	}
}

// �ڑ����Ă���S�Ẵm�[�h������
void CNavNode::ClearConnects()
{
	// �ڑ�����̐ڑ����X�g���玩�M����菜��
	for (CNavConnectData& connect : mConnectData)
	{
		connect.node->RemoveConnect(this);
	}
	// ���g�̐ڑ����X�g���N���A
	mConnectData.clear();
}

// �m�[�h�̐F�ݒ�i�f�o�b�O�p�j
void CNavNode::SetColor(const CColor& color)
{
	mColor.Set(color.R(), color.G(), color.B());
}

// �m�[�h��`��i�f�o�b�O�p�j
void CNavNode::Render()
{
	// �L���Ȃ�`��
	if (mIsEnable)
	{
		// �ڑ���̃m�[�h�܂ł̃��C����`��
		for (CNavConnectData& connect : mConnectData)
		{
			Primitive::DrawLine
			(
				GetOffsetPos(),
				connect.node->GetOffsetPos(),
				CColor(0.11f, 0.1f, 0.1f, 1.0f),
				2.0f
			);
		}
		// �m�[�h�̍��W�ɋ���`��
		CMatrix m;
		m.Translate(GetOffsetPos());
		Primitive::DrawWireSphere(m, 1.0f, mColor);
	}
}
