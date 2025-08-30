#pragma once
#include "SaveData.h"
#include <deque>
#include "CRewindUI.h"

// �ۑ��̊Ǘ��N���X
class CSaveManager : CTask
{
public:
	// �C���X�^���X���擾
	static CSaveManager* Instance();

	// �R���X�g���N�^
	CSaveManager();
	// �f�X�g���N�^
	~CSaveManager();

	// �X�V
	void Update() override;

	// ��Ԃ�ۑ�����
	void Save();

	// ��ԌÂ���Ԃ��폜����
	void DeleteOld();
	// ��ԐV������Ԃ��폜
	void DeleteNew();

	// �S�č폜����
	void AllDelete();

	// �f�[�^�����[�h
	void Load();

	// �ۑ����锠�ɒǉ�
	void AddBox(CBox* box);
	// �ۑ����锠����폜
	void DeleteBox(CBox* box);

	// �ۑ�����ړ��I�u�W�F�N�g�ɒǉ�
	void AddMoveObj(CMoveObj* obj);
	// �ۑ�����ړ��I�u�W�F�N�g����폜
	void DeleteMoveObj(CMoveObj* obj);

	// �ۑ�����X�C�b�`�ňړ����鏰�ɒǉ�
	void AddMoveFloor(CSwitchMoveFloor* floor);
	// �ۑ�����X�C�b�`�ňړ����鏰����폜
	void DeleteMoveFloor(CSwitchMoveFloor* floor);

	// �ۑ�����X�C�b�`�ňړ�����ǂɒǉ�
	void AddMoveWall(CSwitchMoveWall* wall);
	// �ۑ�����X�C�b�`�ňړ�����ǂ���폜
	void DeleteMoveFloor(CSwitchMoveWall* wall);

	// �ۑ��Ǘ��N���X�̏��
	enum class EState
	{
		eSave,	// �Z�[�u
		eLoad,	// ���[�h
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);

private:
	// �C���X�^���X
	static CSaveManager* spInstance;

	EState mState;	// ���

	// �����߂�UI
	CRewindUI* mpRewindUI;

	// �ۑ��f�[�^
	// deque�F�O��ւ̒ǉ��ƍ폜�������Ƃ��Ɏg��
	std::deque<GameData> mData;

	// �ۑ����锠
	std::vector<CBox*> mBox;
	// �ۑ�����ړ��I�u�W�F�N�g
	std::vector<CMoveObj*> mMoveObj;
	// �ۑ�����X�C�b�`�ňړ����鏰
	std::vector<CSwitchMoveFloor*> mMoveFloor;
	// �ۑ�����X�C�b�`�ňړ������
	std::vector<CSwitchMoveWall*> mMoveWall;
};