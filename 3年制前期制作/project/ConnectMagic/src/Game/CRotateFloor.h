#pragma once
#include "CRideableObject.h"
#include "CModel.h"

class CRotateFloor : public CRideableObject
{
public:
	CRotateFloor(CModel* model, const CVector& pos, const CVector& scale, float rotateSpeedY);
	~CRotateFloor();

	void Update();
	void Render();

private:
	void CreateCol() override;

	CModel* mpModel;

	float mRotateSpeedY;
	// メッシュのデータ
	std::vector<float> mMeshVertices;
	std::vector<int> mMeshIndices;
};