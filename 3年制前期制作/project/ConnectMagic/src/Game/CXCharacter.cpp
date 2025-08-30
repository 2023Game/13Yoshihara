#include "CXCharacter.h"
#include "Maths.h"

//�R���X�g���N�^
CXCharacter::CXCharacter(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CCharaBase(tag, prio, sortOrder, pause)
	, mpModel(nullptr)
	, mpCombinedMatrix(nullptr)
	, mAnimationLoopFlg(false)
	, mAnimationIndex(0)
	, mAnimationFrame(0.0f)
	, mAnimationFrameSize(0.0f)
	, mAnimationSpeed(1.0f)
	, mElapsedBlend(0.0f)
	, mBlendIndex(-1)
{
}

//�f�X�g���N�^
CXCharacter::~CXCharacter()
{
	SAFE_DELETE_ARRAY(mpCombinedMatrix)
}

/*
Init
����������
*/
void CXCharacter::Init(CModelX* model)
{
	mpModel = model;
	//�����s��ޔ��G���A�̊m��
	mpCombinedMatrix = new CMatrix[model->Frames().size()];
	//�ŏ��̃A�j���[�V�����ɂ���
	mAnimationIndex = 0;
	//�J��Ԃ��Đ�����
	mAnimationLoopFlg = true;
	//1�A�j���[�V�����ڂ̍ő�t���[����
	mAnimationFrameSize = model->AnimationSet()[0]->MaxTime();
	//�A�j���[�V�����̃t���[�����ő�ɂ���
	mAnimationFrame = 0.0f;
	mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
	//�A�j���[�V�����̏d�݂�1.0(100%)�ɂ���
	mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f);
}

// �A�j���[�V�����ƃ��f���̏���������
void CXCharacter::InitAnimationModel(std::string path, const std::vector<AnimData>* pAnimData)
{
	// �A�j���[�V�����f�[�^�e�[�u����ݒ�
	mpAnimData = pAnimData;

	// ���f���f�[�^��ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>(path);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = mpAnimData->size();
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = (*mpAnimData)[i];
		if (data.path.empty())continue;
		model->AddAnimationSet(data.path.c_str());
	}

	// ���f���̏�����
	Init(model);
}


/*
ChangeAnimation
�A�j���[�V������؂�ւ���
index:�A�j���[�V�����̔ԍ�
loop:true:�J��Ԃ�
framesize:�Ō�܂ōĐ�����̂Ɏg�p�����t���[����
*/
void CXCharacter::ChangeAnimation(int index, bool loop,
	float framesize, bool restart)
{
	// �u�����h�̐i�s�x��0�ɂ���
	mElapsedBlend = 0.0f;
	if (mBlendIndex != -1)
	{
		// �u�����h�̃A�j���[�V�����̏d�݂�0.0(0%)�ɂ���
		mpModel->AnimationSet()[mBlendIndex]->Weight(0.0f);
		mBlendIndex = -1;
	}
	//�ŏ�����J�n���Ȃ��ꍇ���A�����A�j���[�V�����̏ꍇ�͐؂�ւ��Ȃ�
	if (!restart && mAnimationIndex == index) return;
	//���̃A�j���[�V�����̏d�݂�0.0(0%)�ɂ���
	mpModel->AnimationSet()[mAnimationIndex]->Weight(0.0f);
	//�ԍ��A�J��Ԃ��A�t���[�������w��
	mAnimationIndex = index % mpModel->AnimationSet().size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//�A�j���[�V�����̃t���[�����ŏ��ɂ���
	mAnimationFrame = 0.0f;
	mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
	//�A�j���[�V�����̏d�݂�1.0(100%)�ɂ���
	mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f);
}

