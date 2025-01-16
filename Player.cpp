#include "DxLib.h"
#include "common.h"
#include "Paddle.h"
#include "Player.h"
#include "AdjustableVariables.h"

Player::Player() {
	
}

Player::Player(float setx, float sety) {
	m_paddle = new Paddle(setx, sety);
}

Player::~Player() {
	if (m_paddle) {
		delete m_paddle;
		m_paddle = 0;
	}
}
void Player::Paddle_Move() {
	if (CheckHitKey(KEY_INPUT_W) == 1) m_paddle->Move(-1 * PaddleSpeed / FrameRate);
	if (CheckHitKey(KEY_INPUT_S) == 1) m_paddle->Move(PaddleSpeed / FrameRate);
}