#include "CGaugeUI2D.h"
#include "CImage.h"
#include "Maths.h"


// �R���X�g���N�^
CGaugeUI2D::CGaugeUI2D(CObjectBase* owner, std::string gaugePath, bool addTaskList)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, ETaskPauseType::eGame, false, addTaskList)
	, mpOwner(owner)
	, mpGaugeImg(nullptr)
	, mpWhiteImg(nullptr)
	, mMaxPoint(100)
	, mCurrPoint(mMaxPoint)
	, mPercent(1.0f)
{
	// �Q�[�W�̃C���[�W��ǂݍ���
	mpGaugeImg = new CImage
	(
		gaugePath.c_str(),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mGaugeSize = mpGaugeImg->GetSize();

	// ���C���[�W��ǂݍ���
	mpWhiteImg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	CVector2 center = CVector2(-mGaugeSize.X() * 0.5f, 0.0f);
	mpWhiteImg->SetCenter(center);
}

// �f�X�g���N�^
CGaugeUI2D::~CGaugeUI2D()
{
	// �ǂݍ��񂾃C���[�W���폜
	SAFE_DELETE(mpGaugeImg);
	SAFE_DELETE(mpWhiteImg);

	// ������ɍ폜���ꂽ���Ƃ�`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// �������ݒ�
void CGaugeUI2D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �|�C���g�c�ʂ𔽉f
void CGaugeUI2D::ApplyPoint()
{
	// �ő�l���s���l�łȂ����
	if (mMaxPoint > 0)
	{
		// ���ݒl���ő�l�̉��p�[�Z���g�����߂�
		mPercent = Math::Clamp01((float)mCurrPoint / mMaxPoint);
	}
	// �s���l�Ȃ�΁A100%�Œ�
	else
	{
		mPercent = 1.0f;
	}
}

// �ő�l��ݒ�
void CGaugeUI2D::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// ���ݒl��ݒ�
void CGaugeUI2D::SetCurrPoint(int point)
{
	mCurrPoint = point;
	ApplyPoint();
}

// �|�C���g�c�ʂ̊�����ݒ�i0.0�`1.0�j
void CGaugeUI2D::SetPercent(float per)
{
	mPercent = Math::Clamp01(per);
}

// �X�V
void CGaugeUI2D::Update()
{
	mpGaugeImg->Update();
	mpWhiteImg->Update();
}

// �`��
void CGaugeUI2D::Render()
{
	// �s��̕ۑ�
	glPushMatrix();

	CVector2 pos = Position();
	{
		// �Q�[�W�w�i��`��
		CVector2 barPos = pos;
		barPos.X(barPos.X() - mGaugeSize.X() * 0.5f);
		mpWhiteImg->SetPos(barPos);
		mpWhiteImg->SetSize(mGaugeSize);
		mpWhiteImg->SetColor(CColor::black);
		mpWhiteImg->Render();
	}


	// �o�[�̃T�C�Y�A���W�A�F��
	// �|�C���g�c�ʂ̊����ɍ��킹�Ē������āA�o�[��`��
	{
		// �o�[�̃T�C�Y�𒲐�
		CVector2 barSize = mGaugeSize;
		barSize.X(barSize.X() * mPercent);
		mpWhiteImg->SetSize(barSize);
		// �o�[�̍��W�𒲐�
		CVector2 barPos = pos;
		barPos.X(barPos.X() - mGaugeSize.X() * 0.5f);
			//mGaugeSize - barSize;
		mpWhiteImg->SetPos(barPos);
		//CVector2 barPos = pos;
		//barPos.X(barPos.X() - mGaugeSize.X() * 0.5f);
		//mpWhiteImg->SetPos(barPos);
		// �o�[�̐F��ݒ�
		CColor barColor = CColor::green;
		if (mPercent <= 0.2f) barColor = CColor::red;
		else if (mPercent <= 0.5f)barColor = CColor::yellow;
		mpWhiteImg->SetColor(barColor);
		// �o�[��`��
		mpWhiteImg->Render();
	}

	// �Q�[�W�{�̂�`��
	mpGaugeImg->SetSize(mGaugeSize);
	mpGaugeImg->Render();
	mpGaugeImg->SetPos(pos);

	// �s���߂�
	glPopMatrix();
}

// �T�C�Y��ݒ�
void CGaugeUI2D::Size(float rate)
{
	mGaugeSize = mGaugeSize * rate;
	CVector2 center = CVector2(-mGaugeSize.X() * 0.5f, 0.0f);
	mpWhiteImg->SetCenter(center);
}
