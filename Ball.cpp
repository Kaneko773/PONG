#include <math.h>
#include "DxLib.h"
#include "common.h"
#include "AdjustableVariables.h"
#include "Paddle.h"
#include "Ball.h"
#include "CPU.h"
#include "Player.h"

Ball::Ball() {
	
}

Ball::Ball(float setx, float sety) {
	m_pos.x = setx;
	m_pos.y = sety;
	m_speed = 0;
	m_angle = 0;
	m_boundNum = 0;

	for (int i = 0; i < 360; i++) {
		fsin[i] = (float)sin(i * 3.1415926535 / 180);
		fcos[i] = (float)cos(i * 3.1415926535 / 180);
	}

	for (int i = 0; i < 4; ++i) {
		separator[i] = PaddleLength / 5 * (i + 1);
	}
}

Ball::~Ball() {

}

float Ball::Distance(Vector2 arg1, Vector2 arg2) {
	return (arg1.x - arg2.x) * (arg1.x - arg2.x) + (arg1.y - arg2.y) * (arg1.y - arg2.y);
}

void Ball::Reset() {
	Vector2 temp(319, 239);
	m_pos = temp;
	m_speed = 0;
	m_boundNum = 0;
	prevTime = GetNowCount();
}
void Ball::Preparation() {
	if (GetNowCount() - prevTime >= PreparationTime) {
		Firing();
		prevTime = GetNowCount();
	}
}
//発射
void Ball::Firing() {
	m_speed = 1;
	m_boundNum = 0;
	int tempAddAngle = m_direction == -1 ? 180 : 0;
	m_angle = (-1 + (GetRand(1) * 2)) * (GetRand(69) + 1) + tempAddAngle;
	//角度を0→359に収める
	if (m_angle < 0) m_angle += 360;
	if (m_angle > 359) m_angle -= 360;
}
//移動
bool Ball::Move(Paddle& pPad, Paddle& ePad) {
	//移動前の場所を保存
	Vector2 pastPos = m_pos;

	float pixelPerFR = (InitialVelocity / FrameRate) * m_speed;//１フレームで進む距離

	m_pos.x += pixelPerFR * fcos[m_angle];
	m_pos.y -= pixelPerFR * fsin[m_angle];

	//当たり判定すり抜け対策

	//以下の式について
		//y=ax+b
		//b=-ap+q(p,qは線分上の点)
		//x軸にr、y軸にsだけ平行移動するとき
		//y=(x+r-p)a+q+s
		//x=(y-q-s)/a+p+r

	
	//上下の貫通対策
	if (m_pos.y - BallSize < 0) {//上 m_pos.y - BallSizeを０にする
		if (m_pos.x == pastPos.x)/*エラー*/ { return false; }
		float a = (m_pos.y - pastPos.y) / (m_pos.x - pastPos.x);
		if (a == 0)/*エラー*/ { return false; }
		float tempX = (0 - pastPos.y - (-1 * BallSize)) / a + pastPos.x + 0;//上限(y座標)=ボールの上部(y座標) の時のX座標

		m_pos.x = tempX;
		m_pos.y = BallSize;
	}
	if (m_pos.y + BallSize > ScreenHeight - 1) {//下
		if (m_pos.x == pastPos.x)/*エラー*/ { return false; }
		float a = (m_pos.y - pastPos.y) / (m_pos.x - pastPos.x);
		if (a == 0)/*エラー*/ { return false; }
		float tempX = ((ScreenHeight - 1) - pastPos.y - BallSize) / a + pastPos.x + 0;//下限(y座標)=ボールの下部(y座標) の時のX座標

		m_pos.x = tempX;
		m_pos.y = ScreenHeight - 1 - BallSize;
	}

	//プレイヤーのパドル
	if (m_pos.x <= PaddleaPosX) {

		if (m_pos.x == pastPos.x)/*エラー*/ { return false; }
		float a = (m_pos.y - pastPos.y) / (m_pos.x - pastPos.x);
		float tempY = (pPad.Get_m_pos().x + 0 - pastPos.x) * a + pastPos.y + 0;//パドルの位置(x座標)=ボールの中心(x座標) の時のY座標

		if (tempY + BallSize > pPad.Get_m_pos().y && pPad.Get_m_pos().y + PaddleLength > tempY - BallSize) {
			m_pos.x = PaddleaPosX;
			m_pos.y = tempY;
		}
	}
	//敵のパドル
	else if (ScreenWidth - 1 - PaddleaPosX <= m_pos.x) {
		if (m_pos.x == pastPos.x)/*エラー*/ { return false; }
		float a = (m_pos.y - pastPos.y) / (m_pos.x - pastPos.x);
		float tempY = (ePad.Get_m_pos().x + 0 - pastPos.x) * a + pastPos.y + 0;//パドルの位置(x座標)=ボールの中心(x座標) の時のY座標

		if (tempY + BallSize > ePad.Get_m_pos().y && ePad.Get_m_pos().y + PaddleLength > tempY - BallSize) {
			m_pos.x = ScreenWidth - 1 - PaddleaPosX;
			m_pos.y = tempY;
		}
	}

	return true;
}
//速度変更
void Ball::SpeedUp() {
	if (m_boundNum % SpeedUpMultiple == 0)m_speed *= SpeedUpMagnification;
	if (m_speed > MAX_BALL_SPEED) m_speed = MAX_BALL_SPEED;
}

