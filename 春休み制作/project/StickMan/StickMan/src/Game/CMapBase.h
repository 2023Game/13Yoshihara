#pragma once
#include "CFieldBase.h"

class CBlockWall;

class CMapBase : public CFieldBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="isUp">�㑤�ɂȂ���邩</param>
	/// <param name="isDown">�����ɂȂ���邩</param>
	/// <param name="isLeft">�����ɂȂ���邩</param>
	/// <param name="isRight">�E���ɂȂ���邩</param>
	CMapBase(bool isUp, bool isDown, bool isLeft, bool isRight);
	// �f�X�g���N�^
	~CMapBase();

	// �ǂ��ǂ𐶐�
	void CreateBlockWall();

private:
	// �����q����邩�ǂ���
	bool mIsConnectUp;
	bool mIsConnectDown;
	bool mIsConnectL;
	bool mIsConnectR;

	// �ǂ��ǂ̃��X�g
	std::vector<CBlockWall*> mpBlockWalls;

	// �e�[�u��

};