#pragma once
#include "CBillBoardImage.h"

class CBillBoardUI : public CBillBoardImage
{
public:
	// コンストラクタ
	CBillBoardUI(std::string path, ETag tag, ETaskPauseType pause);
	// デストラクタ
	~CBillBoardUI();

	// 最前面にカメラの方向で描画する
	void Render(CMaterial* mpMaterial);
private:

};