#include "CEnemy.h"
#include "CharaStatusDefine.h"
#include "CColliderSphere.h"
#include "CPlayer.h"
#include "Maths.h"
#include "SpellDefine.h"
#include "CEnemyContext.h"
#include "CEnemyIdleState.h"
#include "CEnemyCastState.h"

// �̂̔��a
#define BODY_RADIUS 4.0f
// �T�m�̔��a
#define SEARCH_RADIUS 100.0f

// �r�������̃I�t�Z�b�g���W
#define SPELL_TEXT_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.4f, WINDOW_HEIGHT * 0.4f, 0.0f)

// �r�������̃T�C�Y
#define SPELL_TEXT_SIZE 32

// �c�肪���Ȃ��Ɣ��肷��
#define LOW_PER 0.2f

// �����鋗���̔{��
#define RUN_DIST_RATIO 1.5f

// �R���X�g���N�^
CEnemy::CEnemy(ESpellElementalType elemental)
	: CEnemyBase(0.0f, 0.0f, {}, 0.0f)
	, CEnemyStatus()
	, CCastSpellStr(this, ECastType::eBasic, {}, SPELL_TEXT_UI_OFFSET_POS, SPELL_TEXT_SIZE)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mState(CEnemyIdleState::Instance())
	, mIsSpellComing(false)
	, mPriorityScore(0.0f)
	, mSpellMoveDir(CVector::zero)
{
	// ���C�������̕������ݒ�
	SetMainElementalStr(elemental);
	// �r������`�̕������ݒ�
	SetCastShapeStr(ESpellShapeType::eBall);

	// �d�͖���
	mIsGravity = false;
	// �ړ������������Ȃ�
	mIsMoveDir = false;

	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
	SAFE_DELETE(mpSpellSearch);
}

// �X�V
void CEnemy::Update()
{
	// �r����ԂłȂ����
	if (mState != CEnemyCastState::Instance())
	{
		// �œK�ȍs���ɕύX
		ChangeBestState();
	}

	// �v���C���[�̕���������������
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// ����������
	mMoveSpeed -= mMoveSpeed * DECREASE_MOVE_SPEED;

	// null�łȂ���΍X�V
	if (mState != nullptr)
		mState->Update(this);

	// MP�̍Đ�
	RegeneMp();

	// �G�̊��N���X�̍X�V
	CEnemyBase::Update();
	// �r�������w��N���X�̍X�V
	CCastSpellStr::Update();

#if _DEBUG
	CDebugPrint::Print("EnemyHit:%d\n", mHitCount);
	CDebugPrint::Print("EnemyHp:%d\n", GetHp());
	CDebugPrint::Print("EnemyMp:%f\n", GetMp());
	// �󋵂��擾
	CEnemyContext::EnemyContext context = GetContext();
	// �X�R�A�]���N���X
	CEnemyContext* enemyContext = CEnemyContext::Instance();

	CDebugPrint::Print("EnemyMpRatio:%f\n", context.mpRatio);
	CDebugPrint::Print("EnemyScoreIdle:%f\n", enemyContext->ScoreIdle(context));
	CDebugPrint::Print("EnemyScoreCast:%f\n", enemyContext->ScoreCast(context));
	CDebugPrint::Print("EnemyScoreChase:%f\n", enemyContext->ScoreChase(context));
	CDebugPrint::Print("EnemyScoreDodge:%f\n", enemyContext->ScoreDodge(context));
	CDebugPrint::Print("EnemyScoreRun:%f\n", enemyContext->ScoreRun(context));
	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("SpellComing:%s\n", mIsSpellComing ? "true" : "false");
#endif

	// ���ł��Ă��邩�����Z�b�g
	SetSpellComing(false);
}

// ���C�������̕������ݒ�
void CEnemy::SetMainElementalStr(ESpellElementalType elemental)
{
	mMainElementalStr = GetElementStr(elemental);
}

// ���C�������̕�������擾
std::string CEnemy::GetMainElementalStr() const
{
	return mMainElementalStr;
}

// �r������`�̕������ݒ�
void CEnemy::SetCastShapeStr(ESpellShapeType shape)
{
	mCastShapeStr = GetShapeStr(shape);
}

// �r������`�̕�������擾
std::string CEnemy::GetCastShapeStr() const
{
	return mCastShapeStr;
}

// ��ԓ��̃X�e�b�v��ݒ�
void CEnemy::SetStateStep(int step)
{
	mStateStep = step;
}

