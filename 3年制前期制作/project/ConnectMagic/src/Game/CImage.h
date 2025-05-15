#pragma once
#include "CVector.h"
#include "CTexture.h"
#include "CUIBase.h"

//2Dイメージ描画用クラス
//読み込んだCTextureを2D表示する
class CImage : public CUIBase
{
private:
	CTexture* mpTexture;	//表示するイメージのテクスチャ
	CVector4 mUV;			//表示するテクスチャ上の範囲（UV）
	bool mIsRot;			// 回転するか
	float mRotSpeed;		// 回転速度
	float mAngle;			// 回転角度

public:
	//コンストラクタ
	CImage(const char* path,
		ETaskPriority prio = ETaskPriority::eUI,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false,
		bool addTaskList = true);
	//デストラクタ
	~CImage();

	//テクスチャの読み込み
	void Load(const char* path, bool dontDelete = false);

	//表示する範囲を設定（CVector4版）
	void SetUV(const CVector4& uv);
	//表示する範囲を設定
	void SetUV(const float& left, const float& top, const float& right, const float& bottom);
	//表示する範囲を取得
	const CVector4& GetUV() const;

	// 回転するかを設定
	void SetRot(bool isRot);
	// 回転速度を設定
	void SetRotSpeed(float rotSpeed);

	//描画
	void Render();
};
