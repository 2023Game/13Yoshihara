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

	// ��Ɍq���邩��ݒ�
	void SetIsConnectTop(bool isTop);
	// ���Ɍq���邩��ݒ�
	void SetIsConnectBottom(bool isBottom);
	// ���Ɍq���邩��ݒ�
	void SetIsConnectLeft(bool isLeft);
	// �E�Ɍq���邩��ݒ�
	void SetIsConnectRight(bool isRight);

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