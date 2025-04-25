#pragma once
#include <DxLib.h>
#include <string>

class Application
{
public:
	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1000;
	static constexpr int SCREEN_SIZE_Y = 800;
	static constexpr float HALF_SCREEN_SIZE_X = SCREEN_SIZE_X / 2.0f;
	static constexpr float HALF_SCREEN_SIZE_Y = SCREEN_SIZE_Y / 2.0f;

	// フレームレート
	static constexpr float FRAME_RATE = (1000 / 60);

	// データパス関連
	static const std::string PATH_MODEL;
	static const std::string PATH_IMAGE;
	static const std::string PATH_SOUND;

	// 明示的にインスタンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの解放
	void Release(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

private:
	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);

	// コピーコンストラクタも同様
	Application(const Application&);

	// デストラクタも同様
	~Application(void);

	// フレームレート
	int currentTime;
	int lastFrameTime;
	int frameCnt;
	int updateFrameRateTime;
	float frameRate;

	void CalcFrameRate();
	void DrawFrameRate();
};