#pragma once

class CTextUI2D;

// ��邱�Ƃ�\������UI
class CToDoUI
{
public:
	// �R���X�g���N�^
	CToDoUI();
	// �f�X�g���N�^
	~CToDoUI();

	// �X�V
	void Update();

	// ��邱�Ƃ�\�����镶��������X�g�ɒǉ�
	void AddText(std::string text);
	// ��邱�Ƃ�\�����镶��������X�g����ݒ肷��
	void SetText(int num);
private:
	// ��邱�Ƃ�\�����镶����̃��X�g
	std::vector<std::string> mTexts;
	// ���\�����镶����
	int mNowTextNum;
	// �e�L�X�gUI
	CTextUI2D* mpToDoUI;
};