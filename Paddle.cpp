#include "DxLib.h"
#include "common.h"
#include "AdjustableVariables.h"
#include "Paddle.h"

Paddle::Paddle() {
	
}

Paddle::Paddle(float setx, float sety) {
	m_pos.x = setx;
	m_pos.y = sety;
}

Paddle::~Paddle() {

}

void Paddle::Move(float value) {
	m_pos.y += value;
	//‰æ–Ê“à‚Éƒpƒhƒ‹‚ðŽû‚ß‚é
	if (m_pos.y < 0) m_pos.y = 0;
	if (m_pos.y + PaddleLength > ScreenHeight - 1) m_pos.y = ScreenHeight - PaddleLength - 1;
}

void Paddle::Show() const {
	DrawLine(m_pos.x, m_pos.y, m_pos.x, m_pos.y + PaddleLength + 1, GetColor(255, 255, 255));
}

Vector2 Paddle::Get_m_pos() {
	return m_pos;
}