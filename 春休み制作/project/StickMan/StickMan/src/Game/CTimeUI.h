#pragma once

class CTextUI2D;

// ���ԕ\��UI
class CTimeUI
{
public:
	// �R���X�g���N�^
	CTimeUI(int maxTime);
	// �f�X�g���N�^
	~CTimeUI();

	// �X�V
	void Update();

	// ���Ԃ𐔂���
	void Countdown();
	// ���Ԃ��o�߂�����
	bool GetElapsedTime();

	// ���Ԃ��擾����
	int GetTime() const;

private:
	// ����
	int mTime;
	// �o�ߎ���
	float mElapsedTime;

	// �e�L�X�gUI
	CTextUI2D* mpTimeUI;
};