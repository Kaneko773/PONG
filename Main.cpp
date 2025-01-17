#include "DxLib.h"
#include "common.h"
#include "AdjustableVariables.h"

#include "Paddle.h"
#include "CPU.h"
#include "Ball.h"
#include "Player.h"

//���640*480

//�e�͍ŏ��̓����_���A�ȍ~�͓_�����ꂽ���ɔ��˂����

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);
	// ScreenFlip �����s���Ă����������M����҂��Ȃ�
	SetWaitVSyncFlag(FALSE);
	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0) return -1;
	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	unsigned int CrWhite = GetColor(255, 255, 255);//��ʕ\���p�̐F
	//�X�R�A�p
	int FontHandle_score = CreateFontToHandle(NULL, 120, 3);
	unsigned int Cr_score = GetColor(0, 255, 255);

	int FrameStartTime = GetNowCount();        // �U�O�e�o�r�Œ�p�A���ԕۑ��p�ϐ�

	Player* player = nullptr;
	CPU* cpu = nullptr;
	Ball* ball = nullptr;
	
	player = new Player(PaddleaPosX, PaddleInitlPos);
	cpu = new CPU(ScreenWidth - 1 - PaddleaPosX, PaddleInitlPos);
	ball = new Ball(319, BallSize + 1);//���ˈʒu�͌��

	ball->Reset();
	ball->Set_m_direction(-1 + (GetRand(1) * 2));

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();//��ʃN���A

		//�t���[�����[�g�Œ�
		// �P/�U�O�b���܂ő҂�
		while (GetNowCount() - FrameStartTime < 1000 / FrameRate) {}
		// ���݂̃J�E���g�l��ۑ�
		FrameStartTime = GetNowCount();

		//�p�h���̈ړ�(�v���C���[)
		player->Paddle_Move();
		//�G�̃o�h��//�f�o�b�O
		cpu->Paddle_Move(*ball);
		//�{�[���̈ړ�
		if (!ball->Update(player, cpu)) { break; }

		//Show
		{
			//������
			for (int i = 0; i < 24; ++i) {
				DrawLine(ScreenWidth / 2 - 1, i * 20, ScreenWidth / 2 - 1, i * 20 + 12, CrWhite);
			}

			//���_
			DrawFormatStringToHandle(129, 0, Cr_score, FontHandle_score, "%d", player->Get_m_score());
			DrawFormatStringToHandle(449, 0, Cr_score, FontHandle_score, "%d", cpu->Get_m_score());

			//�{�[��
			ball->Show();

			//�p�h��
			player->Get_m_paddle()->Show();
			cpu->Get_m_paddle()->Show();
		}

		ScreenFlip();//�\��

		if (player->Get_m_score() >= EndingScore || cpu->Get_m_score() >= EndingScore) {
			break;
		}
	}

	if (player) {
		delete player;
		player = 0;
	}
	if (cpu) {
		delete cpu;
		cpu = 0;
	}
	if (ball) {
		delete ball;
		ball = 0;
	}

	DeleteFontToHandle(FontHandle_score);

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();
	// �\�t�g�̏I��
	return 0; 
}