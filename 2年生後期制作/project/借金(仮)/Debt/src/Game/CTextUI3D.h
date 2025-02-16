#pragma once
#include "CTextUI2D.h"
#include "CVector.h"
#include "CTexture.h"
#include "CText.h"
#include "CTriangle.h"
#include "CMaterial.h"

class CFont;

// 3D��Ԃɔz�u����e�L�X�g�N���X
class CTextUI3D : public CTextUI2D
{
public:
	// �R���X�g���N�^
	CTextUI3D(ETaskPauseType pauseType = ETaskPauseType::eGame,
		bool addTaskList = true,
		char* fontPath = "res\\Font\\toroman.ttf");
	// �f�X�g���N�^
	~CTextUI3D();

	// 2D��Ԃł̃I�t�Z�b�g���W���擾
	const CVector2& GetOffsetPos() const;
	// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�
	void SetOffsetPos(const float& x, const float& y);
	// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�iCVector2�Łj
	void SetOffsetPos(const CVector2& pos);

	// �T�C�Y�擾
	const CVector2& GetSize() const;
	// �T�C�Y�ݒ�
	void SetSize(const float& x, const float& y);
	// �T�C�Y�ݒ�iCVector2�Łj
	void SetSize(const CVector2& size);

	// UV�擾
	const CRect& GetUV() const;
	// UV�ݒ�
	void SetUV(const float& left, const float& top, const float& right, const float& bottom);
	// UV�ݒ�iCRect�Łj
	void SetUV(const CRect& uv);

	// 3D��Ԃł̏k�ڂ�ݒ�
	void SetWorldUnitPerPixel(float pixel);

	// �r���{�[�h�̃I���I�t��ݒ�
	void SetBillboard(bool enable);
	// �f�v�X�e�X�g�̃I���I�t��ݒ�
	void SetDepthTest(bool enable);
	// �f�v�X�e�X�g�̃I���I�t��ݒ�
	void SetDepthMask(bool enable);
	// ���C�e�B���O�̃I���I�t��ݒ�
	void SetLighting(bool enable);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �`��i�}�e���A���w��Łj
	void Render(CMaterial* mpMaterial);

	// �O�p�`2��
	CTriangle mT[2];
	// �}�e���A��
	CMaterial mFontMaterial;
	// 2D��Ԃł̃I�t�Z�b�g���W
	CVector2 mOffsetPos;
	// �T�C�Y�L���p
	CVector2 mSize;
	// �e�N�X�`����UV
	CRect mUV;
	// 3D��Ԃł̏k��
	float mWolrdUnitPerPixel;

	bool mIsBillboard;
	// �f�v�X�e�X�g���L�����ǂ���
	bool mIsDepthTest;
	// �f�v�X�������݂��L�����ǂ���
	bool mIsDepthMask;
	// ���C�e�B���O���L�����ǂ���
	bool mIsLighting;
};