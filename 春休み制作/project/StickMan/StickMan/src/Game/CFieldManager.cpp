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
#define MIN_DESK_NUM 9

// ���̍��W�v�Z
#define DESK_POS(row,col) (CVector((col - DESK_COL / 2) * MAP_DIST_X, 0.0f,(row - DESK_ROW / 2) * MAP_DIST_Z))

// �t�B�[���h�Ǘ��N���X�̃C���X�^���X
CFieldManager* CFieldManager::spInstance = nullptr;

// �t�B�[���h�Ǘ��N���X�̃C���X�^���X���擾
CFieldManager* CFieldManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CFieldManager::CFieldManager()
	: mSpawnColRow(0.0f,0.0f)
	, mDeskNum(-1)
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

// ���X�|�[���n�_���W���擾
CVector CFieldManager::GetSpawnPos()
{
	return DESK_POS(mSpawnColRow.Y(), mSpawnColRow.X());
}

// �}�b�v�𐶐�
void CFieldManager::CreateField()
{
	// ����Ɋg���\�ȁA���𐶐�����s��̃��X�g
	std::vector<CVector2> expandColRow;
	// �Ō�Ɋg�������s��
	CVector2 lastExpandColRow;

	// 1�ԍŏ��̍s�̒�����𐶐��\��ɐݒ�
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
		// �����������_���ɕ��ёւ�
		std::shuffle(directions.begin(), directions.end(), mt);

		// �g��������������
		bool isSuccess = false;

		for (const CVector2 dir : directions)
		{
			// �g����̍s��
			int nextCol = col + dir.X();
			int nextRow = row + dir.Y();

			// ���X�g�͈͓����A�������邱�ƂɂȂ��Ă��Ȃ�
			if (InList(nextRow, nextCol) && mMap[nextRow][nextCol] == 0)
			{
				// ���̔z�u�̍s���ݒ�
				mMap[nextRow][nextCol] = 1;
				// ��������s��ɒǉ�
				expandColRow.push_back(CVector2(nextCol, nextRow));
				// �g������
				isSuccess = true;
				// �Ō�Ɋg�������s��
				lastExpandColRow = CVector2(nextCol, nextRow);
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

	for (int i = 0; i < DESK_ROW; i++)
	{
		for (int j = 0; j < DESK_COL; j++)
		{
			if (j == DESK_COL - 1)
			{
				printf("%i\n", mMap[i][j]);
			}
			else
			{
				printf("%i", mMap[i][j]);
			}
		}
	}

	// �Ō�Ɋg�������s��̊����X�|�[���n�_�Ƃ���
	mSpawnColRow.X(lastExpandColRow.X());
	mSpawnColRow.Y(lastExpandColRow.Y());

	// ���𐶐�
	CreateDesk(mSpawnColRow.Y(), mSpawnColRow.X());
	// �S�Ă̌q����ǂ𐶐�
	for (auto* desk : mpDesks)
	{
		desk->CreateConnectWall();
	}

	// �����𐶐�
	mpClassRoom = new CClassRoom();
	// �����̈ʒu�𒲐�
	mpClassRoom->Position(0.0f, -110.0f * mpDesks.back()->Scale().Y(), 0.0f);
}

// ���𐶐�
void CFieldManager::CreateDesk(int row, int col)
{
	// ��ƒ��̊��̔ԍ���i�߂�
	mDeskNum++;
	// ��Ɣԍ����Ō�̃C���f�b�N�X�ȉ��Ȃ�
	// ��Ɣԍ��ɗv�f����ݒ肷��
	if (mDeskNum <= mpDesks.size() - 1)
		mDeskNum = mpDesks.size();
	// ���𐶐�
	mpDesks.push_back(new CDesk());
	// ���W��ݒ�
	mpDesks[mDeskNum]->Position(DESK_POS(row, col));

	CVector pos = DESK_POS(row, col);
	printf("Desk%d:%f,%f,%f\n", mDeskNum, pos.X(),pos.Y(),pos.Z());

	// �����ς݂ɕύX
	mMap[row][col] = 2;

	// �����V�[�h�l�p
	std::random_device rnd;
	// �[�������񐶐���
	std::mt19937 mt(rnd());

	// 4�����i��A���A���A�E�j
	std::vector<CVector2> directions = { {0,-1},{0,1},{-1,0},{1,0} };
	// �����������_���ɕ��ёւ�
	std::shuffle(directions.begin(), directions.end(), mt);

	for (const CVector2 dir : directions)
	{
		// �q�����̍s��
		int connectCol = col + dir.X();
		int connectRow = row + dir.Y();

		// ���X�g���������\��Ȃ�q���邱�Ƃ��o����B
		if (InList(connectRow, connectCol) && mMap[connectRow][connectCol] == 1)
		{
			// ��Ȃ�
			if (dir == CVector2(0, -1))
			{
				// ��ɂȂ��邱�Ƃ��ł���
				mpDesks[mDeskNum]->SetIsConnectTop(true);
			}
			// ���Ȃ�
			else if (dir == CVector2(0, 1))
			{
				// ���ɂȂ��邱�Ƃ��ł���
				mpDesks[mDeskNum]->SetIsConnectBottom(true);
			}
			// ���Ȃ�
			else if (dir == CVector2(-1, 0))
			{
				// ���ɂȂ��邱�Ƃ��ł���
				mpDesks[mDeskNum]->SetIsConnectLeft(true);
			}
			// �E�Ȃ�
			else
			{
				// �E�ɂȂ��邱�Ƃ��ł���
				mpDesks[mDeskNum]->SetIsConnectRight(true);
			}

			// �q������̊��𐶐�
			CreateDesk(connectRow, connectCol);
		}
	}

	// ��ƒ��̔ԍ���߂�
	mDeskNum--;
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
