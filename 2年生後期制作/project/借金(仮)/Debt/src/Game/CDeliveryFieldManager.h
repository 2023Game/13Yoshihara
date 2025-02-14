#pragma once

class CDeliveryField;
class CDeliveryHome;
class CDeliveryObstruction;
class CDeliveryFieldItem;
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
	// �Ƃ𐶐����邩���肷��
	void CreateHome();
	// ��Q���𐶐����邩���肷��
	void CreateObstruction();
	/// <summary>
	/// ��Q���̍��W�������_���Őݒ�
	/// </summary>
	/// <param name="obstruction">��Q���̃|�C���^</param>
	/// <param name="isPopSuccess�`">
	/// ���̓��ɐ����������������̎Q��</param>
	void ObstructionRandomPos(CDeliveryObstruction* obstruction,
		bool& isPopSuccessL1, bool& isPopSuccessL2,
		bool& isPopSuccessR1, bool& isPopSuccessR2);

	// �A�C�e���𐶐����邩���肷��
	void CreateFieldItem();
	// �A�C�e���̍��W�������_���Őݒ�
	void ItemRandomPos(CDeliveryFieldItem* fieldItem);

	// �t�B�[���h�Ǘ��N���X�̃C���X�^���X
	static CDeliveryFieldManager* spInstance;

	// �t�B�[���h
	std::vector<CDeliveryField*> mFields;
	// ��
	std::vector<CDeliveryHome*> mHomes;
	// ��Q��
	std::vector<CDeliveryObstruction*> mObstructions;
	// �A�C�e��
	std::vector<CDeliveryFieldItem*> mFieldItems;

	// ��ԉ����t�B�[���h
	CDeliveryField* mpFarField;
	
	// �Ō�ɉƂ̐������������Z���W
	float mLastHomePopZ;
	// �Ō�ɏ�Q���̐������������Z���W
	float mLastObstructionPopZ;
	// �Ō�ɃA�C�e���̐������������Z���W
	float mLastFieldItemPopZ;

};