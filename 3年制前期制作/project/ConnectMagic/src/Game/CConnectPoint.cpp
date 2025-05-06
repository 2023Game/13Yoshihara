#include "CConnectPoint.h"
#include "CConnectObject.h"

// �R���X�g���N�^
CConnectPoint::CConnectPoint(CConnectObject* connectObj)
	: CBillBoardImage("Effect/ConnectPoint.png", ETag::eConnectPoint, ETaskPauseType::eGame)
	, mpConnectObj(connectObj)
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

// �ڑ��������Ă���I�u�W�F�N�g���擾����
CConnectObject* CConnectPoint::GetConnectObj()
{
	return mpConnectObj;
}
