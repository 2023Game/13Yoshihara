#include "CFieldManager.h"
#include "CDesk.h"
#include "CClassRoom.h"
#include "Maths.h"
#include <algorithm>
#include <random>

// �}�b�v���m�̊Ԋu
#define MAP_DIST_X 185.0f
#define MAP_DIST_Z 245.0f

// ���̍�����������������
#define DESK_HEIGHT 110.0f

// ���̍Œᐔ
#define MIN_DESK_NUM 5

// �t�B�[���h�Ǘ��N���X�̃C���X�^���X
CFieldManager* CFieldManager::spInstance = nullptr;

// �t�B�[���h�Ǘ��N���X�̃C���X�^���X���擾
CFieldManager* CFieldManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CFieldManager::CFieldManager()
{
	spInstance = this;

	// �}�b�v�𐶐�
	CreateField();
}

// �f�X�g���N�^
CFieldManager::~CFieldManager()
{
	SAFE_DELETE(spInstance);
}

// �n�ʂ̃R���C�_�\�擾
CCollider* CFieldManager::GetGroundCol()
{
	return nullptr;
}

// �ǂ̃R���C�_�\�擾
CCollider* CFieldManager::GetWallCol()
{
	return nullptr;
}

// �I�u�W�F�N�g�̃R���C�_�\�擾
CCollider* CFieldManager::GetObjCol()
{
	return nullptr;
}

// �}�b�v�𐶐�
void CFieldManager::CreateField()
{
	// ����Ɋg���\�ȁA���𐶐�����s��̃��X�g
	std::vector<CVector2> expandColRow;

	// 1�ԍŏ��̍s�̒������1�ɐݒ�
	int row = 0;
	int col = (int)(DESK_COL / 2);
	mMap[row][col] = 1;

	// ��������s��ɒǉ�
	expandColRow.push_back(CVector2(col, row));

	// �Œᐔ�ȏ�ɂȂ�܂ŌJ��Ԃ�
	while (expandColRow.size() < MIN_DESK_NUM)
	{
		// ��������ʒu�̒�����g������Ƃ��̊�������_������
		int index = Math::Rand(0, expandColRow.size() - 1);
		CVector2 colRow = expandColRow[index];
		col = colRow.X();	// ����擾
		row = colRow.Y();	// �s���擾

		// �����V�[�h�l�p
		std::random_device rnd;
		// �[�������񐶐���
		std::mt19937 mt(rnd());

		// 4�����i��A���A���A�E�j
		std::vector<CVector2> directions = { {0,-1},{0,1},{-1,0},{1,0} };
		// �v�f�������_���ɕ��ёւ�
		std::shuffle(directions.begin(), directions.end(), mt);

		// �g��������������
		bool isSuccess = false;

		for (const CVector2 dir : directions)
		{
			// �g����̍s��
			int nextCol = col - dir.X();
			int nextRow = row - dir.Y();

			// ���X�g�͈͓����A�������邱�ƂɂȂ��Ă��Ȃ�
			if (InList(nextRow, nextCol) && mMap[nextRow][nextCol] == 0)
			{
				// ���̔z�u�̍s���ݒ�
				mMap[nextRow][nextCol] = 1;
				// ��������s��ɒǉ�
				expandColRow.push_back(CVector2(nextRow, nextCol));
				// �g������
				isSuccess = true;
				break;
			}
		}

		// �g�������s���Ă�����
		if (!isSuccess)
		{
			// ��₩�����
			expandColRow.erase(expandColRow.begin() + index);
		}
	}

	// ���̃X�P�[��
	float deskScale = 1.0f;
	// �}�b�v���m���q����邩�ǂ���
	bool isTop = true;
	bool isBottom = true;
	bool isLeft = true;
	bool isRight = true;
	// ���𐶐�
	for (int i = 0; i < DESK_ROW; i++)
	{
		for (int j = 0; j < DESK_COL; j++)
		{
			// �������Ȃ��Ȃ玟��
			if (mMap[i][j] == 0) continue;
			// ���𐶐�
			mpDeskList.push_back(new CDesk());
			// ���W��ݒ�
			mpDeskList.back()->Position(
				(j - DESK_COL / 2) * MAP_DIST_X,
				0.0f,
				(i - DESK_ROW / 2) * MAP_DIST_Z);
			// ���̃X�P�[�����擾
			deskScale = mpDeskList.back()->Scale().Y();
		}
	}

	//// �����m�̂Ȃ����ݒ肵�ĕǂ𐶐�
	//for (int i = 0; i < DESK_ROW; i++)
	//{
	//	// ��ԏ�̍s�Ȃ��ւ͌q����Ȃ�
	//	if (i == 0)
	//	{
	//		isTop = false;
	//		isBottom = true;
	//	}
	//	// ��ԉ��̍s�Ȃ牺�ւ͌q����Ȃ�
	//	else if (i == DESK_ROW - 1)
	//	{
	//		isTop = true;
	//		isBottom = false;
	//	}
	//	// ����ȊO�͌q����
	//	else
	//	{
	//		isTop = true;
	//		isBottom = true;
	//	}
	//	for (int j = 0; j < DESK_COL; j++)
	//	{
	//		// ��ԍ��̗�Ȃ獶�ւ͌q����Ȃ�
	//		if (j == 0)
	//		{
	//			isLeft = false;
	//			isRight = true;
	//		}
	//		// ��ԉE�̗�Ȃ�E�ւ͌q����Ȃ�
	//		else if (j == DESK_COL - 1)
	//		{
	//			isLeft = true;
	//			isRight = false;
	//		}
	//		// ����ȊO�͌q����
	//		else
	//		{
	//			isLeft = true;
	//			isRight = true;
	//		}
	//		// ��������Ă��Ȃ��ꏊ�Ȃ玟��
	//		if (mMap[i][j] == 0) continue;
	//		// �Ȃ����ݒ�
	//		mpDeskList[i][j]->SetIsConnect(isTop, isBottom, isLeft, isRight);
	//		// �q���ǂ𐶐�
	//		mpDeskList[i][j]->CreateConnectWall();
	//	}
	//}
	// �����𐶐�
	mpClassRoom = new CClassRoom();
	// �����̈ʒu�𒲐�
	mpClassRoom->Position(0.0f, -110.0f * deskScale, 0.0f);
}

// ���X�g�͈͓̔���
bool CFieldManager::InList(int row, int col)
{
	if (row >= 0 &&
		row < DESK_ROW &&
		col >= 0 &&
		col < DESK_COL)
	{
		return true;
	}

	return false;
}
