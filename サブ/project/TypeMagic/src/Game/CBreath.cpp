#include "CBreath.h"

// �R���X�g���N�^
CBreath::CBreath(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBreath, owner, target, BREATH_DELETE_TIME)
{
}

// �f�X�g���N�^
CBreath::~CBreath()
{
}

// �R���C�_�[�𐶐�
void CBreath::CreateCol()
{
	
}

// ���˒��̍X�V
void CBreath::UpdateShooting()
{
}
