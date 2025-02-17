#pragma once
#include "CObjectBase.h"
#include "CText.h"

class CFont;

// 2D��Ԃɔz�u����e�L�X�g�N���X
class CTextUI2D : public CObjectBase
{
public:
	// �R���X�g���N�^
	CTextUI2D(ETaskPauseType pauseType = ETaskPauseType::eGame,
		bool addTaskList = true,
		char * fontPath = "res\\Font\\toroman.ttf");
	// �f�X�g���N�^
	~CTextUI2D();

	// �X�V
	void Update();
	// �`��
	void Render();

	/// <summary>
	/// �`�悷�镶�����ϊ����Đݒ肷��
	/// </summary>
	/// <param name="format">����������</param>
	/// <param name="��2�����ȍ~">������ɕϊ��������</param>
	void ChangeToStr(const char* format, ...);

	// �`�悷�镶�����ݒ肷��
	void SetStr(std::string str);

	// �����̃T�C�Y��ݒ肷��
	void SetFontSize(int fontSize);
	// �����̐F��ݒ肷��
	void SetFontColor(CColor color);
	// �A�E�g���C���̐F��ݒ肷��
	void SetOutLineColor(CColor color);

	// �����̑����̊��ݒ�
	void SetFontAligment(FTGL::TextAlignment aligment);

	// �e��ݒ�
	void SetShadow(bool enable, CColor color = CColor(0.0f, 0.0f, 0.0f));
	// �A�E�g���C����ݒ�
	void SetOutline(bool enable, CColor color = CColor(0.0f, 0.0f, 0.0f));

	// �_�ł��邩��ݒ�
	void SetBlink(bool enable);

	// �_�ł��鏈��
	void Blink();

private:
	CFont* mpFont;	// �t�H���g
	CText* mpText;	// ������`��p

	std::string mStr;	// �\�����镶����

	// �_�ł��邩
	bool mIsBlink;

	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p
};