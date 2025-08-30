#include "CSaveManager.h"
#include "CPlayer.h"
#include "CBox.h"
#include "CMoveObj.h"
#include "CSwitchMoveFloor.h"
#include "CSwitchMoveWall.h"
#include "CTaskManager.h"
#include "CInput.h"
#include "CConnectPointManager.h"

// �ۑ�����t���[����
#define SAVE_FRAMES 600

// �C���X�^���X
CSaveManager* CSaveManager::spInstance = nullptr;

// �C���X�^���X���擾
CSaveManager* CSaveManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CSaveManager::CSaveManager()
	: CTask(ETaskPriority::eSystem, 0, ETaskPauseType::eSystem, false, true)
	, mState(EState::eSave)
{
	spInstance = this;

	// �����߂�UI
	mpRewindUI = new CRewindUI();
	mpRewindUI->SetEnable(false);
}

// �f�X�g���N�^
CSaveManager::~CSaveManager()
{
}

// �X�V
void CSaveManager::Update()
{
	switch (mState)
	{
		// �Z�[�u
	case CSaveManager::EState::eSave:
		Save();
		break;

		// ���[�h
	case CSaveManager::EState::eLoad:

		if (CInput::Key(VK_LBUTTON)) {
			// �e�L�X�g��\��
			mpRewindUI->SetTextEnable(false);
			// �摜�̐F��ɐݒ�
			mpRewindUI->SetImgColor(CColor::blue);
			CTaskManager::Instance()->Pause(PAUSE_GAME);
			// ���[�h
			Load();
			CTaskManager::Instance()->UnPause(PAUSE_GAME);
		}
		// �L�[�𗣂�����
		// �f�[�^����ɂȂ�����
		if (CInput::PullKey(VK_LBUTTON) || mData.empty()) {
			// ���[�h�I��
			ChangeState(EState::eSave);
			// �|�[�Y����
			CTaskManager::Instance()->UnPause(PAUSE_GAME);
			// �v���C���[��ҋ@��Ԃɖ߂�
			CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
			player->ChangeState(CPlayer::EState::eIdle);
		}
		break;
	}
}

// ��Ԃ�ۑ�����
void CSaveManager::Save()
{
	// �Q�[���|�[�Y���Ȃ珈�����Ȃ�
	if (CTaskManager::Instance()->IsPaused(PAUSE_GAME)) return;

	// �f�[�^
	GameData data;
	// �v���C���[���擾
	CPlayerBase* player = CPlayerBase::Instance();
	// ���W
	data.player.pos = player->Position();
	// ����
	data.player.vec = player->EulerAngles();
	// �ڑ����Ă���^�[�Q�b�g
	data.player.target = CConnectPointManager::Instance()->GetConnectWandTarget();
	// �A�j���[�V�����ԍ�
	data.player.animationType = player->AnimationIndex();
	// �A�j���[�V�����̍Đ�����
	data.player.animationFrame = player->GetAnimationFrame();

	// �����擾
	for (CBox* box : mBox) {
		// ���X�g�ɒǉ�
		data.boxes.emplace_back(box->Position(), box);
	}

	// �ړ��I�u�W�F�N�g���擾
	for (CMoveObj* obj : mMoveObj) {
		// ���X�g�ɒǉ�
		data.moveObjs.emplace_back(obj->Position(),
			obj->GetState(), obj->GetPreState(),
			obj->GetElapsedTime(), obj);
	}

	// �����擾
	for (CSwitchMoveFloor* floor : mMoveFloor) {
		// ���X�g�ɒǉ�
		data.moveFloor.emplace_back(floor->Position(),
			floor->GetState(), floor->GetPreState(),
			floor->GetElapsedTime(), floor);
	}

	// �ǂ��擾
	for (CSwitchMoveWall* wall: mMoveWall) {
		// ���X�g�ɒǉ�
		data.moveWall.emplace_back(wall->Position(),
			wall->GetState(), wall->GetPreState(),
			wall->GetElapsedTime(), wall);
	}

	// �f�[�^�̃��X�g�ɒǉ�
	mData.push_back(data);

	// �ۑ�����t���[������葽���Ȃ�
	if (mData.size() > SAVE_FRAMES)
	{
		// ��ԌÂ���Ԃ��폜
		DeleteOld();
	}
}

// ��ԌÂ���Ԃ��폜����
void CSaveManager::DeleteOld()
{
	mData.pop_front();
}

// ��ԐV������Ԃ��폜
void CSaveManager::DeleteNew()
{
	mData.pop_back();
}

// �S�č폜����
void CSaveManager::AllDelete()
{
	mData.clear();
}

