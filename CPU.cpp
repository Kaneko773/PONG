#include <math.h>
#include "DxLib.h"
#include "common.h"
#include "AdjustableVariables.h"
#include "Paddle.h"
#include "CPU.h"
#include "Ball.h"

extern int g_error;/////////////////////

CPU::CPU() {
	
}
CPU::CPU(float setx, float sety) {
	m_paddle = new Paddle(setx, sety);
}
CPU::~CPU() {
	if (m_paddle) {
		delete m_paddle;
		m_paddle = 0;
	}
}
void CPU::Paddle_Move(Ball& ball) {
	//ボールがプレイヤー側に行ったらリセット
	if (!toInvestigate && ball.Get_m_pos().x < ScreenWidth / 2) {
		Reset_Search();
	}

	//ボールが中央を越えたら計算開始
	//ボールのx座標==パドルのx座標 の時のy座標を計算し移動
	if (toInvestigate && ball.Get_m_pos().x >= (float)ScreenWidth / 2) {
		Search(ball);
	}
	/*if (toInvestigate && m_ballPredictedPosY == -99 && ball.Get_m_pos().x >= ScreenWidth / 2) {
		Search(ball);
	}*/

	//予測位置まで移動
	if (m_ballPredictedPosY != -99) {
		Vector2 pastPaddlePos(m_paddle->Get_m_pos().x, m_paddle->Get_m_pos().y);

		//予測位置に着くまで移動
		if (pastPaddlePos.y + (float)PaddleLength / 2 > m_ballPredictedPosY) {//上移動
			m_paddle->Move(-1 * PaddleSpeed / FrameRate);

			if ((m_paddle->Get_m_pos().y + (float)PaddleLength / 2) - m_ballPredictedPosY <= 0) { //停止
				m_ballPredictedPosY = -99;
			}
		}
		else if (pastPaddlePos.y + (float)PaddleLength / 2 < m_ballPredictedPosY) {//下移動
			m_paddle->Move(PaddleSpeed / FrameRate);

			if ((m_paddle->Get_m_pos().y + (float)PaddleLength / 2) - m_ballPredictedPosY >= 0) {//停止
				m_ballPredictedPosY = -99;
			}
		}

		if (m_paddle->Get_m_pos().y == 0 || m_paddle->Get_m_pos().y == ScreenHeight - PaddleLength - 1) {//端まで着いたら停止
			m_ballPredictedPosY = -99;
		}
	}
}

void CPU::Search(Ball& ball) {
	Vector2 tempBallPos(ball.Get_m_pos().x, ball.Get_m_pos().y);//計算用
	int tempBallAngle = ball.Get_m_angle();//計算用

	while (true)
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)return;//ループ時抜ける用

#if 0
		float a = (float)tan((360 - tempBallAngle) * 3.1415926535 / 180);
		if (a == 0) { g_error = 1; return; }/*エラー*///後で!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		float b = tempBallPos.y - a * tempBallPos.x; //y=ax+b → b=y-ax

		float y = a * m_paddle->Get_m_pos().x + b;//パドルの位置でのy座標を求める



		if (y < 0) {
			b -= BallSize;
			float x = (0 - b) / a;
			tempBallPos.x = x;
			tempBallPos.y = BallSize;
			tempBallAngle = 360 - tempBallAngle;

		}
		else if (ScreenHeight - 1 < y) {
			b += BallSize;
			float x = ((ScreenWidth - 1) - b) / a;
			tempBallPos.x = x;
			tempBallPos.y = -1 * BallSize;
			tempBallAngle = 360 - tempBallAngle;
		}
		else {//予測位置設定
			m_ballPredictedPosY = y;
			toInvestigate = false;
			return;
		}
#else
		if(tempBallAngle == 90 || tempBallAngle == 270) {//エラー
			g_error = 16;
			DrawFormatString(0, 0, GetColor(255, 255, 255), "深刻なエラー、落とせ！ エラー%d", g_error);
			ScreenFlip();//表示
			WaitTimer(5000);
			return;
		}
		float a = (float)tan(tempBallAngle * 3.1415926535 / 180);
		a *= -1;
		float b = tempBallPos.y - a * tempBallPos.x; //y=ax+b → b=y-ax

		float y = a * m_paddle->Get_m_pos().x + b;//パドルの位置でのy座標を求める


		if (y < 0) {
			if (a == 0 || a == 180) {//エラー
				g_error = 14;
				DrawFormatString(0, 0, GetColor(255, 255, 255), "深刻なエラー、落とせ！ エラー%d", g_error);
				ScreenFlip();//表示
				WaitTimer(5000);
				return;
			}
			b -= BallSize;
			float x = (0 - b) / a;
			tempBallPos.x = x;
			tempBallPos.y = BallSize;
			tempBallAngle = 360 - tempBallAngle;
		}
		else if (ScreenHeight - 1 < y) {
			if (a == 0 || a == 180) {//エラー
				DrawFormatString(0, 0, GetColor(255, 255, 255), "深刻なエラー、落とせ！ エラー%d", g_error);
				ScreenFlip();//表示
				WaitTimer(5000);
				g_error = 15;
				return;
			}
			b += BallSize;
			float x = ((ScreenHeight - 1) - b) / a;
			tempBallPos.x = x;
			tempBallPos.y = (ScreenHeight - 1) - BallSize;
			tempBallAngle = 360 - tempBallAngle;
		}
		else {//予測位置設定
			m_ballPredictedPosY = y;
			toInvestigate = false;
			return;
		}
#endif
	}
}