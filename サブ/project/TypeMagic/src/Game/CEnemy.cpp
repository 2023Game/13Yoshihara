#include "CEnemy.h"
#include "CharaStatusDefine.h"
#include "CColliderSphere.h"
#include "CPlayer.h"
#include "Maths.h"
#include "SpellDefine.h"
#include "CEnemyContext.h"
#include "CEnemyIdleState.h"

// �̂̔��a
#define BODY_RADIUS 4.0f

// �r�������̃I�t�Z�b�g���W
#define SPELL_TEXT_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.2f, 0.0f)

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
	, mMainElemental(elemental)
	, mCastShape(ESpellShapeType::eBall)
	, mState(CEnemyIdleState::Instance())
{
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
}

// �X�V
void CEnemy::Update()
{
	// �œK�ȍs���ɕύX
	ChangeBestState();

	// �v���C���[�̕���������������
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// ����������
	mMoveSpeed -= mMoveSpeed * DECREASE_MOVE_SPEED;

	// null�łȂ���΍X�V
	if (mState != nullptr)
		mState->Update(this);

	// �G�̊��N���X�̍X�V
	CEnemyBase::Update();
	// �r�������w��N���X�̍X�V
	CCastSpellStr::Update();

#if _DEBUG
	CDebugPrint::Print("EnemyHit:%d\n", mHitCount);
	CDebugPrint::Print("EnemyHp:%d\n", GetHp());
	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
#endif
}

// ���C��������ݒ�
void CEnemy::SetMainElemental(ESpellElementalType elemental)
{
	mMainElemental = elemental;
}

// ���C���������擾
ESpellElementalType CEnemy::GetMainElemental() const
{
	return mMainElemental;
}

// �r������`���擾
ESpellShapeType CEnemy::GetCastShape() const
{
	return mCastShape;
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

// �o�ߎ��Ԃ��擾
float CEnemy::GetElapsedTime() const
{
	return mElapsedTime;
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
}

// �œK�ȍs���ɕύX����
void CEnemy::ChangeBestState()
{
	// �v���C���[���擾
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
	// �󋵏��
	CEnemyContext::EnemyContext context;
	// HP������ݒ�
	context.hpRatio = GetHp() / GetMaxHp();
	// MP������ݒ�
	context.mpRatio = GetMp() / GetMaxMp();
	// �v���C���[�܂ł̋�����ݒ�
	context.distanceToPlayer = (player->Position() - Position()).Length();
	// �v���C���[���r�����Ă��邩
	context.isPlayerCasting = player->IsCasting();
	// �߂��ɕǂ����邩��ݒ�
	context.isNearWall = false;

	// �œK�ȍs���ɕύX
	ChangeState(CEnemyContext::Instance()->GetBestState(context));
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