#pragma once

class Paddle {
public:
	Paddle();
	Paddle(float setx, float sety);
	~Paddle();

	//ˆÚ“®
	void Move(float value);

	void Show() const;

	Vector2 Get_m_pos();

private:
	Vector2 m_pos;
};
