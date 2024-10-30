#pragma once
#include "CInteractObject.h"
class CModel;
class CColliderMesh;
class CGameMenuBase;

class CComputer : public CInteractObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">3dモデル</param>
	/// <param name="pos">座標</param>
	/// <param name="scale">大きさ</param>
	/// <param name="rotation">回転</param>
	CComputer(CModel* model, const CVector& pos,
		const CVector& scale, const CVector& rotation);
	~CComputer();

	void Update();
	void Render();

private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;
	CGameMenuBase* mpBuyMenu;	// 購入画面
};
