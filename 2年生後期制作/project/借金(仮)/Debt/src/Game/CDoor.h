#pragma once
#include "CInteractObject.h"
#include "CModel.h"

class CColliderRectangle;
class CStageSelectMenu;

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

	void Update();
	void Render();
private:
	CModel* mpModel;
	CColliderMesh* mpCollider;
	CStageSelectMenu* mpStageSelectMenu;	// ステージ選択メニュー
};