//当たり判定
state Ball::HitJudge(Paddle& pPad, Paddle& ePad) {

	if (m_speed == 0) return preparation;

	bool bound = false;

	//ゴール
	if (m_pos.x - BallSize <= 0) {
		return state::cpuPoint;//敵の得点
	}
	if (m_pos.x + BallSize >= ScreenWidth - 1) {
		return state::playerPoint;//プレイヤーの得点
	}

	//プレイヤーのパドル
	if (m_pos.x - BallSize <= pPad.Get_m_pos().x) {
		if (m_pos.y + BallSize < pPad.Get_m_pos().y || pPad.Get_m_pos().y + PaddleLength < m_pos.y - BallSize) return move;

		//後で（ボールの後ろにパドルが当たった時）
		if (m_pos.x < pPad.Get_m_pos().x) {

		}
		//跳ね返し処理
		else {

			//ボールとパドルが触れている部分を調べる
			int hitCenter = -1;//触れてる部分の中心（バドルの原点からの距離）
			float BallSizeSquared = BallSize * BallSize;
			//パドルの全ピクセルでボールに触れてるか調べる
			for (int i = 0; i < PaddleLength; ++i) {
				Vector2 temp(pPad.Get_m_pos().x, pPad.Get_m_pos().y + i);//演算子オーバーロードはいつかやる
				if (Distance(temp, m_pos) <= BallSizeSquared) {
					if (hitCenter == -1) hitCenter = i;

					if (hitCenter != -1) {
						hitCenter += (i - hitCenter) / 2;
					}
				}
			}

			//触れた場所によって跳ね返り方向を変える
			//パドルを５等分する
			//両端　来た角度からx軸に向けて+30度ぐらい
			//２，４　反射する角度からx軸の間
			//真ん中　普通に反射

			//真ん中
			if (separator[1] <= hitCenter && hitCenter <= separator[2]) {
				if (m_angle == 180) m_angle = (GetRand(9) + 1) * (1 - GetRand(2));
				else if (90 < m_angle && m_angle < 180) m_angle = 180 - m_angle;
				else if (180 < m_angle && m_angle < 270) m_angle = 540 - m_angle;
			}
			//両端
			else if ((0 <= hitCenter && hitCenter < this->separator[0]) || (separator[3] < hitCenter && hitCenter <= PaddleLength)) {
				if (m_angle == 180) m_angle = (GetRand(29) + 1) * (1 - GetRand(2));
				else if (90 < m_angle && m_angle < 180) m_angle += 180 + GetRand(30);
				else if (180 < m_angle && m_angle < 270) m_angle -= 180 + GetRand(30);
			}
			//２，４
			else {
				if (m_angle == 180) m_angle = (GetRand(19) + 1) * (1 - GetRand(2));
				else if (90 < m_angle && m_angle < 180) m_angle = GetRand(180 - m_angle);
				else if (180 < m_angle && m_angle < 270) m_angle = 540 - m_angle + GetRand((180 - m_angle) * -1);
			}
			//角度を0→359に収める
			if (m_angle < 0) m_angle += 360;
			if (m_angle > 359) m_angle -= 360;

			++m_boundNum;
			SpeedUp();

			bound = true;
		}
	}
	//敵のパドル
	if (m_pos.x + BallSize >= ePad.Get_m_pos().x) {
		if (m_pos.y + BallSize < ePad.Get_m_pos().y || ePad.Get_m_pos().y + PaddleLength < m_pos.y - BallSize) return move;

		//後で（ボールの後ろにパドルが当たった時）
		if (m_pos.x > ePad.Get_m_pos().x) {

		}
		//跳ね返し処理
		else {

			//ボールとパドルが触れている部分を調べる
			int hitCenter = -1;//触れてる部分の中心（バドルの原点からの距離）
			float BallSizeSquared = BallSize * BallSize;
			//パドルの全ピクセルでボールに触れてるか調べる
			for (int i = 0; i < PaddleLength; ++i) {
				Vector2 temp(ePad.Get_m_pos().x, ePad.Get_m_pos().y + i);//演算子オーバーロードはいつかやる
				if (Distance(temp, m_pos) <= BallSizeSquared) {
					
					if (hitCenter == -1) hitCenter = i;

					if (hitCenter != -1) {
						hitCenter += (i - hitCenter) / 2;
					}
				}
			}

			//触れた場所によって跳ね返り方向を変える
			//パドルを５等分する
			//両端　来た角度からx軸に向けて+30度ぐらい
			//２，４　反射する角度からx軸の間
			//真ん中　普通に反射

			//真ん中
			if (separator[1] <= hitCenter && hitCenter <= separator[2]) {
				if (m_angle == 0) m_angle = 180 + (GetRand(9) + 1) * (1 - GetRand(2));
				else if (0 < m_angle && m_angle < 90) m_angle = 180 - m_angle;
				else if (270 < m_angle && m_angle < 360) m_angle = 540 - m_angle;
			}
			//両端
			else if ((0 <= hitCenter && hitCenter < separator[0]) || (separator[3] < hitCenter && hitCenter <= PaddleLength)) {
				if (m_angle == 0) m_angle = 180 + (GetRand(29) + 1) * (1 - GetRand(2));
				else if (0 < m_angle && m_angle < 90) m_angle += 180 - GetRand(30);
				else if (270 < m_angle && m_angle < 360) m_angle -= 180 - GetRand(30);
			}
			//２，４
			else {
				if (m_angle == 0) m_angle = 180 + (GetRand(19) + 1) * (1 - GetRand(2));
				else if (0 < m_angle && m_angle < 90) m_angle = 180 - m_angle + GetRand(m_angle);
				else if (270 < m_angle && m_angle < 360) m_angle = 540 - m_angle - GetRand(360 - m_angle);
			}
			//角度を0→359に収める
			if (m_angle < 0) m_angle += 360;
			if (m_angle > 359) m_angle -= 360;

			++m_boundNum;
			SpeedUp();

			bound = true;
		}
	}
	//上限
	if (m_pos.y - BallSize <= 0) {
		if (m_angle == 90) { /*エラー*/ return error; }
		if (0 < m_angle && m_angle < 90) m_angle = 360 - m_angle;
		if (90 < m_angle && m_angle < 180) m_angle = 360 - m_angle;

		bound = true;
	}
	//下限
	if (m_pos.y + BallSize >= ScreenHeight - 1) {
		if (m_angle == 270) { /*エラー*/ return error; }
		if (180 < m_angle && m_angle < 270) m_angle = 360 - m_angle;
		if (270 < m_angle && m_angle < 360) m_angle = 360 - m_angle;

		bound = true;
	}

	if (bound)return state::bound;
	return state::move;
}

void Ball::Show() {
	DrawCircle(m_pos.x, m_pos.y, BallSize, GetColor(255, 255, 255), true);
}

bool Ball::Update(Player* player, CPU* cpu) {
	if (!Move(*player->Get_m_paddle(), *cpu->Get_m_paddle())) {
		return false;
		//エラー
	}
	switch (HitJudge(*player->Get_m_paddle(), *cpu->Get_m_paddle()))
	{
	case error:
		//エラー
		return false;
		break;
	case move:
	case bound:
		//何もなし
		break;
	case playerPoint:
		player->Add_m_score();
		Set_m_direction(1);
		Reset();
		cpu->Reset_Search();
		break;
	case cpuPoint:
		cpu->Add_m_score();
		Set_m_direction(-1);
		Reset();
		cpu->Reset_Search();
		break;
	case preparation:
		Preparation();
		break;
	}
	return true;
}

int Ball::Get_m_angle() {
	return m_angle;
}

Vector2 Ball::Get_m_pos() {
	return m_pos;
}