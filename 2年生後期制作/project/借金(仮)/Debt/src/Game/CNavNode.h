#pragma once

class CNavManager;
class CNavNode;

// �o�H�T���m�[�h�̐ڑ����
class CNavConnectData
{
public:
	CNavNode* node;	// �q�����Ă���m�[�h
	float cost;		// �q�����Ă���m�[�h�܂ł̈ړ��R�X�g
	CNavConnectData(CNavNode* n, float c)
		: node(n), cost(c) {}
};
// �o�H�T���m�[�h�N���X
class CNavNode
{
	friend CNavManager;

public:
	// �R���X�g���N�^
	CNavNode(const CVector& pos, bool isDestNode = false);
	// �f�X�g���N�^
	~CNavNode();

	// �m�[�h�̍��W���擾
	const CVector& GetPos() const;
	// �Օ����`�F�b�N��\���p�ɁA������ɏグ���m�[�h�̍��W��ς���
	CVector GetOffsetPos() const;
	// �m�[�h�̍��W��ݒ�
	void SetPos(const CVector& pos);

	/// <summary>
	/// �m�[�h���L�����ǂ������擾
	/// </summary>
	/// <returns>true�Ȃ�΁A�L��</returns>
	bool IsEnable() const;
	/// <summary>
	/// �m�[�h���L�����ǂ�����ݒ�
	/// </summary>
	/// <param name="isEnable">�L�����ǂ���</param>
	void SetEnable(bool isEnable);

	// �ڑ�����m�[�h��ǉ�
	void AddConnect(CNavNode* node);
	// �ڑ����Ă���m�[�h����菜��
	void RemoveConnect(CNavNode* node);
	// �ڑ����Ă���S�Ẵm�[�h������
	void ClearConnects();

	// �m�[�h�̐F�ݒ�i�f�o�b�O�p�j
	void SetColor(const CColor& color);
	// �m�[�h��`��i�f�o�b�O�p�j
	void Render();

private:
	// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
	void ResetCalcData();
	// �o�H�T�����Ɍo�R�ł��Ȃ��ړI�n��p�̃m�[�h���ǂ���
	bool mIsDestNode;
	CVector mPosition;	// �m�[�h�̍��W
	// �ڑ����Ă���m�[�h�̏�񃊃X�g
	std::list<CNavConnectData> mConnectData;

	// �ŒZ�o�H�v�Z���ɋL�����Ă������
	float mCalcMoveCost;		// �J�n�m�[�h���炱�̃m�[�h�܂łɂ��������ړ��R�X�g
	CNavNode* mpCalcFromNode;	// �ŒZ�o�H�ł̑O�̃m�[�h�̃|�C���^

	bool mIsEnable;	// �L�����ǂ���

	// �f�o�b�O�֘A
	CColor mColor;		// �m�[�h�̐F
};