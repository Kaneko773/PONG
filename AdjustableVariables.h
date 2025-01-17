#pragma once

const int ScreenHeight = 480;
const int ScreenWidth = 640;

const float FrameRate = 60;//�Œ�t���[�����[�g�p

const int EndingScore = 5;//�Q�[���I�����_

//�p�h���ɂ���
//���_��Y���W���������[
const int PaddleLength = 48;//�p�h���̒���
const int PaddleaPosX = 15;//�p�h���̉�ʒ[����̋���
const int PaddleInitlPos = (ScreenHeight / 2) - (PaddleLength / 2);//�p�h���̏����ʒu(Y���W)
const float PaddleSpeed = 480;//�p�h���̑��x(�s�N�Z��/�b)

//�{�[���ɂ���
const int BallSize = 5;//�{�[���̑傫��(���a) 
const float InitialVelocity = 360;//�{�[���̏����x(�s�N�Z��/�b)
const int SpeedUpMultiple = 4;//���˕Ԃ�񐔂����̐��l�̔{���ɂȂ�x�A���x�㏸
const float SpeedUpMagnification = 1.2;//���x�㏸�̔{��
const float MAX_BALL_SPEED = 100;//�ő呬�x100
const int PreparationTime = 2000;//���˂܂�(����)�̎���