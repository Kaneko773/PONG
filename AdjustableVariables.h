#pragma once

const int ScreenHeight = 480;
const int ScreenWidth = 640;

const float FrameRate = 60;//固定フレームレート用

const int EndingScore = 5;//ゲーム終了得点

//パドルについて
//原点はY座標が小さい端
const int PaddleLength = 480;//パドルの長さ48
const int PaddleaPosX = 20;//パドルの画面端からの距離
const int PaddleInitlPos = 240;//パドルの初期位置(Y座標)
const float PaddleSpeed = 480;//パドルの速度(ピクセル/秒)240

//ボールについて
const int BallSize = 5;//ボールの大きさ(半径) 10
const float InitialVelocity = 360;//ボールの初速度(ピクセル/秒)240
const int SpeedUpMultiple = 5;//跳ね返り回数がこの数値の倍数になる度、速度上昇
const float SpeedUpMagnification = 10.1;//速度上昇の倍率1.1
const float MAX_BALL_SPEED = 100;//最大速度
const int PreparationTime = 2000;//発射まで(準備)の時間