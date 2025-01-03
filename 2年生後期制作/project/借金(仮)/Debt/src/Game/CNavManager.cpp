#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include <assert.h>
#include "CFieldBase.h"
#include "CVehicleManager.h"

CNavManager* CNavManager::spInstance = nullptr;

// �o�H�Ǘ��N���X�̃C���X�^���X���擾
CNavManager* CNavManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CNavManager::CNavManager()
	: CTask(ETaskPriority::eNavigation, 0, ETaskPauseType::eGame)
	, mIsRender(false)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// �f�X�g���N�^
CNavManager::~CNavManager()
{
	spInstance = nullptr;

	// �T���m�[�h��S�č폜
	auto itr = mNodes.begin();
	auto end = mNodes.end();
	while (itr != end)
	{
		CNavNode* del = *itr;
		itr = mNodes.erase(itr);
		delete del;
	}
}

// �o�H�T���p�̃m�[�h��ǉ�
void CNavManager::AddNode(CNavNode* node)
{
	mNodes.push_back(node);
}

// �o�H�T���p�̃m�[�h����菜��
void CNavManager::RemoveNode(CNavNode* node)
{
	mNodes.remove(node);
}

// �w�肵���m�[�h�ɐڑ��ł���m�[�h���������Đݒ�
int CNavManager::FindConnectNavNodes(CNavNode* node, float distance)
{
	// ���݂̐ڑ���̏������ׂăN���A���Ă���
	node->ClearConnects();

	for (CNavNode* findNode : mNodes)
	{
		// �������g�ł���΁A�X���[
		if (findNode == node) continue;

		// �w�肳�ꂽ�����̌��E�l�𒴂���ꍇ�́A�X���[
		float dist = (findNode->GetPos() - node->GetPos()).Length();
		if (dist > distance) continue;

		// �t�B�[���h�Ǝԗ��Ƃ̃��C����ŎՕ����`�F�b�N���s��
		CVector start = node->GetOffsetPos();
		CVector end = findNode->GetOffsetPos();
		CHitInfo hit;

		CFieldBase* fieldBase = CFieldBase::Instance();
		CVehicleManager* vehicleMgr = CVehicleManager::Instance();

		// �t�B�[���h������Ȃ�Փ˔��������
		if (fieldBase != nullptr)
		{
			if (fieldBase->CollisionRay(start, end, &hit))
			{
				// �����Ƀq�b�g�����ꍇ�́A�Օ���������̂Őڑ��ł��Ȃ�
				continue;
			}
		}
		// �ԗ��Ǘ��N���X������Ȃ�Փ˔��������
		if (vehicleMgr != nullptr)
		{
			if (vehicleMgr->CollisionRay(start, end, &hit))
			{
				// �����Ƀq�b�g�����ꍇ�́A�Օ���������̂Őڑ��ł��Ȃ�
				continue;
			}
		}

		// �����̏����𖞂������m�[�h��ڑ����X�g�ɒǉ�
		node->AddConnect(findNode);
	}

	return node->mConnectData.size();
}

// �S�Ẵm�[�h�ƌo�H��`��
void CNavManager::Render()
{
	// N�L�[�Ōo�H�T���m�[�h�̕`�惂�[�h��؂�ւ�
	if (CInput::PushKey('N'))
	{
		mIsRender = !mIsRender;
	}

	// �o�H�T���m�[�h��`�悵�Ȃ��Ȃ�΁A�ȍ~�������Ȃ�
	if (!mIsRender) return;

	// ���X�g���̃m�[�h��S�ĕ`��
	for (CNavNode* node : mNodes)
	{
		node->Render();
	}
}