#pragma once

class CDeliveryField;
class CSky;

/*
�z�B�̃t�B�[���h�Ǘ��N���X
*/
class CDeliveryFieldManager
{
public:
	static CDeliveryFieldManager* Instance();
	// �R���X�g���N�^
	CDeliveryFieldManager();
	// �f�X�g���N�^
	~CDeliveryFieldManager();

	// �X�V
	void Update();

	/// <summary>
	/// �w��̃t�B�[���h���擾����
	/// </summary>
	/// <param name="num">�w�肷��ԍ�</param>
	/// <param name="end">�Ō�̗v�f�Ȃ�true</param>
	/// <returns></returns>
	CDeliveryField* GetField(int num, bool& end);

private:
	// �t�B�[���h�𐶐�
	void CreateField();

	// �t�B�[���h�Ǘ��N���X�̃C���X�^���X
	static CDeliveryFieldManager* spInstance;

	// �t�B�[���h
	std::vector<CDeliveryField*> mFields;

	// ��ԉ��̃t�B�[���h��Z���W
	float mFieldPosZ;
};