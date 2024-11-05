#pragma once
#include "CInteractObject.h"
class CModel;
class CColliderMesh;
class CStageSelectMenu;

class CDoor : public CInteractObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">3d���f��</param>
	/// <param name="pos">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="rotation">��]</param>
	CDoor(CModel* model, const CVector& pos,
		const CVector& scale, const CVector& rotation);
	~CDoor();

	void Update();
	void Render();
private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;
	CStageSelectMenu* mpStageSelectMenu;	// �X�e�[�W�I�����j���[
};