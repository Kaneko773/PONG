#include "DxLib.h"
#include "common.h"
#include "AdjustableVariables.h"

#include "Paddle.h"
#include "CPU.h"
#include "Ball.h"
#include "Player.h"

//画面640*480

//弾は最初はランダム、以降は点を取られた方に発射される

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(TRUE);
	// ScreenFlip を実行しても垂直同期信号を待たない
	SetWaitVSyncFlag(FALSE);
	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0) return -1;
	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	unsigned int CrWhite = GetColor(255, 255, 255);//画面表示用の色
	//スコア用
	int FontHandle_score = CreateFontToHandle(NULL, 120, 3);
	unsigned int Cr_score = GetColor(0, 255, 255);

	int FrameStartTime = GetNowCount();        // ６０ＦＰＳ固定用、時間保存用変数

	Player* player = nullptr;
	CPU* cpu = nullptr;
	Ball* ball = nullptr;
	
	player = new Player(PaddleaPosX, PaddleInitlPos);
	cpu = new CPU(ScreenWidth - 1 - PaddleaPosX, PaddleInitlPos);
	ball = new Ball(319, BallSize + 1);//発射位置は後で

	ball->Reset();
	ball->Set_m_direction(-1 + (GetRand(1) * 2));

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();//画面クリア

		//フレームレート固定
		// １/６０秒立つまで待つ
		while (GetNowCount() - FrameStartTime < 1000 / FrameRate) {}
		// 現在のカウント値を保存
		FrameStartTime = GetNowCount();

		//パドルの移動(プレイヤー)
		player->Paddle_Move();
		//敵のバドル//デバッグ
		cpu->Paddle_Move(*ball);
		//ボールの移動
		if (!ball->Update(player, cpu)) { break; }

		//Show
		{
			//中央線
			for (int i = 0; i < 24; ++i) {
				DrawLine(ScreenWidth / 2 - 1, i * 20, ScreenWidth / 2 - 1, i * 20 + 12, CrWhite);
			}

			//得点
			DrawFormatStringToHandle(129, 0, Cr_score, FontHandle_score, "%d", player->Get_m_score());
			DrawFormatStringToHandle(449, 0, Cr_score, FontHandle_score, "%d", cpu->Get_m_score());

			//ボール
			ball->Show();

			//パドル
			player->Get_m_paddle()->Show();
			cpu->Get_m_paddle()->Show();
		}

		ScreenFlip();//表示

		if (player->Get_m_score() >= EndingScore || cpu->Get_m_score() >= EndingScore) {
			break;
		}
	}

	if (player) {
		delete player;
		player = 0;
	}
	if (cpu) {
		delete cpu;
		cpu = 0;
	}
	if (ball) {
		delete ball;
		ball = 0;
	}

	DeleteFontToHandle(FontHandle_score);

	// ＤＸライブラリ使用の終了処理
	DxLib_End();
	// ソフトの終了
	return 0; 
}