/*
ChangeAnimationBlend
�A�j���[�V�������u�����h���Ȃ���؂�ւ���
index:�A�j���[�V�����̔ԍ�
loop:true:�J��Ԃ�
framesize:�Ō�܂ōĐ�����̂Ɏg�p�����t���[����
restart:�ŏ�����J�n���邩
blendSpeed:�u�����h���Ȃ���؂�ւ��鑬�x
*/
bool CXCharacter::ChangeAnimationBlend(int index, bool loop, 
	float framesize, bool restart, float blendSpeed)
{
	//�ŏ�����J�n���Ȃ��ꍇ���A�����A�j���[�V�����̏ꍇ�͐؂�ւ��Ȃ�
	if (!restart && mAnimationIndex == index) return true;

	// �u�����h����A�j���[�V�����ԍ��ƈႤ�Ȃ珈������
	if (mBlendIndex != index);
	{
		if (mBlendIndex != -1)
		{
			// ���݂̃u�����h����A�j���[�V�����̃t���[�����ŏ��ɂ���
			mpModel->AnimationSet()[mBlendIndex]->Time(0.0f);
			// ���݂̃u�����h����A�j���[�V�����̏d�݂�0�ɂ���
			mpModel->AnimationSet()[mBlendIndex]->Weight(0.0f);
		}
		// �u�����h����A�j���[�V�����ԍ���ݒ�
		mBlendIndex = index % mpModel->AnimationSet().size();
		// ���̃u�����h����A�j���[�V�����̃t���[�����ŏ��ɂ���
		mpModel->AnimationSet()[mBlendIndex]->Time(0.0f);
	}
	// �u�����h�i�s�x��i�߂�
	mElapsedBlend += blendSpeed * Times::DeltaTime();

	// �i�s�x��1.0��菬�����ꍇ
	if (mElapsedBlend < 1.0f)
	{
		// ���̃A�j���[�V�����̏d�݂�1.0�|�i�s�x�ɂ���
		mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f - mElapsedBlend);
		// ���̃A�j���[�V�����̏d�݂�i�s�x�ɂ���
		mpModel->AnimationSet()[mBlendIndex]->Weight(mElapsedBlend);

		// �u�����h���I�����Ă��Ȃ�
		return false;
	}
	// �i�s�x��1.0�ȏ�̏ꍇ
	else
	{
		// �A�j���[�V������ύX
		ChangeAnimation(index, loop, framesize, restart);

		// �u�����h�I��
		return true;
	}
}

/*
Update
�X�V����
matrix:�ړ��A��]�A�g��k���̍s��
*/
void CXCharacter::Update(const CMatrix& matrix)
{
	if (mpModel == nullptr) return;

	auto& animSet = mpModel->AnimationSet();
	CAnimationSet* currAnim = animSet[mAnimationIndex];
	CAnimationSet* blendAnim;
	// �u�����h����A�j���[�V�������ݒ肳��Ă���Ȃ�
	if (mBlendIndex >= 0)
	{
		blendAnim = animSet[mBlendIndex];
	}

	for (size_t i = 0; i < animSet.size(); i++)
	{
		animSet[i]->mWeight = 0.0f;
		currAnim->mWeight = 1.0f - mElapsedBlend;
		// �u�����h����A�j���[�V�������ݒ肳��Ă���Ȃ�
		if (mBlendIndex >= 0)
		{
			blendAnim->mWeight = mElapsedBlend;
		}
	}
	//�Ō�܂ōĐ�����
	if (0.0f <= mAnimationFrame && mAnimationFrame <= mAnimationFrameSize)
	{
		// �i�ރt���[����
		float frame = mAnimationSpeed * 60.0f * Times::DeltaTime();

		//�A�j���[�V�����̎��Ԃ�ݒ�
		currAnim->Time(mAnimationFrame);
		// �u�����h����A�j���[�V�������ݒ肳��Ă���Ȃ�
		if (mBlendIndex >= 0)
		{
			// �u�����h����A�j���[�V�����̎���
			float blendTime = blendAnim->mTimeLocation;
			blendAnim->Time(blendTime + frame);
		}

		//�t���[����i�߂�
		mAnimationFrame += frame;
	}
	else
	{
		//�A�j���[�V�������J��Ԃ��ꍇ
		if (mAnimationLoopFlg)
		{
			//�A�j���[�V�����̃t���[���������[�v������
			float maxTime = currAnim->MaxTime();
			if (maxTime > 0.0f)
			{
				mAnimationFrame = fmodf(mAnimationFrame + maxTime, maxTime);
			}
			else
			{
				mAnimationFrame = 0.0f;
			}
			currAnim->Time(mAnimationFrame);
		}
		//�A�j���[�V�������J��Ԃ��Ȃ��ꍇ
		else
		{
			//�ŏI�t���[���ɐݒ�
			currAnim->TimeProgress(mAnimationFrame >= 0.0f ? 1.0f : 0.0f);
		}
	}
	//�t���[���̕ϊ��s����A�j���[�V�����ōX�V����
	mpModel->AnimateFrame();
	//�t���[���̍����s����v�Z����
	mpModel->Frames()[0]->AnimateCombined(&matrix);
	//�����s��̑ޔ�
	for (size_t i = 0; i < mpModel->Frames().size(); i++)
	{
		mpCombinedMatrix[i] =
			mpModel->Frames()[i]->CombinedMatrix();
	}
}

