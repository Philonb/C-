#ifndef __GAME_H__
#define __GAME_H__
/****************************************************
 此种方法用于实现c调用cpp函数                       */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
	/*函数功能:游戏平抛小球的本体
  函数返回值:无
  函数入口参数:无*/
	extern void BallFlatthrow();
	extern void JumpBall();
	extern void RotateSnake();
	extern void MovingRectangle();
	extern void ThrowPins();
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
/******************************************************/
#endif // !__GAME_H__
