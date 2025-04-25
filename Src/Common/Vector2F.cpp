#include <DxLib.h>		
#include "Vector2F.h"		

//コンストラクタ
Vector2F::Vector2F(void)
{
	x = 0.0f;
	y = 0.0f;
}

// コンストラクタ
Vector2F::Vector2F(float vx, float vy)
{
	x = vx;
	y = vy;
}

//デストラクタ
Vector2F::~Vector2F(void)
{
}