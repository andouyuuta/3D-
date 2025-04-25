#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "GameClear.h"


// コンストラクタ
GameClear::GameClear(void) : SceneBase()
{

}

// デストラクタ
GameClear::~GameClear(void)
{
}

// 初期化
void GameClear::Init(void)
{
	// 画像の読み込み

}

// 更新
void GameClear::Update(void)
{
	// インスタンスの取得
	InputManager& ins = InputManager::GetInstance();

	// マウス座標の更新
	mousePos_ = ins.GetMousePos();

	if (mousePos_.x >= 500 && mousePos_.x <= 600 && mousePos_.y >= 500 && mousePos_.y <= 600)
	{
		mouseFlg_ = true;
	}
	else
	{
		mouseFlg_ = false;
	}

	// シーン遷移
	if (mouseFlg_)
	{
		if (ins.IsTrgMouseLeft())
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}

// 描画
void GameClear::Draw(void)
{
	// タイトル画像の描画


	// 透過素材の描画


	// 文字の描画
	DrawFormatString(20, 20, GetColor(0xff, 0xff, 0xff), "ゲームクリア画面");
	DrawFormatString(300, 300, GetColor(0xff, 0xff, 0xff), "左クリックでスタート");

	if (mouseFlg_)
	{
		DrawBox(500, 500, 600, 600, GetColor(128, 128, 128), true);
	}
	else
	{
		DrawBox(500, 500, 600, 600, GetColor(255, 255, 255), true);
	}
}

// 解放
void GameClear::Release(void)
{
	// 画像の解放

}