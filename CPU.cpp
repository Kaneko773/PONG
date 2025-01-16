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
	//�{�[�����v���C���[���ɍs�����烊�Z�b�g
	if (!toInvestigate && ball.Get_m_pos().x < ScreenWidth / 2) {
		Reset_Search();
	}

	//�{�[�����������z������v�Z�J�n
	//�{�[����x���W==�p�h����x���W �̎���y���W���v�Z���ړ�
	if (toInvestigate && ball.Get_m_pos().x >= (float)ScreenWidth / 2) {
		Search(ball);
	}
	/*if (toInvestigate && m_ballPredictedPosY == -99 && ball.Get_m_pos().x >= ScreenWidth / 2) {
		Search(ball);
	}*/

	//�\���ʒu�܂ňړ�
	if (m_ballPredictedPosY != -99) {
		Vector2 pastPaddlePos(m_paddle->Get_m_pos().x, m_paddle->Get_m_pos().y);

		//�\���ʒu�ɒ����܂ňړ�
		if (pastPaddlePos.y + (float)PaddleLength / 2 > m_ballPredictedPosY) {//��ړ�
			m_paddle->Move(-1 * PaddleSpeed / FrameRate);

			if ((m_paddle->Get_m_pos().y + (float)PaddleLength / 2) - m_ballPredictedPosY <= 0) { //��~
				m_ballPredictedPosY = -99;
			}
		}
		else if (pastPaddlePos.y + (float)PaddleLength / 2 < m_ballPredictedPosY) {//���ړ�
			m_paddle->Move(PaddleSpeed / FrameRate);

			if ((m_paddle->Get_m_pos().y + (float)PaddleLength / 2) - m_ballPredictedPosY >= 0) {//��~
				m_ballPredictedPosY = -99;
			}
		}

		if (m_paddle->Get_m_pos().y == 0 || m_paddle->Get_m_pos().y == ScreenHeight - PaddleLength - 1) {//�[�܂Œ��������~
			m_ballPredictedPosY = -99;
		}
	}
}

void CPU::Search(Ball& ball) {
	Vector2 tempBallPos(ball.Get_m_pos().x, ball.Get_m_pos().y);//�v�Z�p
	int tempBallAngle = ball.Get_m_angle();//�v�Z�p

	while (true)
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0)return;//���[�v��������p

#if 0
		float a = (float)tan((360 - tempBallAngle) * 3.1415926535 / 180);
		if (a == 0) { g_error = 1; return; }/*�G���[*///���!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		float b = tempBallPos.y - a * tempBallPos.x; //y=ax+b �� b=y-ax

		float y = a * m_paddle->Get_m_pos().x + b;//�p�h���̈ʒu�ł�y���W�����߂�



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
		else {//�\���ʒu�ݒ�
			m_ballPredictedPosY = y;
			toInvestigate = false;
			return;
		}
#else
		if(tempBallAngle == 90 || tempBallAngle == 270) {//�G���[
			g_error = 16;
			DrawFormatString(0, 0, GetColor(255, 255, 255), "�[���ȃG���[�A���Ƃ��I �G���[%d", g_error);
			ScreenFlip();//�\��
			WaitTimer(5000);
			return;
		}
		float a = (float)tan(tempBallAngle * 3.1415926535 / 180);
		a *= -1;
		float b = tempBallPos.y - a * tempBallPos.x; //y=ax+b �� b=y-ax

		float y = a * m_paddle->Get_m_pos().x + b;//�p�h���̈ʒu�ł�y���W�����߂�


		if (y < 0) {
			if (a == 0 || a == 180) {//�G���[
				g_error = 14;
				DrawFormatString(0, 0, GetColor(255, 255, 255), "�[���ȃG���[�A���Ƃ��I �G���[%d", g_error);
				ScreenFlip();//�\��
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
			if (a == 0 || a == 180) {//�G���[
				DrawFormatString(0, 0, GetColor(255, 255, 255), "�[���ȃG���[�A���Ƃ��I �G���[%d", g_error);
				ScreenFlip();//�\��
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
		else {//�\���ʒu�ݒ�
			m_ballPredictedPosY = y;
			toInvestigate = false;
			return;
		}
#endif
	}
}