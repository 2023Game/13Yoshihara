#pragma once
#include "CInteractObject.h"
#include "CModel.h"

class CSound;

class CDoor : public CInteractObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">3dモデル</param>
	/// <param name="pos">座標</param>
	/// <param name="scale">大きさ</param>
	/// <param name="rotation">回転</param>
	CDoor(CModel* model, const CVector& pos,
		const CVector& scale, const CVector& rotation);
	~CDoor();

	void Interact() override;

	void Render();
private:
	CModel* mpModel;
	CCollider* mpCollider;

	// 効果音
	CSound* mpBuzzerSE;
};