// �f�[�^�����[�h
void CSaveManager::Load()
{
	// �f�[�^���Ȃ��Ȃ�
	// �������Ȃ�
	if (mData.empty()) return;
	// ���[�h����f�[�^
	const GameData& data = mData[mData.size() - 1];

	// �v���C���[�f�[�^
	auto& playerData = data.player;
	// �v���C���[���擾
	CPlayerBase* player = CPlayerBase::Instance();
	if (player)
	{
		// ���W�����[�h
		player->Position(playerData.pos);
		// ���������[�h
		player->Rotation(playerData.vec);
		// �ڑ���L��
		CConnectPointManager::Instance()->EnableConnect(playerData.target);
		// �A�j���[�V�����ݒ�
		player->ChangeAnimationTime(playerData.animationType, playerData.animationFrame);
	}

	CBox* box = nullptr;
	for (auto& boxData : data.boxes)
	{
		// �����擾
		box = boxData.box;
		if (box)
		{
			// ���W�����[�h
			box->Position(boxData.pos);
		}
	}

	CMoveObj* moveObj = nullptr;
	for (auto& objData : data.moveObjs)
	{
		// �����I�u�W�F�N�g���擾
		moveObj = objData.moveObj;
		if (moveObj)
		{
			// ���W�����[�h
			moveObj->Position(objData.pos);
			// ��Ԃ����[�h
			moveObj->SetState(objData.state);
			// �O�̏�Ԃ����[�h
			moveObj->SetPreState(objData.preState);
			// �o�ߎ��Ԃ����[�h
			moveObj->SetElapsedTime(objData.elapsedTime);
		}
	}

	CSwitchMoveFloor* moveFloor = nullptr;
	for (auto& floorData : data.moveFloor)
	{
		// �����擾
		moveFloor = floorData.moveFloor;
		if (moveFloor)
		{
			// ���W�����[�h
			moveFloor->Position(floorData.pos);
			// ��Ԃ����[�h
			moveFloor->SetState(floorData.state);
			// �O�̏�Ԃ����[�h
			moveFloor->SetPreState(floorData.preState);
			// �o�ߎ��Ԃ����[�h
			moveFloor->SetElapsedTime(floorData.elapsedTime);
		}
	}

	CSwitchMoveWall* moveWall = nullptr;
	for (auto& wallData : data.moveWall)
	{
		// �ǂ��擾
		moveWall = wallData.moveWall;
		if (moveWall)
		{
			// ���W�����[�h
			moveWall->Position(wallData.pos);
			// ��Ԃ����[�h
			moveWall->SetState(wallData.state);
			// �O�̏�Ԃ����[�h
			moveWall->SetPreState(wallData.preState);
			// �o�ߎ��Ԃ����[�h
			moveWall->SetElapsedTime(wallData.elapsedTime);
		}
	}

	// ��ԐV������Ԃ��폜
	DeleteNew();
}

// �ۑ����锠�ɒǉ�
void CSaveManager::AddBox(CBox* box)
{
	mBox.push_back(box);
}

// �ۑ����锠����폜
void CSaveManager::DeleteBox(CBox* box)
{
	mBox.erase(
		std::remove(mBox.begin(), mBox.end(), box),
		mBox.end()
	);
}

// �ۑ�����ړ��I�u�W�F�N�g�ɒǉ�
void CSaveManager::AddMoveObj(CMoveObj* obj)
{
	mMoveObj.push_back(obj);
}

// �ۑ�����ړ��I�u�W�F�N�g����폜
void CSaveManager::DeleteMoveObj(CMoveObj* obj)
{
	mMoveObj.erase(
		std::remove(mMoveObj.begin(), mMoveObj.end(), obj),
		mMoveObj.end()
	);
}

// �ۑ�����X�C�b�`�ňړ����鏰�ɒǉ�
void CSaveManager::AddMoveFloor(CSwitchMoveFloor* floor)
{
	mMoveFloor.push_back(floor);
}

// �ۑ�����X�C�b�`�ňړ����鏰����폜
void CSaveManager::DeleteMoveFloor(CSwitchMoveFloor* floor)
{
	mMoveFloor.erase(
		std::remove(mMoveFloor.begin(), mMoveFloor.end(), floor),
		mMoveFloor.end()
	);
}

// �ۑ�����X�C�b�`�ňړ�����ǂɒǉ�
void CSaveManager::AddMoveWall(CSwitchMoveWall* wall)
{
	mMoveWall.push_back(wall);
}

// �ۑ�����X�C�b�`�ňړ�����ǂ���폜
void CSaveManager::DeleteMoveFloor(CSwitchMoveWall* wall)
{
	mMoveWall.erase(
		std::remove(mMoveWall.begin(), mMoveWall.end(), wall),
		mMoveWall.end()
	);
}

// ��Ԑ؂�ւ�
void CSaveManager::ChangeState(EState state)
{
	// �����Ȃ珈�����Ȃ�
	if (mState == state) return;

	if (state == EState::eLoad) {
		mpRewindUI->SetEnable(true);
		mpRewindUI->SetTextEnable(true);
		mpRewindUI->SetImgColor(CColor::red);
	}
	else {
		mpRewindUI->SetEnable(false);
	}

	mState = state;
}
