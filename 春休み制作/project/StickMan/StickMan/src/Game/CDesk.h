#pragma once
#include "CFieldBase.h"

class CConnectWall;

// ��
class CDesk : public CFieldBase
{
public:
	// �R���X�g���N�^
	CDesk();
	// �f�X�g���N�^
	~CDesk();

	// �q���邩��ݒ�
	void SetIsConnect(bool isTop, bool isBottom, bool isLeft, bool isRight);

	// �q���ǂ𐶐�
	void CreateConnectWall();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects();

	// ����̃t�B�[���h�𐶐�
	CFieldBase* mpMap;

	// �����q����邩�ǂ���
	bool mIsConnectT;
	bool mIsConnectB;
	bool mIsConnectL;
	bool mIsConnectR;
	// �q���ǂ̃��X�g
	std::vector<CConnectWall*> mpConnectWalls;
};