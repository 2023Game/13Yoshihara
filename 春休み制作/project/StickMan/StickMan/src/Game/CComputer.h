#pragma once
#include "CInteractObject.h"
#include "CModel.h"

class CBuyMenu;

class CComputer : public CInteractObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">3d���f��</param>
	/// <param name="pos">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="rotation">��]</param>
	CComputer(CModel* model, const CVector& pos,
		const CVector& scale, const CVector& rotation);
	~CComputer();

	void Interact() override;

	void Render();

private:
	CModel* mpModel;
	CCollider* mpCollider;
	CBuyMenu* mpBuyMenu;	// �w�����
};
