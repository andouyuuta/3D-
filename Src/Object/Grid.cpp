#include <DxLib.h>
#include "Grid.h"

// コンストラクタ
Grid::Grid(void)
{
}

// コンストラクタ
Grid::~Grid(void)
{
}

// 初期化
void Grid::SystemInit(void)
{
}

// 初期化
void Grid::GameInit(void)
{
}

// 更新
void Grid::Update(void)
{
}

void Grid::Draw(void)
{
	// 線座標
	VECTOR sPos = { 0.0f, 0.0f, 0.0f };	// 開始座標
	VECTOR ePos = { 0.0f, 0.0f, 0.0f };	// 終了座標

	// グリッド線を引く
	for (int ii = -H_LINE_NUM; ii < H_LINE_NUM; ++ii)
	{
		// 線の番号
		float lineNumber = static_cast<float>(ii);

		// X軸(赤)→座標設定→線描画→球描画
		sPos = { -H_LINE_LEN, 0.0f, lineNumber * TERM };
		ePos = { H_LINE_LEN, 0.0f, lineNumber * TERM };
		DrawLine3D(sPos, ePos, 0xff0000);
		DrawSphere3D(ePos, SPHERE_RADIUS, SPHERE_DIV_NUM, 0xff0000, 0xff0000, true);

		// Z軸(青)→座標設定→線描画→球描画
		sPos = { lineNumber * TERM, 0.0f, -H_LINE_LEN };
		ePos = { lineNumber * TERM, 0.0f, H_LINE_LEN };
		DrawLine3D(sPos, ePos, 0x0000ff);
		DrawSphere3D(ePos, SPHERE_RADIUS, SPHERE_DIV_NUM, 0x0000ff, 0x0000ff, true);
	}

	// Y軸(緑)(一本)→座標設定→線描画→球描画
	sPos = { 0.0f, -H_LINE_LEN, 0.0f };
	ePos = { 0.0f, H_LINE_LEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00ff00);
	DrawSphere3D(ePos, SPHERE_RADIUS, SPHERE_DIV_NUM, 0x00ff00, 0x00ff00, true);
}

void Grid::Release(void)
{
}