// ��ԓ��̃X�e�b�v���擾
int CEnemy::GetStateStep() const
{
	return mStateStep;
}

// �o�ߎ��Ԃ�ݒ�
void CEnemy::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// �o�ߎ��Ԃɉ��Z
void CEnemy::AddElapsedTime(float addTime)
{
	mElapsedTime += addTime;
}

// �o�ߎ��Ԃ��擾
float CEnemy::GetElapsedTime() const
{
	return mElapsedTime;
}

// ���������ł��Ă��邩��ݒ�
void CEnemy::SetSpellComing(bool enable, ESpellShapeType shape, float score, CVector moveDir)
{
	// ���ł��Ă���Ȃ�
	if (enable)
	{
		// ��ڂ̒T�m�Ȃ�
		if (!mIsSpellComing)
		{
			// �����̌`��ݒ�
			mComingSpellShape = shape;
			// �D��x�X�R�A��ݒ�
			mPriorityScore = score;
			// �����̈ړ�������ݒ�
			mSpellMoveDir = moveDir;
		}
		// ��ڈȍ~�Ȃ�
		else
		{
			// �V���������̕����D��x�������Ȃ�
			if (mPriorityScore < score)
			{
				// �����̌`��ݒ�
				mComingSpellShape = shape;
				// �D��x�X�R�A��ݒ�
				mPriorityScore = score;
				// �����̈ړ�������ݒ�
				mSpellMoveDir = moveDir;
			}
		}
	}

	mIsSpellComing = enable;
}

// ���ł��Ă�������̈ړ��������擾
CVector CEnemy::GetComingSpellMoveDir() const
{
	return mSpellMoveDir;
}

// �R���C�_�[�𐶐�
void CEnemy::CreateCol()
{
	// �{�̃R���C�_
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eEnemy,
		BODY_RADIUS
	);
	// �t�B�[���h,�ǁA�I�u�W�F�N�g�ƃv���C���[�ƍU���Ƃ����Փ�
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::ePlayer,
		ELayer::eAttackCol});

	// �����T�m�p
	mpSpellSearch = new CColliderSphere
	(
		this, ELayer::eSpellSearch,
		SEARCH_RADIUS
	);
	// �U���Ƃ����Փ�
	mpSpellSearch->SetCollisionLayers({ ELayer::eAttackCol });
}

// �󋵂��擾
CEnemyContext::EnemyContext CEnemy::GetContext()
{	
	// �v���C���[���擾
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
	// �󋵏��
	CEnemyContext::EnemyContext context;
	// HP������ݒ�
	context.hpRatio = (float)GetHp() / (float)GetMaxHp();
	// MP������ݒ�
	context.mpRatio = (float)GetMp() / (float)GetMaxMp();
	// �v���C���[��HP������ݒ�
	context.hpRatioP = (float)player->GetHp() / (float)GetMaxHp();
	// �v���C���[��MP������ݒ�
	context.mpRatioP = (float)player->GetMp() / (float)GetMaxMp();
	// �v���C���[�܂ł̋�����ݒ�
	context.distanceToPlayer = (player->Position() - Position()).Length();

	// �v���C���[���r�������r����ԂȂ�
	if (player->IsCasting() || player->IsCastState())
	{
		// �r�����Ă���
		context.isPlayerCasting = true;
	}
	// �v���C���[���r�����łȂ�
	else
	{
		context.isPlayerCasting = false;
	}
	// ���������ł��Ă��邩
	context.isSpellComing = mIsSpellComing;
	// ���ł��Ă�������̌`
	context.shape = mComingSpellShape;
	// ���ł��Ă�������̃X�R�A
	context.comingSpellScore = mPriorityScore;

	return context;
}

// �œK�ȍs���ɕύX����
void CEnemy::ChangeBestState()
{
	// �œK�ȍs���ɕύX
	ChangeState(CEnemyContext::Instance()->GetBestState(GetContext()));
}

// ��Ԑ؂�ւ�
void CEnemy::ChangeState(CEnemyStateBase* state)
{
	// null�������Ȃ珈�����Ȃ�
	if (state == nullptr ||
		mState == state) return;
	// ���݂̏�Ԃ͏I��
	mState->End(this);
	// ��Ԃ�ύX
	mState = state;
	// �V������Ԃ��J�n
	mState->Start(this);
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CEnemy::GetStateStr(CEnemyStateBase* state) const
{
	return state->GetName();
}
#endif