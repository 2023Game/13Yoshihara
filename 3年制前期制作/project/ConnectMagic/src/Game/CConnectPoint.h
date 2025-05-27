#pragma once
#include "CBillBoardImage.h"

class CConnectObject;

// �ڑ�����
class CConnectPoint : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CConnectPoint(CConnectObject* connectObj);
	// �f�X�g���N�^
	~CConnectPoint();

	// �X�V
	void Update() override;

	// �ڑ��������Ă���I�u�W�F�N�g���擾����
	CConnectObject* GetConnectObj();

	// �y�A�̐ڑ�����ݒ�
	void SetPair(CConnectPoint* pair);

	// �y�A�̐ڑ������擾
	CConnectPoint* GetPair() const;

private:
	// �ڑ��������Ă���I�u�W�F�N�g
	CConnectObject* mpConnectObj;

	// �y�A�̐ڑ���
	CConnectPoint* mpPair;
};