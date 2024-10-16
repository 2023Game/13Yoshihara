#include "CComputer.h"
#include "CGameMenu.h"
#include "CPlayer.h"

CComputer::CComputer(CModel* model, const CVector& pos, const CVector& scale, const CVector& rotation)
	: mpModel(model)
{
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);
	Rotate(rotation);

	//仕事選択メニューを作成
	mpJobMenu = new CGameMenu();
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
	//プレイヤーのインタラクトオブジェクトがコンピュータなら
	if (CPlayer::Instance()->GetInteractObject() == "eComputer")
	{
		Open();
	}
	else
	{
		Close();
	}
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}

void CComputer::Open()
{
	//仕事選択メニューを開いていないなら開く
	if (!mpJobMenu->IsOpened())
	{
		mpJobMenu->Open();
	}
}

void CComputer::Close()
{
	//仕事選択メニューが開いていたら閉じる
	if (mpJobMenu->IsOpened())
	{
		mpJobMenu->Close();
	}
}
