#ifndef __GAME_H__
#define __GAME_H__
/****************************************************
 此种方法用于实现c调用cpp函数                       */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
	extern void BallFlatthrow();
	extern void JumpBall();
	extern void RotateSnake();
	extern void MovingRectangle();
	extern void ThrowPins();
	//见缝插圆
	
	extern void BeatifulCircle();
	//见缝插圆
	//贪吃蛇

	extern void Snake();
	//贪吃蛇
	//十步万度

	extern void TenStep();
	//十步万度

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
/******************************************************/
#endif // !__GAME_H__
