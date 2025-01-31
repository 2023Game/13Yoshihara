#pragma once
#include "CObjectBase.h"

class CText;
class CFont;

// 2D��Ԃɔz�u����e�L�X�g�N���X
class CTextUI2D : public CObjectBase
{
public:
	// �R���X�g���N�^
	CTextUI2D();
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

private:
	CFont* mpFont;	// �t�H���g
	CText* mpText;	// ������`��p

	std::string mStr;	// �\�����镶����
};