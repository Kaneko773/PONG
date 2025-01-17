#pragma once

const int ScreenHeight = 480;
const int ScreenWidth = 640;

const float FrameRate = 60;//固定フレームレート用

const int EndingScore = 5;//ゲーム終了得点

//パドルについて
//原点はY座標が小さい端
const int PaddleLength = 48;//パドルの長さ
const int PaddleaPosX = 15;//パドルの画面端からの距離
const int PaddleInitlPos = (ScreenHeight / 2) - (PaddleLength / 2);//パドルの初期位置(Y座標)
const float PaddleSpeed = 480;//パドルの速度(ピクセル/秒)

//ボールについて
const int BallSize = 5;//ボールの大きさ(半径) 
const float InitialVelocity = 360;//ボールの初速度(ピクセル/秒)
const int SpeedUpMultiple = 4;//跳ね返り回数がこの数値の倍数になる度、速度上昇
const float SpeedUpMagnification = 1.2;//速度上昇の倍率
const float MAX_BALL_SPEED = 100;//最大速度100
const int PreparationTime = 2000;//発射まで(準備)の時間