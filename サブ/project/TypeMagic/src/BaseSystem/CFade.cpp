#include "CFade.h"

// �t�F�[�h�N���X�̃C���X�^���XdeskNum
CFade* CFade::ms_instance = nullptr;

#define BLACKOUT_PATH	"UI/white.png"
#define VORTEX_PATH		"Effect/Portal.png"

// �摜�̉�]���x
#define ROT_SPEED 2000.0f

// �R���X�g���N�^
CFade::CFade(EFadeType type)
	: CTask(ETaskPriority::eFade, 0, ETaskPauseType::eSystem, true, true)
	, mFadeColor(CColor::black)
	, mFadeTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsFadeIn(false)
	, mIsFading(false)
	, mType(type)
{
	std::string path;
	switch (type)
	{
		// �Ó]����t�F�[�h�̏ꍇ
	case EFadeType::eBlackOut:
		path = BLACKOUT_PATH;
		break;

		// �Q��t�F�[�h�̏ꍇ
	case EFadeType::eVortex:
		path = VORTEX_PATH;
		break;
	}
	// �t�F�[�h�摜��ǂݍ���
	mpFadeImage = new CImage
	(
		path.c_str(),
		ETaskPriority::eFade, 0,
		ETaskPauseType::eSystem,
		true, false // �t�F�[�h�摜�̓^�X�N���X�g�ɓo�^���Ȃ�
	);
	// �t�F�[�h�̎�ނ�ύX
	// �T�C�Y��J���[���̕ύX�̂��߂ɏ���
	ChangeFadeType(type);
}

// �t�F�[�h�̎�ނ�ύX����
void CFade::ChangeFadeType(EFadeType type)
{
	// �w��̎�ނ̃t�F�[�h�̏����ݒ菈��������
	switch (type)
	{
		// �Ó]����t�F�[�h�̏ꍇ
	case EFadeType::eBlackOut:	InitBlackOut();	break;
		// �Q��̃t�F�[�h�̏ꍇ
	case EFadeType::eVortex:	InitVortex();	break;
	}

	// �t�F�[�h�̎�ނ�ύX
	mType = type;
}

// �Ó]�t�F�[�h�̏����ݒ�
void CFade::InitBlackOut()
{		
	// �Ó]����t�F�[�h�łȂ��Ȃ�
	if (mType != EFadeType::eBlackOut)
	{
		// �摜��ǂݍ���
		mpFadeImage->Load(BLACKOUT_PATH, true);
	}
	// �t�F�[�h�摜����ʑS�̂̃T�C�Y�ɕύX
	mpFadeImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// �t�F�[�h�J���[��ݒ�
	mpFadeImage->SetColor(mFadeColor);
}

// �Q��t�F�[�h�̏����ݒ�
void CFade::InitVortex()
{		
	// �Q��̃t�F�[�h�łȂ��Ȃ�
	if (mType != EFadeType::eVortex)
	{
		// �摜��ǂݍ���
		mpFadeImage->Load(VORTEX_PATH, true);
	}
	// �t�F�[�h�摜����ʕ���2�{�̃T�C�Y�ɕύX
	float size = WINDOW_WIDTH * 2.0f;
	mpFadeImage->SetSize(size, size);
	// �t�F�[�h�摜�̈ʒu��^�񒆂ɐݒ�
	CVector2 pos = CVector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
	mpFadeImage->SetPos(pos - CVector2(size / 2.0f, size / 2.0f));
}

// �f�X�g���N�^
CFade::~CFade()
{
	// �t�F�[�h�摜���^�X�N���X�g�ɓo�^���Ă��Ȃ��̂ŁA
	// �f�X�g���N�^�Ŏ����Ŕj������
	SAFE_DELETE(mpFadeImage);

	ms_instance = nullptr;
}

// �C���X�^���X���擾
CFade* CFade::Instance(EFadeType type)
{
	if (ms_instance == nullptr)
	{
		ms_instance = new CFade(type);
	}
	return ms_instance;
}

// �t�F�[�h�J���[��ݒ�
void CFade::SetFadeColor(const CColor& color)
{
	CFade* inst = Instance();
	inst->mFadeColor.Set(color.R(), color.G(), color.B());
	inst->mpFadeImage->SetColor(inst->mFadeColor);
}

// �t�F�[�h�C���J�n
void CFade::FadeIn(float time)
{
	Instance()->StartFade(time, true);
}

