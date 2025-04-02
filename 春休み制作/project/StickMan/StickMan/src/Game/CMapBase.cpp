#include "CMapBase.h"
#include "CBlockWall.h"

#define SCALE 10.0f
// �ǂ��ǂ̃I�t�Z�b�g���W
#define OFFSET_POS_SHORT 50.0f	// ������
#define OFFSET_POS_LONG 95.0f	// �Z����

// �㉺�̍ǂ��ǂ̃I�t�Z�b�g��]
#define OFFSET_ROT_UpDown CVector(0.0f,90.0f,0.0f)

// �R���X�g���N�^
CMapBase::CMapBase(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CFieldBase()
	, mIsConnectUp(isUp)
	, mIsConnectDown(isDown)
	, mIsConnectL(isLeft)
	, mIsConnectR(isRight)
{
	Scale(Scale() * SCALE);
}

// �f�X�g���N�^
CMapBase::~CMapBase()
{
}

// �ǂ��ǂ𐶐�
void CMapBase::CreateBlockWall()
{
	// ��Ɍq����Ȃ��ꍇ
	if (!mIsConnectUp)
	{
		CBlockWall* wallL = new CBlockWall();
		CBlockWall* wallR = new CBlockWall();
		wallL->Position(Position() + CVector(-OFFSET_POS_SHORT, 0.0f, -OFFSET_POS_LONG));
		wallR->Position(Position() + CVector( OFFSET_POS_SHORT, 0.0f, -OFFSET_POS_LONG));
		wallL->Scale(Scale());
		wallR->Scale(Scale());
		wallL->Rotation(OFFSET_ROT_UpDown);
		wallR->Rotation(OFFSET_ROT_UpDown);
	}
	// ���Ɍq����Ȃ��ꍇ
	if (!mIsConnectDown)
	{
		CBlockWall* wallL = new CBlockWall();
		CBlockWall* wallR = new CBlockWall();
		wallL->Position(Position() + CVector(-OFFSET_POS_SHORT, 0.0f, OFFSET_POS_LONG));
		wallR->Position(Position() + CVector( OFFSET_POS_SHORT, 0.0f, OFFSET_POS_LONG));
		wallL->Scale(Scale());
		wallR->Scale(Scale());
		wallL->Rotation(OFFSET_ROT_UpDown);
		wallR->Rotation(OFFSET_ROT_UpDown);
	}
	// ���Ɍq����Ȃ��ꍇ
	if (!mIsConnectL)
	{
		CBlockWall* wallU = new CBlockWall();
		CBlockWall* wallD = new CBlockWall();
		wallU->Position(Position() + CVector(-OFFSET_POS_LONG, 0.0f, -OFFSET_POS_SHORT));
		wallD->Position(Position() + CVector(-OFFSET_POS_LONG, 0.0f, OFFSET_POS_SHORT));
		wallU->Scale(Scale());
		wallD->Scale(Scale());
	}
	// �E�Ɍq����Ȃ��ꍇ
	if (!mIsConnectR)
	{
		CBlockWall* wallU = new CBlockWall();
		CBlockWall* wallD = new CBlockWall();
		wallU->Position(Position() + CVector(OFFSET_POS_LONG, 0.0f, -OFFSET_POS_SHORT));
		wallD->Position(Position() + CVector(OFFSET_POS_LONG, 0.0f,  OFFSET_POS_SHORT));
		wallU->Scale(Scale());
		wallD->Scale(Scale());
	}
}
