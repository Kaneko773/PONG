#pragma once

class Player;
class CPU;

class Ball {
public:
	Ball();
	Ball(float setx, float sety);
	~Ball();

	float Distance(Vector2 arg1, Vector2 arg2);

	//���Z�b�g
	void Reset();
	//����
	void Preparation();
	//����
	void Firing();
	//�ړ�
	bool Move(Paddle& pPad, Paddle& ePad);

	//���x�ύX
	void SpeedUp();

	//�����蔻��
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
	float m_speed;//���x
	int m_angle;//�����v���𐳂Ƃ���
	int m_boundNum = 0;//���˂���̒��˕Ԃ��(�p�h���̂�)

	int prevTime = 0;

	int m_direction = 0;//����(x��)�@-1:�v���C���[�A1:CPU

	//�ȉ����˔���Ŏg�p
	float fsin[360] = { 0 }, fcos[360] = { 0 };
	float separator[4] = { 0 };
};
