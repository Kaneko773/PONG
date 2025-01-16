#pragma once

class Player {
public:
	Player();
	Player(float setx, float sety);
	~Player();
	void Paddle_Move();

	int Get_m_score() const {
		return m_score;
	}
	void Add_m_score() {
		++m_score;
	}

	Paddle* Get_m_paddle() const {
		return m_paddle;
	}

private:
	Paddle* m_paddle;
	int m_score = 0;
};
