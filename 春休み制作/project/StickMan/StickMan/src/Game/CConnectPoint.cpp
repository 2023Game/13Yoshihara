#include "CConnectPoint.h"

// �R���X�g���N�^
CConnectPoint::CConnectPoint()
	: CBillBoardImage("Effect/ConnectPoint.png", ETag::eConnectPoint, ETaskPauseType::eGame)
{
}

// �f�X�g���N�^
CConnectPoint::~CConnectPoint()
{
}

// �X�V
void CConnectPoint::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();
}