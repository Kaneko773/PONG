#pragma once

const int ScreenHeight = 480;
const int ScreenWidth = 640;

const float FrameRate = 60;//�Œ�t���[�����[�g�p

const int EndingScore = 5;//�Q�[���I�����_

//�p�h���ɂ���
//���_��Y���W���������[
const int PaddleLength = 480;//�p�h���̒���48
const int PaddleaPosX = 20;//�p�h���̉�ʒ[����̋���
const int PaddleInitlPos = 240;//�p�h���̏����ʒu(Y���W)
const float PaddleSpeed = 480;//�p�h���̑��x(�s�N�Z��/�b)240

//�{�[���ɂ���
const int BallSize = 5;//�{�[���̑傫��(���a) 10
const float InitialVelocity = 360;//�{�[���̏����x(�s�N�Z��/�b)240
const int SpeedUpMultiple = 5;//���˕Ԃ�񐔂����̐��l�̔{���ɂȂ�x�A���x�㏸
const float SpeedUpMagnification = 10.1;//���x�㏸�̔{��1.1
const float MAX_BALL_SPEED = 100;//�ő呬�x
const int PreparationTime = 2000;//���˂܂�(����)�̎���