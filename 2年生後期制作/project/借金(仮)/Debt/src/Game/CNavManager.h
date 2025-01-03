#pragma once
#include <list>
#include <vector>
#include "CTask.h"

class CNavNode;

// �o�H�T�������̊Ǘ��N���X
class CNavManager :public CTask
{
public:	
	// �o�H�Ǘ��N���X�̃C���X�^���X���擾
	static CNavManager* Instance();

	// �R���X�g���N�^
	CNavManager();
	// �f�X�g���N�^
	~CNavManager();

	// �o�H�T���p�̃m�[�h��ǉ�
	void AddNode(CNavNode* node);
	// �o�H�T���p�̃m�[�h����菜��
	void RemoveNode(CNavNode* node);

	/// <summary>
	/// �w�肵���m�[�h�ɐڑ��ł���m�[�h���������Đݒ�
	/// �i�ԂɎՕ���������m�[�h�ɂ͐ڑ��ł��Ȃ��j
	/// </summary>
	/// <param name="node">�ڑ��ł���m�[�h����������m�[�h</param>
	/// <param name="distance">�ڑ��ł��鋗���̌��E�l</param>
	/// <returns>�ڑ������m�[�h�̐�</returns>
	int FindConnectNavNodes(CNavNode* node, float distance);

	// �S�Ẵm�[�h�ƌo�H��`��
	void Render() override;
private:
	// �o�H�Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CNavManager* spInstance;

	// �o�H�T���p�m�[�h�̃��X�g
	std::list<CNavNode*> mNodes;

	// �o�H�T���̃f�o�b�O�\�����s�����ǂ���
	bool mIsRender;
};