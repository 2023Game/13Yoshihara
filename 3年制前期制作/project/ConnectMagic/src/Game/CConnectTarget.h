#pragma once
#include "CObjectBase.h"
#include "CConnectObject.h"

// �ڑ��ł���^�[�Q�b�g����
class CConnectTarget : public CObjectBase
{
public:
	// �R���X�g���N�^
	CConnectTarget(CConnectObject* connectObj);
	// �f�X�g���N�^
	~CConnectTarget();

	// �`��
	void Render() override;

	// �ڑ��������Ă���I�u�W�F�N�g���擾
	CConnectObject* GetConnectObj() const;
	// �ڑ��������Ă���I�u�W�F�N�g��ݒ�
	void SetConnectObj(CConnectObject* obj);

private:
	// �ڑ��������Ă���I�u�W�F�N�g
	CConnectObject* mpConnectObj;
};