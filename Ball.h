#pragma once

class Player;
class CPU;

class Ball {
public:
	Ball();
	Ball(float setx, float sety);
	~Ball();

	float Distance(Vector2 arg1, Vector2 arg2);

	//リセット
	void Reset();
	//準備
	void Preparation();
	//発射
	void Firing();
	//移動
	bool Move(Paddle& pPad, Paddle& ePad);

	//速度変更
	void SpeedUp();

	//当たり判定
	state HitJudge(Paddle& pPad, Paddle& ePad);

	void Show();

	bool Update(Player* player, CPU* cpu);

	int Get_m_angle();

	Vector2 Get_m_pos();

	void Set_m_direction(int dir) {
		m_direction = dir;
	}

private:
	Vector2 m_pos;
	float m_speed;//速度
	int m_angle;//反時計回りを正とする
	int m_boundNum = 0;//発射からの跳ね返り回数(パドルのみ)

	int prevTime = 0;

	int m_direction = 0;//向き(x軸)　-1:プレイヤー、1:CPU

	//以下反射判定で使用
	float fsin[360] = { 0 }, fcos[360] = { 0 };
	float separator[4] = { 0 };
};
