#pragma once

class Grid
{
public:
	// コンストラクタ
	Grid(void);

	// デストラクタ
	~Grid(void);

	void SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	// 線の長さ
	static constexpr float LINE_LEN = 1200.0f;

	// 線の長さの半分
	static constexpr float H_LINE_LEN = LINE_LEN / 2.0f;

	// 線の間隔
	static constexpr float TERM = 50.0f;

	// 球体の半径
	static constexpr float SPHERE_RADIUS = 20.0f;

	// 球体の半径
	static const int LINE_NUM = static_cast<int>(LINE_LEN / TERM);

	// 線の数の半分
	static const int H_LINE_NUM = LINE_NUM / 2;

	// 球体を形成するポリゴンの細かさ
	static const int SPHERE_DIV_NUM = 10;
};