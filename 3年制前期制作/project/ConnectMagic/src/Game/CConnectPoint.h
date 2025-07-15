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
	CConnectObject* GetConnectObj() const;
	// �ڑ��������Ă���I�u�W�F�N�g��ݒ�
	void SetConnectObj(CConnectObject* obj);

private:
	// �ڑ��������Ă���I�u�W�F�N�g
	CConnectObject* mpConnectObj;
};