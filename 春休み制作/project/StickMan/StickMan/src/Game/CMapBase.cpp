#include "CMapBase.h"
#include "CConnectWall.h"

// �q���ǂ̃I�t�Z�b�g���W
#define OFFSET_POS_TB CVector(0.0f,0.0f,95.0f)		// �㉺
#define OFFSET_POS_LR CVector(65.0f,0.0f,0.0f)		// ���E

// �q���ǂ̃I�t�Z�b�g��]
#define OFFSET_ROT_BOTTOM	CVector(0.0f,180.0f,0.0f)	// ��
#define OFFSET_ROT_LEFT		CVector(0.0f,-90.0f,0.0f)	// ��
#define OFFSET_ROT_RIGHT	CVector(0.0f,90.0f,0.0f)	// �E

// �R���X�g���N�^
CMapBase::CMapBase(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CFieldBase()
	, mIsConnectT(isUp)
	, mIsConnectB(isDown)
	, mIsConnectL(isLeft)
	, mIsConnectR(isRight)
{
}

// �f�X�g���N�^
CMapBase::~CMapBase()
{
}

// �q���ǂ𐶐�
void CMapBase::CreateConnectWall()
{
	// ��Ɍq����Ȃ��ꍇ
	if (mIsConnectT)
	{
		CConnectWall* wallT = new CConnectWall();
		wallT->Position(Position() - OFFSET_POS_TB);
	}
	// ���Ɍq����Ȃ��ꍇ
	if (mIsConnectB)
	{
		CConnectWall* wallB = new CConnectWall();
		wallB->Position(Position() + OFFSET_POS_TB);
		wallB->SetDefaultRot(OFFSET_ROT_BOTTOM);
	}
	// ���Ɍq����Ȃ��ꍇ
	if (mIsConnectL)
	{
		CConnectWall* wallL = new CConnectWall();
		wallL->Position(Position() - OFFSET_POS_LR);
		wallL->SetDefaultRot(OFFSET_ROT_LEFT);
	}
	// �E�Ɍq����Ȃ��ꍇ
	if (mIsConnectR)
	{
		CConnectWall* wallR = new CConnectWall();
		wallR->Position(Position() + OFFSET_POS_LR);
		wallR->SetDefaultRot(OFFSET_ROT_RIGHT);
	}
}
