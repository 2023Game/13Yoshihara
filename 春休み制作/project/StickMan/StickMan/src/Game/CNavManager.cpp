#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include <assert.h>
#include "CFieldBase.h"
#include "CTrashVehicleManager.h"
#include "Primitive.h"

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
		// �����ł���΁A�X���[
		if (!findNode->IsEnable()) continue;

		// �ړI�n��p�m�[�h�͋������l�����Ȃ�
		if (!node->mIsDestNode)
		{
			// �w�肳�ꂽ�����̌��E�l�𒴂���ꍇ�́A�X���[
			float dist = (findNode->GetPos() - node->GetPos()).Length();
			if (dist > distance) continue;
		}
		// �t�B�[���h�Ǝԗ��Ƃ̃��C����ŎՕ����`�F�b�N���s��
		CVector start = node->GetOffsetPos();
		CVector end = findNode->GetOffsetPos();
		CHitInfo hit;
		bool isHit = false;

		CFieldBase* fieldBase = CFieldBase::Instance();
		CTrashVehicleManager* vehicleMgr = CTrashVehicleManager::Instance();

		// �t�B�[���h������Ȃ�Փ˔��������
		if (fieldBase != nullptr)
		{
			if (fieldBase->CollisionRay(start, end, &hit))
			{
				isHit = true;
			}
		}
		// �ԗ��Ǘ��N���X������Ȃ�Փ˔��������
		if (vehicleMgr != nullptr)
		{
			if (vehicleMgr->NavCollisionRay(start, end, &hit, isHit))
			{
				isHit = true;
			}
		}

		// �����Ƀq�b�g�����ꍇ�́A�Օ���������̂Őڑ��ł��Ȃ�
		if (isHit)
		{
			continue;
		}

		// �����̏����𖞂������m�[�h��ڑ����X�g�ɒǉ�
		node->AddConnect(findNode);
	}

	return node->mConnectData.size();
}

// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
void CNavManager::ResetCalcData()
{
	// �S�Ẵm�[�h�̌v�Z�p�̃f�[�^�����Z�b�g
	for (CNavNode* node : mNodes)
	{
		node->ResetCalcData();
	}
}

// �w�肵���m�[�h���玟�̃m�[�h�ւ̈ړ��R�X�g���v�Z
void CNavManager::CalcNextMoveCost(CNavNode* node, CNavNode* goal)
{
	// �w�肳�ꂽ�m�[�h�܂��͖ړI�n�m�[�h���󂾂����ꍇ�́A�ړ��R�X�g�v�Z�s��
	if (node == nullptr || goal == nullptr) return;

	// �ڑ����Ă���S�Ẵm�[�h�ւ̈ړ��R�X�g�����߂�
	for (CNavConnectData& connect : node->mConnectData)
	{
		if (!connect.node->IsEnable()) continue;
		// �ڑ����Ă���m�[�h���ړI�n��p�m�[�h�̏ꍇ�́A
		// ����̌o�H�T���̖ړI�n�m�[�h�ȊO�͌o�R���Ȃ����߁A�X���[
		if (connect.node->mIsDestNode && connect.node != goal) continue;

		// �ړ��R�X�g�������܂ł̈ړ��R�X�g�{�ڑ���̃m�[�h�܂ł̈ړ��R�X�g
		float cost = node->mCalcMoveCost + connect.cost;
		// �ڑ���̃m�[�h�܂ł̈ړ��R�X�g���܂��v�Z����Ă��Ȃ����A
		// ���߂��ړ��R�X�g�̕������݂̐ڑ���̃m�[�h�܂ł̈ړ��R�X�g��菬�����ꍇ
		if (connect.node->mCalcMoveCost < 0 || cost < connect.node->mCalcMoveCost)
		{
			// �ڑ���̃m�[�h�̈ړ��R�X�g��1�O�̃m�[�h�̃|�C���^���X�V
			connect.node->mCalcMoveCost = cost;
			connect.node->mpCalcFromNode = node;

			// �ڑ���̃m�[�h���ړI�n�̃m�[�h�łȂ���΁A
			// �ڑ���̃m�[�h����ڑ�����Ă���m�[�h�ւ̈ړ��R�X�g���v�Z����
			if (connect.node != goal)
			{
				CalcNextMoveCost(connect.node, goal);
			}
		}
	}
}


// �w�肵���J�n�m�[�h����ړI�n�m�[�h�܂ł̍ŒZ�o�H�����߂�
bool CNavManager::Navigate(CNavNode* start, CNavNode* goal, std::vector<CNavNode*>& route)
{
	// �J�n�m�[�h�܂��͖ړI�n�m�[�h���󂾂����ꍇ�́A�o�H�T���s��
	if (start == nullptr || goal == nullptr) return false;
	// �J�n�m�[�h�܂��͖ړI�n�m�[�h�������������ꍇ�́A�o�H�T���s��
	if (!start->IsEnable() || !goal->IsEnable()) return false;

	// �S�Ẵm�[�h�̍ŒZ�o�H�v�Z�p�̃f�[�^���N���A
	ResetCalcData();

	// �J�n�m�[�h�ւ̈ړ��R�X�g��0�ɐݒ�
	start->mCalcMoveCost = 0.0f;
	// �J�n�m�[�h���珇�ԂɁA
	// �e�m�[�h���ڑ����Ă���m�[�h�܂ł̈ړ��R�X�g���v�Z
	CalcNextMoveCost(start, goal);

	// �ŒZ�o�H�̃��X�g���N���A���āA�ŏ��ɖړI�n�m�[�h��o�^
	route.clear();
	route.push_back(goal);

	// �ړI�n�m�[�h���珇�ԂɁA
	// �ŒZ�o�H�ł�1�O�̃m�[�h�����݂�����胋�[�v���āA
	// �ŒZ�o�H�̃��X�g�Ƀm�[�h��o�^���Ă���
	CNavNode* node = goal;
	while (node->mpCalcFromNode != nullptr)
	{
		route.insert(route.begin(), node->mpCalcFromNode);
		node = node->mpCalcFromNode;
	}

	// ���߂��ŒZ�o�H���f�o�b�O�\���p�̍ŒZ�o�H���X�g�ɃR�s�[
	mLastCalcRoute = route;

	// ���߂��ŒZ�o�H��1�ԍŏ��̃m�[�h���J�n�m�[�h�ł���΁A
	// �J�n�m�[�h����ړI�n�m�[�h�܂Ōo�H���Ȃ����Ă���
	return route[0] == start;
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

	// �Ō�ɋ��߂��ŒZ�o�H�Ƀ��C��������
	glDisable(GL_DEPTH_TEST);
	int size = mLastCalcRoute.size();
	for (int i = 0; i < size - 1; i++)
	{
		CVector start = mLastCalcRoute[i]->GetOffsetPos();
		CVector end = mLastCalcRoute[i + 1]->GetOffsetPos();
		Primitive::DrawLine(start, end, CColor::cyan, 4.0f);
	}
	glEnable(GL_DEPTH_TEST);
}