#pragma once
#include "CTask.h"

class CExpandButton;

class CScrollUIBase : public CTask
{
public:
	// �R���X�g���N�^
	CScrollUIBase(const CVector2& pos, const CVector2& size,
		ETaskPriority prio, int sortOrder,
		ETaskPauseType pause, bool dontDelete, bool addTaskList);
	// �f�X�g���N�^
	~CScrollUIBase();

	// �X�V
	void Update() override;

	// �`��
	void Render() override;

private:
	CVector2 mPosition;	// ���W
	CVector2 mSize;		// �T�C�Y
	std::vector<CExpandButton*> mButtons;
};