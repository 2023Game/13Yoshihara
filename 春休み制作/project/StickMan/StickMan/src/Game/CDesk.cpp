#include "CDesk.h"
#include "CMap_1.h"
#include "CConnectWall.h"

#define SCALE 1.5f
#define ROT	CVector(0.0f,90.0f,0.0f)

// �q���ǂ̃I�t�Z�b�g���W
#define OFFSET_POS_TB CVector(0.0f,0.0f,95.0f)		// �㉺
#define OFFSET_POS_LR CVector(65.0f,0.0f,0.0f)		// ���E

// �q���ǂ̃I�t�Z�b�g��]
#define OFFSET_ROT_BOTTOM	CVector(0.0f,180.0f,0.0f)	// ��
#define OFFSET_ROT_LEFT		CVector(0.0f,-90.0f,0.0f)	// ��
#define OFFSET_ROT_RIGHT	CVector(0.0f,90.0f,0.0f)	// �E

// �R���X�g���N�^
CDesk::CDesk()
	: mIsConnectT(false)
	, mIsConnectB(false)
	, mIsConnectL(false)
	, mIsConnectR(false)
{
	mpModel = CResourceManager::Get<CModel>("Desk");

	Scale(Scale() * SCALE);
	Rotation(ROT);

	// �R���C�_�[�𐶐�
	CreateCol("DeskCol", "", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CDesk::~CDesk()
{
}

// ��Ɍq���邩��ݒ�
void CDesk::SetIsConnectTop(bool isTop)
{
	mIsConnectT = isTop;
}

// ���Ɍq���邩��ݒ�
void CDesk::SetIsConnectBottom(bool isBottom)
{
	mIsConnectB = isBottom;
}

// ���Ɍq���邩��ݒ�
void CDesk::SetIsConnectLeft(bool isLeft)
{
	mIsConnectL = isLeft;
}

// �E�Ɍq���邩��ݒ�
void CDesk::SetIsConnectRight(bool isRight)
{
	mIsConnectR = isRight;
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CDesk::CreateFieldObjects()
{
	//mpMap = new CMap_1();
	//// �e�ɐݒ�
	//mpMap->SetParent(this);
}

// �q���ǂ𐶐�
void CDesk::CreateConnectWall()
{
	// ��Ɍq����Ȃ��ꍇ
	if (mIsConnectT)
	{
		CConnectWall* wallT = new CConnectWall();
		// �e�ɐݒ�
		wallT->SetParent(this);
		wallT->Position(Position() - OFFSET_POS_TB);
	}
	// ���Ɍq����Ȃ��ꍇ
	if (mIsConnectB)
	{
		CConnectWall* wallB = new CConnectWall();
		// �e�ɐݒ�
		wallB->SetParent(this);
		wallB->Position(Position() + OFFSET_POS_TB);
		wallB->SetDefaultRot(OFFSET_ROT_BOTTOM);
	}
	// ���Ɍq����Ȃ��ꍇ
	if (mIsConnectL)
	{
		CConnectWall* wallL = new CConnectWall();
		// �e�ɐݒ�
		wallL->SetParent(this);
		wallL->Position(Position() - OFFSET_POS_LR);
		wallL->SetDefaultRot(OFFSET_ROT_LEFT);
	}
	// �E�Ɍq����Ȃ��ꍇ
	if (mIsConnectR)
	{
		CConnectWall* wallR = new CConnectWall();
		// �e�ɐݒ�
		wallR->SetParent(this);
		wallR->Position(Position() + OFFSET_POS_LR);
		wallR->SetDefaultRot(OFFSET_ROT_RIGHT);
	}
}