// �t�F�[�h�A�E�g�J�n
void CFade::FadeOut(float time)
{
	Instance()->StartFade(time, false);
}

// �t�F�[�h�J�n
void CFade::StartFade(float time, bool isFadeIn)
{
	// �e�p�����[�^��ݒ�
	mFadeTime = time;
	mElapsedTime = 0.0f;
	mIsFadeIn = isFadeIn;

	switch(mType)
	{
		// �Ó]�t�F�[�h
	case EFadeType::eBlackOut:
	{
		// �t�F�[�h���Ԃ�0���ǂ����ŏ�����Ԃ̃A���t�@�l��ύX
		float alpha = mFadeTime <= 0.0f ? 1.0f : 0.0f;
		// ������Ԃ̃A���t�@�l��ݒ�
		mFadeColor.A(mIsFadeIn ? 1.0f - alpha : alpha);
		mpFadeImage->SetColor(mFadeColor);
		break;
	}

		// �Q��̃t�F�[�h
	case EFadeType::eVortex:
		// ��]����
		mpFadeImage->SetRot(true);
		// ���x�ݒ�
		mpFadeImage->SetRotSpeed(mIsFadeIn ? ROT_SPEED : -ROT_SPEED);
		break;
	}

	// �t�F�[�h���Ԃ�0��蒷���ꍇ�́A�t�F�[�h�t���O�𗧂Ă�
	mIsFading = mFadeTime > 0.0f;
}

// �t�F�[�h�����ǂ���
bool CFade::IsFading()
{
	return Instance()->mIsFading;
}

// �X�V
void CFade::Update()
{
	// �t�F�[�h���łȂ���΁A�������Ȃ�
	if (!mIsFading) return;

	// �t�F�[�h���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < mFadeTime)
	{
		// �t�F�[�h���̏���
		Fade();
	}
	// �t�F�[�h���Ԃ��o�߂���
	else
	{
		// �t�F�[�h�I���̏���
		FadeEnd();
	}
}

// �`��
void CFade::Render()
{
	if (mFadeColor.A() == 0.0f) return;
	mpFadeImage->Render();
}

// �t�F�[�h���̏���
void CFade::Fade()
{
	// �t�F�[�h���i�񂾊���
	float ratio = mElapsedTime / mFadeTime;
	// �t�F�[�h�C�����ǂ����Ŋ���������
	ratio = (mIsFadeIn ? 1.0f - ratio : ratio);

	switch (mType)
	{
		// �Ó]�t�F�[�h
	case EFadeType::eBlackOut:
		// �t�F�[�h�C���[�W�̃A���t�@�l��ύX
		mFadeColor.A(ratio);
		mpFadeImage->SetColor(mFadeColor);
		break;

		// �Q��̃t�F�[�h
	case EFadeType::eVortex:
		// �t�F�[�h�C���[�W�̃T�C�Y��ύX
		float size = WINDOW_WIDTH * 2.0f * ratio;
		mpFadeImage->SetSize(size, size);
		// �t�F�[�h�摜�̈ʒu��^�񒆂ɐݒ�
		CVector2 pos = CVector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
		mpFadeImage->SetPos(pos - CVector2(size / 2.0f, size / 2.0f));
		break;
	}

	// �o�ߎ��ԉ��Z
	mElapsedTime += Times::DeltaTime();
}

// �t�F�[�h�I���̏���
void CFade::FadeEnd()
{
	mIsFading = false;
	mElapsedTime = mFadeTime;
	
	switch (mType)
	{
		// �Ó]�t�F�[�h
	case EFadeType::eBlackOut:
		mFadeColor.A(mIsFadeIn ? 0.0f : 1.0f);
		mpFadeImage->SetColor(mFadeColor);
		break;

		// �Q��̃t�F�[�h
	case EFadeType::eVortex:
	{
		// �t�F�[�h�C���[�W�̃T�C�Y��ύX
		float size = (mIsFadeIn ? 0.0f : WINDOW_WIDTH * 2.0f);
		mpFadeImage->SetSize(size, size);
		// �T�C�Y��0�Ȃ炱���ŏI��
		if (size == 0.0f) return;
		// �t�F�[�h�摜�̈ʒu��^�񒆂ɐݒ�
		CVector2 pos = CVector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
		mpFadeImage->SetPos(pos - CVector2(size / 2.0f, size / 2.0f));
		break;
	}
	}
}
