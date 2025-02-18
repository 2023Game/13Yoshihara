#pragma once
#include "CHomeUIBase.h"

class CTextUI2D;
class CImage;

// ��邱�Ƃ�\������UI
class CToDoUI : CHomeUIBase
{
public:
	// �R���X�g���N�^
	CToDoUI();
	// �f�X�g���N�^
	~CToDoUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// ��邱�Ƃ�\�����镶��������X�g�ɒǉ�
	void AddText(std::string text);
	// ��邱�Ƃ�\�����镶��������X�g����ݒ肷��
	void SetText(int num);

private:
	// ��邱�Ƃ�\�����镶����̃��X�g
	std::vector<std::string> mTexts;
	// ���\�����镶����
	int mNowTextNum;
};