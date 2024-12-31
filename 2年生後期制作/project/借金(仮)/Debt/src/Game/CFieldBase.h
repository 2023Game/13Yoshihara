#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CFieldBase : public CObjectBase
{
public:
	// �t�B�[���h�̃C���X�^���X��Ԃ�
	static CFieldBase* Instance();
	// �R���X�g���N�^
	CFieldBase();
	// �f�X�g���N�^
	~CFieldBase();

	/// <summary>
	/// ���C�ƃt�B�[���h�I�u�W�F�N�g�Ƃ̏Փ˔���
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <returns>�Փ˂��Ă�����Atrue��Ԃ�</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	// �X�V
	virtual void Update();
	// �`��
	virtual void Render();

protected:
	// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
	virtual void CreateFieldObjects() = 0;

	// �t�B�[���h�̃C���X�^���X
	static CFieldBase* spInstance;
	// �t�B�[���h�̃��f��
	CModel* mpModel;
	// �t�B�[���h�̒n�ʂ̏Փ˔���
	CColliderMesh* mpGroundColliderMesh;
	// �t�B�[���h�̕ǂ̏Փ˔���
	CColliderMesh* mpWallColliderMesh;
	// �t�B�[���h�̃I�u�W�F�N�g�̏Փ˔���
	CColliderMesh* mpObjectColliderMesh;
};