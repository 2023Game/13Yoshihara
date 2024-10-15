#pragma once
#include "CInteractObject.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
class CGameMenu;

class CComputer : public CInteractObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">3d���f��</param>
	/// <param name="pos">���W</param>
	/// <param name="scale">�傫��</param>
	CComputer(CModel* model, const CVector& pos, const CVector& scale);
	~CComputer();

	void Update();
	void Render();
	//�v���C���[�C���^���N�g���̏���
	void Interact();

private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;
	CGameMenu* mpJobMenu;
};
