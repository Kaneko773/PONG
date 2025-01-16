#pragma once

class Paddle;
class Ball;

class CPU {
public:
	CPU();
	CPU(float setx, float sety);
	~CPU();
	void Paddle_Move(Ball& ball);

	int Get_m_score() const {
		return m_score;
	}
	void Add_m_score() {
		++m_score;
	}

	Paddle* Get_m_paddle() const {
		return m_paddle;
	}

	void Search(Ball& ball);
	void Reset_Search() {
		toInvestigate = true;
		m_ballPredictedPosY = (float)ScreenHeight / 2;
	}

private:
	Paddle* m_paddle;
	int m_score = 0;
	float m_ballPredictedPosY = -99;//ボールを反射する予測位置(Y座標) 予測位置未設定時は-99を入れとく
	bool toInvestigate = true;
};