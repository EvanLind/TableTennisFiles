#ifndef __CALPARA_H__
#define __CALPARA_H__
#include "stdafx.h"
#define CALIB 1//
//#define CalDis 1
#ifdef CALIB
//#define BOOL BOOL
#define CHESS_WIDTH 9       //
#define CHESS_HIGHT 9
#define WIDTH_PIX      1280     //
#define HIGHT_PIX      1024
#define SQUAR_HIGHT 97.5
#define SQUAR_WIDTH 98
#define NUM_IMAGE 17       //
#define CORNER_NUML 6//在场地上找到的用于标定的点数目
#define CORNER_NUMR 6//在场地上找到的用于标定的点数目

#endif
void CalFromUndistortImgPoint(CvMat * image_matrix, CvMat* intrinsic, CvMat* distortion);
float CalUV2XYZ(float u1, float v1, float u2, float v2, CvPoint3D32f *xyz, CvPoint3D32f *xyzNew);
void CalExtrPara(float *object_pointL, float *image_point1, float *object_pointR, float *image_point2);
int calib(char L_Or_R);
void PrintMat(CvMat *matrix, BOOL save_or_show, FILE *fp);
int calcRobotMove();
#endif