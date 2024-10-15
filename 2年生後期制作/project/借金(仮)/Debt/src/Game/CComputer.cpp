#include "CComputer.h"


CComputer::CComputer(CModel* model, const CVector& pos, const CVector& scale, const float& radius)
	: mpModel(model)
{
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);
}

CComputer::~CComputer()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CComputer::Update()
{
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}

void CComputer::Interact()
{
	//TODO:仕事選択メニューを呼び出す
}
