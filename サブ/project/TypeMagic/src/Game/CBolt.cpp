#include "CBolt.h"

// �R���X�g���N�^
CBolt::CBolt(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBolt, owner, target)
{
}

// �f�X�g���N�^
CBolt::~CBolt()
{
}

// �R���C�_�[�𐶐�
void CBolt::CreateCol()
{
}

// ���˒��̍X�V
void CBolt::UpdateShooting()
{
}
