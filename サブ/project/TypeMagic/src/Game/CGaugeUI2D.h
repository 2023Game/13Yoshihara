#pragma once
#include "CObjectBase.h"

class CImage;

// 2D��Ԃɔz�u����C���[�W�N���X
class CGaugeUI2D : public CObjectBase
{
public:
	enum class EGaugeType
	{
		eHpGauge,	// HP�Q�[�W
		eMpGauge,	// MP�Q�[�W
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">������</param>
	/// <param name="gaugePath">�Q�[�W�̉摜�̃p�X</param>
	/// <param name="isRight">�E����Q�[�W�̌������n�܂邩</param>
	/// <param name="gaugeType">�Q�[�W�̃^�C�v</param>
	CGaugeUI2D(CObjectBase* owner, std::string gaugePath, bool isRight, EGaugeType gaugeType);
	// �f�X�g���N�^
	virtual ~CGaugeUI2D();

	// �������ݒ�
	void SetOwner(CObjectBase* owner);

	// �ő�l��ݒ�
	void SetMaxPoint(int point);
	// ���ݒl��ݒ�
	void SetCurrPoint(int point);
	// �|�C���g�c�ʂ̊�����ݒ�i0.0�`1.0�j
	void SetPercent(float per);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
	// �T�C�Y��ݒ�
	void Size(float rate);
	// �T�C�Y���擾
	CVector Size() const;

protected:
	// �|�C���g�c�ʂ𔽉f
	void ApplyPoint();

	CObjectBase* mpOwner;	// HP�Q�[�W�̎�����

	CImage* mpGaugeImg;	// �Q�[�W�̃C���[�W
	CImage* mpWhiteImg;	// ���C���[�W

	CVector2 mGaugeSize;	// �Q�[�W�̃C���[�W�̃T�C�Y
	int mMaxPoint;			// �ő�l
	int mCurrPoint;			// ���ݒl
	float mPercent;			// �|�C���g�c�ʂ̊���

	bool mIsRight;	// �E���猸���Ă���
	EGaugeType mGaugeType;	// �Q�[�W�̃^�C�v
};