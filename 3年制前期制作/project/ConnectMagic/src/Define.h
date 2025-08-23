#pragma once

#define GAME_TITLE "ConnectMagic"	//ゲームタイトル

#define WINDOW_WIDTH 1920 * 0.8f		//ウィンドウの幅
#define WINDOW_HEIGHT 1080 * 0.8f		//ウィンドウの高さ

#define CAMERA_FOVY 60.0f		//カメラの画角
#define CAMERA_ZNEAR 1.0f		//カメラの描画最短距離
#define CAMERA_ZFAR 10000.0f		//カメラの描画最長距離

//#define FULL_SCREEN	//フルスクリーンにする場合は定義する

//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//リソースファイルフォルダ
#define RES_DIR "res\\"
//領域解放をマクロ化
#define SAFE_DELETE(del) {if(del != nullptr) delete del; del = nullptr;}
//動的配列の領域解放をマクロ化
#define SAFE_DELETE_ARRAY(ary) {if(ary != nullptr) delete[] ary; ary = nullptr;}

// 重力
#define GRAVITY 0.0625f

// 場外判定の高さ
#define MAX_UNDER_POS -100.0f



#include "System.h"
#include "Times.h"
#include "Blend.h"
#include "CDebugCamera.h"
#include "CDebugPrint.h"
#include "CDebugProfiler.h"
#include "CResourceManager.h"
