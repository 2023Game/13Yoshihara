#pragma once
#include "CInteractObject.h"
#include "CModel.h"
class CColliderMesh;
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

	void Update();
	void Render();

private:
	CModel* mpModel;
	CBuyMenu* mpBuyMenu;	// �w�����
};
