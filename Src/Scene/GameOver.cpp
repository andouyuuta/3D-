#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "GameOver.h"


// コンストラクタ
GameOver::GameOver(void) : SceneBase()
{

}

// デストラクタ
GameOver::~GameOver(void)
{
}

// 初期化
void GameOver::Init(void)
{
	// 画像の読み込み
	hightLight_ = LoadGraph("Data/Image/hightLight.png");

	gameFlg_ = false;
	titleFlg_ = false;
}

// 更新
void GameOver::Update(void)
{
	// インスタンスの取得
	InputManager& ins = InputManager::GetInstance();

	// マウス座標の更新
	mousePos_ = ins.GetMousePos();

	if (mousePos_.x >= 600 && mousePos_.x <= 760 && mousePos_.y >= 300 && mousePos_.y <= 316)
	{
		gameFlg_ = true;
		if (ins.IsTrgMouseLeft()) 
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
	}
	else
	{
		gameFlg_ = false;
	}
	
	if (mousePos_.x >= 600 && mousePos_.x <= 760 && mousePos_.y >= 330 && mousePos_.y <= 346)
	{
		titleFlg_ = true;
		if (ins.IsTrgMouseLeft())
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
	else
	{
		titleFlg_ = false;
	}
}

// 描画
void GameOver::Draw(void)
{
	// 透過素材の描画
	if (gameFlg_)
	{
		DrawGraph(600, 300, hightLight_, true);
	}

	if (titleFlg_)
	{
		DrawGraph(600, 330, hightLight_, true);
	}

	// 文字の描画
	DrawFormatString(20, 20, GetColor(0xff, 0xff, 0xff), "ゲームオーバー画面");
	DrawFormatString(300, 300, GetColor(0xff, 0xff, 0xff), "左クリックで決定");

	DrawFormatString(600, 300, GetColor(0xff, 0x00, 0x00), "リトライ");
	DrawFormatString(600, 330, GetColor(0xff, 0x00, 0x00), "タイトルに戻る");
}

// 解放
void GameOver::Release(void)
{
	// 画像の解放
	DeleteGraph(hightLight_);
}