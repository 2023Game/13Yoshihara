#include "CEnemy.h"
#include "CharaStatusDefine.h"
#include "CColliderSphere.h"
#include "CPlayer.h"
#include "Maths.h"
#include "SpellDefine.h"
#include "CEnemyContext.h"
#include "CEnemyIdleState.h"
#include "CEnemyCastState.h"
#include "CGaugeUI3D.h"
#include "CEnemyManager.h"
#include "CTextUI3D.h"

// �̂̔��a
#define BODY_RADIUS 4.0f
// �T�m�̔��a
#define SEARCH_RADIUS 100.0f

// �r�������̃I�t�Z�b�g���W
#define SPELL_TEXT_UI_OFFSET_POS CVector(30.0f, 30.0f, 0.0f)

// �r�������̃T�C�Y
#define SPELL_TEXT_SIZE 32

// �c�肪���Ȃ��Ɣ��肷��
#define LOW_PER 0.2f

// �����鋗���̔{��
#define RUN_DIST_RATIO 1.5f

// �Q�[�W�̃p�X
#define GAUGE_PATH "UI\\gauge.png"
// �Q�[�W�̍��W
#define GAUGE_POS CVector(0.0f,50.0f,0.0f)
// �Q�[�W�̑傫��
#define GAUGE_SIZE 2.0f
// �Q�[�W���m�̊Ԋu
#define GAUGE_DIST 7.5f

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
	, mSpellDist(0.0f)
{
	// ���C�������̕������ݒ�
	SetMainElementalStr(elemental);
	// �r������`�̕������ݒ�
	SetCastShapeStr(ESpellShapeType::eBall);

	// �d�͖���
	mIsGravity = false;
	// �ړ������������Ȃ�
	mIsMoveDir = false;

	// HP�Q�[�W��ݒ�
	mpHpGauge = new CGaugeUI3D(this, GAUGE_PATH, true, CGaugeUI3D::EGaugeType::eHpGauge);
	mpHpGauge->SetShow(false);
	mpHpGauge->Position(Position() + GAUGE_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

	// MP�Q�[�W��ݒ�
	mpMpGauge = new CGaugeUI3D(this, GAUGE_PATH, true, CGaugeUI3D::EGaugeType::eMpGauge);
	mpMpGauge->SetShow(false);
	mpMpGauge->Position(Position() + GAUGE_POS + CVector(0.0f, -GAUGE_DIST, 0.0f));
	mpMpGauge->SetMaxPoint(GetMaxMp());
	mpMpGauge->SetCurrPoint(GetMp());

	// �����̕�����e�L�X�g�̎������ݒ�
	mpSpellText->SetOwner(this);

	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
	SAFE_DELETE(mpSpellSearch);

	// MP�Q�[�W�����݂�����A�ꏏ�ɍ폜����
	if (mpMpGauge != nullptr)
	{
		mpMpGauge->SetOwner(nullptr);
		mpMpGauge->Kill();
	}
}

// �X�V
void CEnemy::Update()
{
	// �v���C���[�̕���������������
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// ���X�Ɍ���
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

	// HP�Q�[�W�̍X�V
	mpHpGauge->Position(Position() + GAUGE_POS);
	mpHpGauge->SetShow(true);
	mpHpGauge->SetCurrPoint(GetHp());
	// MP�Q�[�W�̍X�V
	mpMpGauge->Position(Position() + GAUGE_POS + CVector(0.0f, -GAUGE_DIST, 0.0f));
	mpMpGauge->SetShow(true);
	mpMpGauge->SetCurrPoint(GetMp());	
	
	// �r��������̃e�L�X�g�̍X�V
	CTextUI3D* text3d = dynamic_cast<CTextUI3D*>(mpSpellText);
	if (text3d != nullptr)
	{
		text3d->SetWorldPos(Position() + VectorX() * SPELL_TEXT_UI_OFFSET_POS.X() +
			CVector(0.0f,
				SPELL_TEXT_UI_OFFSET_POS.Y(),
				SPELL_TEXT_UI_OFFSET_POS.Z()));
	}

#if _DEBUG
	//CDebugPrint::Print("EnemyHp:%d\n", GetHp());
	//CDebugPrint::Print("EnemyMp:%f\n", GetMp());
	//// �󋵂��擾
	//CEnemyContext::EnemyContext context = GetContext();
	//// �X�R�A�]���N���X
	//CEnemyContext* enemyContext = CEnemyContext::Instance();

	//CDebugPrint::Print("EnemyMpRatio:%f\n", context.mpRatio);
	//CDebugPrint::Print("EnemyScoreIdle:%f\n", enemyContext->ScoreIdle(context));
	//CDebugPrint::Print("EnemyScoreCast:%f\n", enemyContext->ScoreCast(context));
	//CDebugPrint::Print("EnemyScoreChase:%f\n", enemyContext->ScoreChase(context));
	//CDebugPrint::Print("EnemyScoreDodge:%f\n", enemyContext->ScoreDodge(context));
	//CDebugPrint::Print("EnemyScoreRun:%f\n", enemyContext->ScoreRun(context));
	//CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
	//CDebugPrint::Print("SpellComing:%s\n", mIsSpellComing ? "true" : "false");
#endif

	// ���ł��Ă��邩�����Z�b�g
	SetSpellComing(false);
}

// �I�u�W�F�N�g�폜����
void CEnemy::DeleteObject(CObjectBase* obj)
{
	CEnemyBase::DeleteObject(obj);

	// �폜���ꂽ�̂������̃e�L�X�g�Ȃ�
	// �|�C���^����ɂ���
	if (obj == mpSpellText)
	{
		mpSpellText = nullptr;
	}
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
void CEnemy::SetSpellComing(bool enable, ESpellShapeType shape,
	float score, CVector moveDir, float dist)
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
			// �����Ƃ̋�����ݒ�
			mSpellDist = dist;
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
				// �����Ƃ̋�����ݒ�
				mSpellDist = dist;
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

// ���S����
void CEnemy::Death()
{
	// �폜
	CEnemyManager::Instance()->DeleteEnemy(this);
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
	// �����܂ł̋�����ݒ�
	context.distanceToSpell = mSpellDist;

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
	context.comingShape = mComingSpellShape;
	// ���ł��Ă�������̃X�R�A
	context.comingSpellScore = mPriorityScore;

	// �V�[���h��\���Ă��邩
	context.isShield = mIsShield;
	// �r�����̎���
	context.castShape = mSpellShape;

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