//�X�V����
void CXCharacter::Update()
{
	//�A�j���[�V�������X�V����
	Update(Matrix());
#if _DEBUG
	CDebugPrint::Print("AnimationIndex:%d\n", mAnimationIndex);
	CDebugPrint::Print("BlendIndex:%d\n", mBlendIndex);
	CDebugPrint::Print("ElapsedBlend:%f\n", mElapsedBlend);
#endif
}

/*
Render
�`�悷��
*/
void CXCharacter::Render()
{
	if (mpModel == nullptr) return;
	mpModel->SetColor(mColor);
	mpModel->RenderShader(mpCombinedMatrix);
}

bool CXCharacter::IsAnimationFinished()
{
	return mAnimationFrame >= mAnimationFrameSize;
}

int CXCharacter::AnimationIndex()
{
	return mAnimationIndex;
}

// �Đ����̃A�j���[�V�����t���[�����擾
float CXCharacter::GetAnimationFrame() const
{
	return mAnimationFrame;
}

// �Đ����̃A�j���[�V�����̐i�s�x���擾
float CXCharacter::GetAnimationFrameRatio() const
{
	if (mAnimationFrameSize == 0.0f) return 0.0f;
	return Math::Clamp01(mAnimationFrame / mAnimationFrameSize);
}

// �w�肵���{�[���̍s����擾
const CMatrix* CXCharacter::GetFrameMtx(std::string name) const
{
	// ���f���f�[�^���ݒ肳��Ă��Ȃ�
	if (mpModel == nullptr) return nullptr;

	// �t���[������
	CModelXFrame* frame = mpModel->FinedFrame(name.c_str());

	//�w�肳�ꂽ�t���[�������݂��Ȃ�����
	if (frame == nullptr) return nullptr;

	// �t���[���̍s��c�Ԃ�
	return &frame->CombinedMatrix();
}

// �A�j���[�V�����̍Đ����x��ݒ�
void CXCharacter::SetAnimationSpeed(float speed)
{
	mAnimationSpeed = speed;
}

// �A�j���[�V�����̍Đ����x���擾
float CXCharacter::GetAnimationSpeed() const
{
	return mAnimationSpeed;
}

// �A�j���[�V�����̍Đ��t���[����ݒ肷��
void CXCharacter::SetAnimationFrame(float frame)
{
	auto& animSet = mpModel->AnimationSet();
	CAnimationSet* currAnim = animSet[mAnimationIndex];
	mAnimationFrame = frame;
	currAnim->Time(mAnimationFrame);
}
