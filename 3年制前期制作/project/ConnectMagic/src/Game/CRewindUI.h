#pragma once

class CImage;
class CTextUI2D;

class CRewindUI : public CTask
{
public:
	// �R���X�g���N�^
	CRewindUI();
	// �f�X�g���N�^
	~CRewindUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// Danger�̕����̗L��������ݒ�
	void SetTextEnable(bool enable);

	// �摜�̐F��ݒ�
	void SetImgColor(CColor col);

private:
	// ��ʑS�̂ɐF��t����摜
	CImage* mpImg;
	// �uDANGER�v�̕���
	CTextUI2D* mpDangerText;
};