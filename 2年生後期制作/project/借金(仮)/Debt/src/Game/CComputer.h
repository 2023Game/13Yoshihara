#pragma once
#include "CRideableObject.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"

class CComputer : public CRideableObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">3dモデル</param>
	/// <param name="pos">座標</param>
	/// <param name="scale">大きさ</param>
	CComputer(CModel* model, const CVector& pos, const CVector& scale);
	~CComputer();

	void Update();
	void Render();
	//プレイヤーインタラクト時の処理
	void Interact();

private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;
};
