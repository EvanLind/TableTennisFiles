#include "stdafx.h"
//#include "calExtrPara.h"
//CvMat *mapx_L = NULL;
//CvMat *mapy_L = NULL;
//CvMat *mapy_R = NULL;
//CvMat *mapx_R = NULL;
//
//CvMat *intrinsic1 = NULL;
//CvMat *distortion1 = NULL;
//CvMat *intrinsic2 = NULL;
//CvMat *distortion2 = NULL;
//CvMat *AxMxS_matrix = NULL;
//CvMat *AxMxS_matrix_1 = NULL;
//CvMat *AxM_4 = NULL;
//CvMat *XYZResult = NULL;
//CvMat *RT_matrix1 = NULL;
//CvMat *RT_matrix2 = NULL;
//CvMat *AxRt1 = NULL;//A*M[3x4]
//CvMat *AxRt2 = NULL;
//CvMat *AxRtExtnd1 = NULL;//A*M的扩展，最后一行为0,0,0,1[4x4]
//CvMat *AxRtExtnd2 = NULL;
//
//CvMat *AxRtExtnd1_1 = NULL;//A*M的扩展的逆，最后一行为0,0,0,1[4x4]
//CvMat *AxRtExtnd2_1 = NULL;
//
//CvMat *Me2Me1_1 = NULL;//A*M的扩展，最后一行为0,0,0,1[4x4]
//CvMat *Me1Me2_1 = NULL;//A*M的扩展，最后一行为0,0,0,1[4x4]
//
//
//void InitMapxy()//计算图像点对应矫正点的映射
//{
//	mapx_L = cvCreateMat(HIGHT_PIX, WIDTH_PIX, CV_32F);
//	mapy_L = cvCreateMat(HIGHT_PIX, WIDTH_PIX, CV_32F);
//	mapy_R = cvCreateMat(HIGHT_PIX, WIDTH_PIX, CV_32F);
//	mapx_R = cvCreateMat(HIGHT_PIX, WIDTH_PIX, CV_32F);
//	cvInitUndistortMap(intrinsic1, distortion1, mapx_L, mapy_L);
//	cvInitUndistortMap(intrinsic2, distortion2, mapx_R, mapy_R);
//
//}
//int CalImgPointFromUndistortPoint(CvPoint *uv)
//{
//	for (int x = 0; x < WIDTH_PIX; x++)
//	{
//		for (int y = 0; y < HIGHT_PIX; y++)
//		{
//#ifdef IF_IS_LEFT
//			if (abs(CV_MAT_ELEM(*mapx_L, float, y, x) - uv->x) <= 1)
//				if (abs(CV_MAT_ELEM(*mapy_L, float, y, x) - uv->y) <= 1)
//				{
//					uv->x = x;
//					uv->y = y;
//					return 1;
//				}
//#else
//			if (abs(CV_MAT_ELEM(*mapx_R, float, y, x) - uv->x) < 1)
//				if (abs(CV_MAT_ELEM(*mapy_R, float, y, x) - uv->y) < 1)
//				{
//					uv->x = x;
//					uv->y = y;
//					return 1;
//				}
//#endif
//		}
//	}
//	return 0;
//
//}
//void CalPixPointFrom3D(CvPoint3D32f xyz, CvPoint *uv)
//{
//	CvMat* xyz1 = cvCreateMat(4, 1, CV_32FC1);
//	CvMat* uv1 = cvCreateMat(3, 1, CV_32FC1);
//	CV_MAT_ELEM(*xyz1, float, 0, 0) = xyz.x;
//	CV_MAT_ELEM(*xyz1, float, 1, 0) = xyz.y;
//	CV_MAT_ELEM(*xyz1, float, 2, 0) = xyz.z;
//	CV_MAT_ELEM(*xyz1, float, 3, 0) = 1;
//#ifdef IF_IS_LEFT
//	cvmMul(AxRt1, xyz1, uv1);
//#else
//	cvmMul(AxRt2, xyz1, uv1);
//#endif
//	float x = CV_MAT_ELEM(*uv1, float, 0, 0) / CV_MAT_ELEM(*uv1, float, 2, 0);
//	float y = CV_MAT_ELEM(*uv1, float, 1, 0) / CV_MAT_ELEM(*uv1, float, 2, 0);
//	uv->x = x;
//	uv->y = y;
//}
//
//
////双目标定的外部参数标定
//void CalExtrPara(float *object_pointL, float *image_pointL, float *object_pointR, float *image_pointR)
//{
//	CvMat *object_matrixL = cvCreateMat(CORNER_NUML, 3, CV_32FC1);
//	cvInitMatHeader(object_matrixL, CORNER_NUML, 3, CV_32FC1, object_pointL);
//
//	CvMat *object_matrixR = cvCreateMat(CORNER_NUMR, 3, CV_32FC1);
//	cvInitMatHeader(object_matrixR, CORNER_NUMR, 3, CV_32FC1, object_pointR);
//
//	CvMat *image_matrix1 = cvCreateMat(CORNER_NUML, 2, CV_32FC1);
//	cvInitMatHeader(image_matrix1, CORNER_NUML, 2, CV_32FC1, image_pointL);
//
//	CvMat *image_matrix2 = cvCreateMat(CORNER_NUMR, 2, CV_32FC1);
//	cvInitMatHeader(image_matrix2, CORNER_NUMR, 2, CV_32FC1, image_pointR);
//
//	intrinsic1 = (CvMat *)cvLoad("imgL\\Intrinsics.xml");
//	distortion1 = (CvMat *)cvLoad("imgL\\Distortion.xml");
//
//	intrinsic2 = (CvMat *)cvLoad("imgR\\Intrinsics.xml");
//	distortion2 = (CvMat *)cvLoad("imgR\\Distortion.xml");
//
//	CvMat *rotation_vectors1 = cvCreateMat(1, 3, CV_32F);        //旋转向量
//	CvMat *translation_vectors1 = cvCreateMat(1, 3, CV_32F);        //平移向量
//	CvMat *rotation_vectors2 = cvCreateMat(1, 3, CV_32F);        //旋转向量
//	CvMat *translation_vectors2 = cvCreateMat(1, 3, CV_32F);        //平移向量
//	CvMat *rotation_matrix1 = cvCreateMat(3, 3, CV_32F);        //旋转向量
//	CvMat *rotation_matrix2 = cvCreateMat(3, 3, CV_32F);        //旋转向量
//
//	//CalFromUndistortImgPoint(image_matrix1,intrinsic1,distortion1);
//	cvFindExtrinsicCameraParams2(object_matrixL, image_matrix1,
//		intrinsic1, distortion1, rotation_vectors1, translation_vectors1);
//	//CalFromUndistortImgPoint(image_matrix2,intrinsic2,distortion2);
//	cvFindExtrinsicCameraParams2(object_matrixR, image_matrix2,
//		intrinsic2, distortion2, rotation_vectors2, translation_vectors2);
//	cvRodrigues2(rotation_vectors1, rotation_matrix1);
//	cvRodrigues2(rotation_vectors2, rotation_matrix2);
//	/************至此，参数就全计算出来了************/
//	RT_matrix1 = cvCreateMat(3, 4, CV_32F);
//	RT_matrix2 = cvCreateMat(3, 4, CV_32F);
//
//	AxRt1 = cvCreateMat(3, 4, CV_32F);
//	AxRt2 = cvCreateMat(3, 4, CV_32F);
//
//	AxRtExtnd1 = cvCreateMat(4, 4, CV_32F);
//	AxRtExtnd2 = cvCreateMat(4, 4, CV_32F);
//
//	AxRtExtnd1_1 = cvCreateMat(4, 4, CV_32F);
//	AxRtExtnd2_1 = cvCreateMat(4, 4, CV_32F);
//
//	Me2Me1_1 = cvCreateMat(4, 4, CV_32F);
//	Me1Me2_1 = cvCreateMat(4, 4, CV_32F);
//
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			CV_MAT_ELEM(*RT_matrix1, float, i, j) = CV_MAT_ELEM(*rotation_matrix1, float, i, j);
//			CV_MAT_ELEM(*RT_matrix2, float, i, j) = CV_MAT_ELEM(*rotation_matrix2, float, i, j);
//		}
//	}
//	for (int i = 0; i < 3; i++)
//	{
//		CV_MAT_ELEM(*RT_matrix1, float, i, 3) = CV_MAT_ELEM(*translation_vectors1, float, 0, i);
//		CV_MAT_ELEM(*RT_matrix2, float, i, 3) = CV_MAT_ELEM(*translation_vectors2, float, 0, i);
//	}
//	cvmMul(intrinsic1, RT_matrix1, AxRt1);
//	cvmMul(intrinsic2, RT_matrix2, AxRt2);//A*M
//
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			CV_MAT_ELEM(*AxRtExtnd1, float, i, j) = CV_MAT_ELEM(*AxRt1, float, i, j);
//			CV_MAT_ELEM(*AxRtExtnd2, float, i, j) = CV_MAT_ELEM(*AxRt2, float, i, j);
//		}
//	}
//	CV_MAT_ELEM(*AxRtExtnd1, float, 3, 0) = 0;
//	CV_MAT_ELEM(*AxRtExtnd2, float, 3, 0) = 0;
//
//	CV_MAT_ELEM(*AxRtExtnd1, float, 3, 1) = 0;
//	CV_MAT_ELEM(*AxRtExtnd2, float, 3, 1) = 0;
//
//	CV_MAT_ELEM(*AxRtExtnd1, float, 3, 2) = 0;
//	CV_MAT_ELEM(*AxRtExtnd2, float, 3, 2) = 0;
//
//	CV_MAT_ELEM(*AxRtExtnd1, float, 3, 3) = 1;
//	CV_MAT_ELEM(*AxRtExtnd2, float, 3, 3) = 1;
//
//	cvInvert(AxRtExtnd1, AxRtExtnd1_1);//求逆
//	cvInvert(AxRtExtnd2, AxRtExtnd2_1);//求逆
//
//	cvmMul(AxRtExtnd1, AxRtExtnd2_1, Me1Me2_1);
//	cvmMul(AxRtExtnd2, AxRtExtnd1_1, Me2Me1_1);
//
//
//	AxMxS_matrix = cvCreateMat(5, 5, CV_32F);
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			CV_MAT_ELEM(*AxMxS_matrix, float, i, j) = CV_MAT_ELEM(*AxRt1, float, i, j);
//		}
//	}
//	for (int i = 3; i < 5; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			CV_MAT_ELEM(*AxMxS_matrix, float, i, j) = CV_MAT_ELEM(*AxRt2, float, i - 3, j);
//		}
//	}
//
//
//	CV_MAT_ELEM(*AxMxS_matrix, float, 2, 3) = -1;
//	CV_MAT_ELEM(*AxMxS_matrix, float, 3, 3) = 0;
//	CV_MAT_ELEM(*AxMxS_matrix, float, 4, 3) = 0;
//
//	CV_MAT_ELEM(*AxMxS_matrix, float, 0, 4) = 0;
//	CV_MAT_ELEM(*AxMxS_matrix, float, 1, 4) = 0;
//	CV_MAT_ELEM(*AxMxS_matrix, float, 2, 4) = 0;
//
//
//	AxM_4 = cvCreateMat(5, 1, CV_32F);
//	XYZResult = cvCreateMat(5, 1, CV_32F);
//	for (int i = 0; i < 3; i++)
//	{
//		CV_MAT_ELEM(*AxM_4, float, i, 0) = -CV_MAT_ELEM(*AxRt1, float, i, 3);
//	}
//	for (int i = 3; i < 5; i++)
//	{
//		CV_MAT_ELEM(*AxM_4, float, i, 0) = -CV_MAT_ELEM(*AxRt2, float, i - 3, 3);
//	}
//	AxMxS_matrix_1 = cvCreateMat(5, 5, CV_32F);
//
//	cvReleaseMat(&object_matrixL);
//	cvReleaseMat(&object_matrixR);
//	cvReleaseMat(&image_matrix1);
//	cvReleaseMat(&image_matrix2);
//	cvReleaseMat(&rotation_vectors1);
//	cvReleaseMat(&translation_vectors1);
//	cvReleaseMat(&rotation_vectors2);
//	cvReleaseMat(&translation_vectors2);
//	cvReleaseMat(&rotation_matrix1);
//	cvReleaseMat(&rotation_matrix2);
//}
////返回值：返回两个相机找到点的误差，太大说明不是同一个点
//float CalUV2XYZ(float u1, float v1, float u2, float v2, CvPoint3D32f *xyz, CvPoint3D32f *xyzNew)
//{
//	CvPoint2D32f uv1 = cvPoint2D32f(u1, v1);
//	CvPoint2D32f uv2 = cvPoint2D32f(u2, v2);
//	CvPoint2D32f uvS1 = cvPoint2D32f(0, 0);
//	CvPoint2D32f uvS2 = cvPoint2D32f(0, 0);
//	CvMat *UVPoint1 = cvCreateMatHeader(1, 1, CV_32FC2);
//	CvMat *UVPoint2 = cvCreateMatHeader(1, 1, CV_32FC2);
//	CvMat *UVPointS1 = cvCreateMatHeader(1, 1, CV_32FC2);
//	CvMat *UVPointS2 = cvCreateMatHeader(1, 1, CV_32FC2);
//	cvInitMatHeader(UVPoint1, 1, 1, CV_32FC2, &uv1);
//	cvInitMatHeader(UVPoint2, 1, 1, CV_32FC2, &uv2);
//	cvInitMatHeader(UVPointS1, 1, 1, CV_32FC2, &uvS1);
//	cvInitMatHeader(UVPointS2, 1, 1, CV_32FC2, &uvS2);
//	cvUndistortPoints(UVPoint1, UVPointS1, intrinsic1, distortion1);
//	cvUndistortPoints(UVPoint2, UVPointS2, intrinsic2, distortion2);
//	CvMat* uv1xA = cvCreateMat(3, 1, CV_32F);
//	CvMat* uv1xARst = cvCreateMat(3, 1, CV_32F);
//	CV_MAT_ELEM(*uv1xA, float, 0, 0) = uvS1.x;
//	CV_MAT_ELEM(*uv1xA, float, 1, 0) = uvS1.y;
//	CV_MAT_ELEM(*uv1xA, float, 2, 0) = 1;
//	cvmMul(intrinsic1, uv1xA, uv1xA);
//	CvMat* uv2xA = cvCreateMat(3, 1, CV_32F);
//	CvMat* uv2xARst = cvCreateMat(3, 1, CV_32F);
//	CV_MAT_ELEM(*uv2xA, float, 0, 0) = uvS2.x;
//	CV_MAT_ELEM(*uv2xA, float, 1, 0) = uvS2.y;
//	CV_MAT_ELEM(*uv2xA, float, 2, 0) = 1;
//	cvmMul(intrinsic2, uv2xA, uv2xA);
//	cvCopy(uv2xA, uv2xARst);
//	cvCopy(uv1xA, uv1xARst);
//	CV_MAT_ELEM(*AxMxS_matrix, float, 0, 3) = -CV_MAT_ELEM(*uv1xA, float, 0, 0);//u1
//	CV_MAT_ELEM(*AxMxS_matrix, float, 1, 3) = -CV_MAT_ELEM(*uv1xA, float, 1, 0);//v1
//	CV_MAT_ELEM(*AxMxS_matrix, float, 3, 4) = -CV_MAT_ELEM(*uv2xA, float, 0, 0);//u2
//	CV_MAT_ELEM(*AxMxS_matrix, float, 4, 4) = -CV_MAT_ELEM(*uv2xA, float, 1, 0);//v2
//
//
//	cvInvert(AxMxS_matrix, AxMxS_matrix_1);//求逆
//	cvmMul(AxMxS_matrix_1, AxM_4, XYZResult);
//	xyz->x = CV_MAT_ELEM(*XYZResult, float, 0, 0);
//	xyz->y = CV_MAT_ELEM(*XYZResult, float, 1, 0);
//	xyz->z = CV_MAT_ELEM(*XYZResult, float, 2, 0);
//	printf("x:%f\ny:%f\nz:%f\n", xyz->x, xyz->y, xyz->z);//打印出X,Y,Z
//	CvMat * SubMe2Me1_1 = cvCreateMat(3, 3, CV_32F);
//	cvGetSubRect(Me2Me1_1, SubMe2Me1_1, cvRect(0, 0, 3, 3));
//	cvmMul(SubMe2Me1_1, uv1xARst, uv1xARst);
//	//PrintMat(Me2Me1_1,0,NULL);
//	float a = CV_MAT_ELEM(*uv1xARst, float, 0, 0);
//	float b = CV_MAT_ELEM(*uv1xARst, float, 1, 0);
//	float c = CV_MAT_ELEM(*uv1xARst, float, 2, 0);
//	float d1 = CV_MAT_ELEM(*Me2Me1_1, float, 0, 3);
//	float d2 = CV_MAT_ELEM(*Me2Me1_1, float, 1, 3);
//	float d3 = CV_MAT_ELEM(*Me2Me1_1, float, 2, 3);
//	float kx = (d2*c - b*d3) / (d1*c - d3*a);
//	float bx = (d1*b - d2*a) / (d1*c - d3*a);
//	u1 = CV_MAT_ELEM(*uv2xA, float, 0, 0);
//	v1 = CV_MAT_ELEM(*uv2xA, float, 1, 0);
//	float tmpv1 = u1*kx + bx;//tmpv1 is the value calculated base on the other camera.
//	float error = fabs(tmpv1 - v1);
//	printf("\ncalv1:%f,v1:%f error: %f\n", tmpv1, v1, error);
//
//
//	// if the err is minner than the thresh,we can calc again using the tmpv1
//	CV_MAT_ELEM(*AxMxS_matrix, float, 4, 4) = -tmpv1;//v2
//	cvInvert(AxMxS_matrix, AxMxS_matrix_1);//求逆
//	cvmMul(AxMxS_matrix_1, AxM_4, XYZResult);
//	xyzNew->x = CV_MAT_ELEM(*XYZResult, float, 0, 0);
//	xyzNew->y = CV_MAT_ELEM(*XYZResult, float, 1, 0);
//	xyzNew->z = CV_MAT_ELEM(*XYZResult, float, 2, 0);
//
//
//	cvReleaseMat(&UVPoint1);
//	cvReleaseMat(&UVPoint2);
//	cvReleaseMat(&UVPointS1);
//	cvReleaseMat(&UVPointS2);
//	cvReleaseMat(&uv1xA);
//	cvReleaseMat(&uv2xA);
//	cvReleaseMat(&uv1xARst);
//	cvReleaseMat(&uv2xARst);
//	return error;
//}
//
//void CalFromUndistortImgPoint(CvMat * image_matrix, CvMat* intrinsic, CvMat* distortion)
//{
//	CvMat* imagePoint = cvCreateMat(3, 1, CV_32F);
//	CvPoint2D32f uv = cvPoint2D32f(0, 0);
//	CvPoint2D32f uvS = cvPoint2D32f(0, 0);
//	CvMat *UVPoint = cvCreateMatHeader(1, 1, CV_32FC2);
//	CvMat *UVPointS = cvCreateMatHeader(1, 1, CV_32FC2);
//	cvInitMatHeader(UVPoint, 1, 1, CV_32FC2, &uv);
//	cvInitMatHeader(UVPointS, 1, 1, CV_32FC2, &uvS);
//
//	for (int i = 0; i < image_matrix->rows; i++)
//	{
//		uv.x = CV_MAT_ELEM(*image_matrix, float, i, 0);
//		uv.y = CV_MAT_ELEM(*image_matrix, float, i, 1);
//		cvUndistortPoints(UVPoint, UVPointS, intrinsic, distortion);
//		CV_MAT_ELEM(*imagePoint, float, 0, 0) = uvS.x;
//		CV_MAT_ELEM(*imagePoint, float, 1, 0) = uvS.y;
//		CV_MAT_ELEM(*imagePoint, float, 2, 0) = 1;
//		cvmMul(intrinsic, imagePoint, imagePoint);
//
//		CV_MAT_ELEM(*image_matrix, float, i, 0) = CV_MAT_ELEM(*imagePoint, float, 0, 0);
//		CV_MAT_ELEM(*image_matrix, float, i, 1) = CV_MAT_ELEM(*imagePoint, float, 1, 0);
//	}
//	cvReleaseMat(&imagePoint);
//	cvReleaseMat(&UVPoint);
//	cvReleaseMat(&UVPointS);
//}
///*********************************
//函数名：  PrintMat(CvMat *matrix)
//函数输入：matrix指针 数据类型opencv规定的任意一个
//函数作用：在屏幕上打印矩阵
//**********************************/
//void PrintMat(CvMat *matrix, BOOL save_or_show, FILE *fp)
//{
//	int i = 0;
//	int j = 0;
//	for (i = 0; i < matrix->rows; i++)//行
//	{
//		if (save_or_show)
//		{
//			fprintf(fp, "\n");
//		}
//		else
//		{
//			printf("\n");
//		}
//		switch (matrix->type & 0X07)
//		{
//		case CV_32F:
//		case CV_64F:
//		{
//			for (j = 0; j<matrix->cols; j++)//列
//			{
//				if (save_or_show)
//				{
//					fprintf(fp, "%9.3f ", (float)cvGetReal2D(matrix, i, j));
//				}
//				else
//				{
//					printf("%9.3f ", (float)cvGetReal2D(matrix, i, j));
//				}
//
//			}
//
//
//			break;
//		}
//		case CV_8U:
//		case CV_16U:
//		{
//			for (j = 0; j<matrix->cols; j++)
//			{
//				printf("%6d  ", (int)cvGetReal2D(matrix, i, j));
//				if (save_or_show)
//				{
//					fprintf(fp, "%6d  ", (int)cvGetReal2D(matrix, i, j));
//				}
//				else
//				{
//					printf("%6d  ", (int)cvGetReal2D(matrix, i, j));
//				}
//			}
//
//			break;
//		}
//
//		default:
//			break;
//
//		}
//	}
//
//}
//
//int calib(char L_Or_R)
//{
//	char loadDis[100];
//	char SaveDis[100];
//	int  i = 1;
//	char k = 0;
//	int  CurrentImage = 0;
//	int  CurrentRow = 0;         //行 
//	int  CurrentColumn = 0;         //列
//	int  findcorner_result = 0;
//
//	FILE *fp;                        //文件指针
//
//
//	int ChessBoardSize_w = CHESS_WIDTH;       //角点个数
//	int ChessBoardSize_h = CHESS_HIGHT;
//	int width_pixel = WIDTH_PIX;     //像素
//	int high_pixel = HIGHT_PIX;
//
//
//	float      SquareHight = SQUAR_HIGHT;       //棋盘大小
//	float      SquareWidth = SQUAR_WIDTH;       //棋盘大小
//	int        NImages = NUM_IMAGE;
//	CvSize     ChessBoardSize;
//	CvSize     image_pixel;
//	int        NPoints = 0;
//
//	int          *corner_counter;
//	float        *temppoints;       //这里可以使用内存动态存储管理。。。
//	CvPoint2D32f *corners;          //存储角点坐标的数组
//
//	//单通道灰度图像
//	IplImage   *grayimage = 0;
//	IplImage   *srcimage = 0;              //三通道图像
//	IplImage   *result_image = 0;          //矫正以后的图像
//
//
//	CvMat      *intrinsic_matrix = 0;   //内部参数矩阵
//	CvMat      *distortion_coeffs = 0;  //畸变系数
//	CvMat      *rotation_vectors = 0;   //旋转向量
//	CvMat      *translation_vectors = 0;//平移向量
//	CvMat      *points_counts = 0;      //图片角点数
//	CvMat      *object_points = 0;      //世界坐标系中角点的坐标
//	CvMat      *image_points = 0;       //检测到的角点坐标
//
//	CvMat *temp_matrix, *rotation_matrix, *translation_matrix;
//
//	//*****************************************************
//	//数据载入、开辟空间
//	image_pixel = cvSize(width_pixel, high_pixel);
//	ChessBoardSize = cvSize(ChessBoardSize_w, ChessBoardSize_h);
//	NPoints = ChessBoardSize_w*ChessBoardSize_h;
//
//	corner_counter = (int*)calloc(NImages, sizeof(int));                   //动态管理，记着释放空间
//	temppoints = (float*)calloc(NImages*NPoints * 3, sizeof(float));
//	corners = (CvPoint2D32f*)calloc(NImages*NPoints, sizeof(CvPoint2D32f));
//
//	if ((corner_counter == 0) || (temppoints == 0) || (corners == 0))
//	{
//		return -1;
//	}
//	intrinsic_matrix = cvCreateMat(3, 3, CV_32FC1);              //内参数矩阵
//	distortion_coeffs = cvCreateMat(1, 5, CV_32FC1);              //形变参数
//
//	rotation_vectors = cvCreateMat(NImages, 3, CV_32FC1);        //旋转向量
//	translation_vectors = cvCreateMat(NImages, 3, CV_32FC1);        //平移向量
//
//
//	points_counts = cvCreateMat(NImages, 1, CV_32SC1);        //视图数目
//
//	object_points = cvCreateMat(NImages*NPoints, 3, CV_32FC1);//世界坐标系中角点的坐标
//	image_points = cvCreateMat(NImages*NPoints, 2, CV_32FC1);//检测到的坐标点坐标
//
//	temp_matrix = cvCreateMat(1, 3, CV_32FC1);
//	rotation_matrix = cvCreateMat(3, 3, CV_32FC1);              //旋转矩阵
//	translation_matrix = cvCreateMat(3, 3, CV_32FC1);              //旋转矩阵
//
//	grayimage = cvCreateImage(image_pixel, IPL_DEPTH_8U, 1);       //单通道灰度图像        
//	result_image = cvCreateImage(image_pixel, IPL_DEPTH_8U, 1); //校正以后的图像
//
//	fp = fopen("data1.txt", "w+");  //打开文件,建立一个文件，然后写入数据
//	fprintf(fp, "坐标数据：\n");  //写入数据，写入文件
//	//****************************************************
//	//图片的加载以及角点的提取 
//	//if(argc!=NImages+1)
//	//{
//	//	printf("图片加载有误!"); 
//	//	return -1;
//	//}
//	for (CurrentImage = 0; CurrentImage<NImages; CurrentImage++)
//	{
//		if (L_Or_R == 'L')
//		{
//			sprintf(loadDis, "imgL\\%d.jpg", CurrentImage + 1);
//		}
//		else if (L_Or_R == 'R')
//		{
//			sprintf(loadDis, "imgR\\%d.jpg", CurrentImage + 1);
//		}
//		//加载图片
//		if ((srcimage = cvLoadImage(loadDis, 3)) != 0)
//		{
//			//色彩转换
//			cvCvtColor(srcimage, grayimage, CV_BGR2GRAY);
//			//角点检测
//			findcorner_result = cvFindChessboardCorners(grayimage, ChessBoardSize, &corners[CurrentImage*NPoints],
//				&corner_counter[CurrentImage], CV_CALIB_CB_ADAPTIVE_THRESH);
//
//			//精确坐标位置
//			cvFindCornerSubPix(grayimage, &corners[CurrentImage*NPoints], corner_counter[CurrentImage],
//				cvSize(10, 10), cvSize(-1, -1),//这个搜索的范围。。。？
//				cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.003)//迭代终止条件
//				);
//			//画出检测到的点
//			cvDrawChessboardCorners(srcimage, ChessBoardSize,
//				&corners[CurrentImage*NPoints], corner_counter[CurrentImage], findcorner_result);
//			cvNamedWindow("image", 0);
//			cvShowImage("image", srcimage);
//			printf("第%d幅图片角点。。。\n", CurrentImage + 1);
//			printf("检测到的角点:%d\n", corner_counter[CurrentImage]);
//			//fprintf(fp,"检测到的角点:%d\n",corner_counter[CurrentImage]);
//			//for(i=0;i<corner_counter[CurrentImage];i++)//坐标输出
//			// {
//			//    printf("第%d个角点  %f  %f\n",i,corners[CurrentImage*NPoints+i].x,corners[CurrentImage*NPoints+i].y);
//			//}
//			cvWaitKey(0);
//			//fprintf(fp,"按任意键提取下一幅图片角点。。。\n");
//
//		}
//	}
//
//	printf("角点提取结束......\n");
//
//	printf("开始定标......\n");
//	fprintf(fp, "角点提取结束......\n");
//
//	fprintf(fp, "开始定标......\n");
//
//	//棋盘世界坐标系坐标
//	for (CurrentImage = 0; CurrentImage < NImages; CurrentImage++)//图片
//	{
//		for (CurrentRow = 0; CurrentRow < ChessBoardSize_h; CurrentRow++)//行
//		{
//			for (CurrentColumn = 0; CurrentColumn < ChessBoardSize_w; CurrentColumn++)//列
//			{
//				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize_w + CurrentColumn) * 3] = (float)(CurrentRow*SquareHight);
//				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize_w + CurrentColumn) * 3 + 1] = (float)(CurrentColumn*SquareWidth);
//				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize_w + CurrentColumn) * 3 + 2] = 0;
//			}
//		}
//	}
//	//参数求解
//	*object_points = cvMat(NImages*NPoints, 3, CV_32FC1, temppoints);//似乎这里开始有点问题。。。还有就是在标定过程中这个棋盘的边长和标定的精度无关
//	cvSetData(image_points, corners, sizeof(CvPoint2D32f));//设置image_points矩阵的数据
//	cvSetData(points_counts, corner_counter, sizeof(int));
//
//	printf("棋盘坐标");
//	printf("\n");
//	PrintMat(object_points, FALSE, NULL);
//	printf("\n计算机\n");
//	printf("\n");
//	PrintMat(image_points, FALSE, NULL);
//	printf("\n");
//	printf("\n角点个数\n");
//	PrintMat(points_counts, FALSE, NULL);
//	printf("\n");
//
//	cvCalibrateCamera2(object_points, image_points, points_counts, cvGetSize(grayimage),
//		intrinsic_matrix, distortion_coeffs, rotation_vectors,
//		translation_vectors, 0
//		);
//
//	printf("\n内部参数矩阵\n");
//	fprintf(fp, "\n内部参数矩阵\n");
//
//	PrintMat(intrinsic_matrix, FALSE, NULL);
//	PrintMat(intrinsic_matrix, TRUE, fp);
//
//	printf("\n形变参数\n");
//
//	fprintf(fp, "\n形变参数\n");
//	PrintMat(distortion_coeffs, FALSE, NULL);
//	PrintMat(distortion_coeffs, TRUE, fp);
//
//
//
//	for (i = 0; i<NImages; i++)
//	{
//
//		cvGetRow(rotation_vectors, temp_matrix, i);
//		cvRodrigues2(temp_matrix, rotation_matrix, 0);
//
//		printf(" 第%d个图片的旋转向量\n", i + 1);
//		fprintf(fp, " 第%d个图片的旋转向量\n", i + 1);
//
//		PrintMat(temp_matrix, FALSE, NULL);
//		PrintMat(temp_matrix, TRUE, fp);
//
//		printf(" 第%d个图片的旋转矩阵\n", i + 1);
//		fprintf(fp, " 第%d个图片的旋转矩阵\n", i + 1);
//
//		PrintMat(rotation_matrix, FALSE, NULL);
//		PrintMat(rotation_matrix, TRUE, fp);
//		printf("\n");
//		fprintf(fp, "\n");
//
//	}
//	//cvReleaseMat(&temp_matrix);
//	printf("平移矩阵\n");
//	fprintf(fp, "平移矩阵\n");
//
//	for (i = 0; i<NImages; i++)
//	{
//
//		cvGetRow(translation_vectors, temp_matrix, i);
//		cvRodrigues2(temp_matrix, translation_matrix, 0);
//
//		printf(" 第%d个图片的平移向量\n", i + 1);
//		fprintf(fp, " 第%d个图片的平移向量\n", i + 1);
//		PrintMat(temp_matrix, FALSE, NULL);
//		PrintMat(temp_matrix, TRUE, fp);
//
//		printf(" 第%d个图片的平移矩阵\n", i + 1);
//		fprintf(fp, " 第%d个图片的平移矩阵\n", i + 1);
//		PrintMat(translation_matrix, FALSE, NULL);
//		PrintMat(translation_matrix, TRUE, fp);
//		printf("\n");
//		fprintf(fp, "\n");
//
//	}
//
//	//  PrintMat(translation_vectors,FALSE,NULL);
//	//  PrintMat(translation_vectors,TRUE,fp);
//
//	printf("标定结束.....\n");
//	fprintf(fp, "标定结束.....\n");
//
//	printf("按任意键开始误差分析.....\n");
//	fprintf(fp, "按任意键开始误差分析.....\n");
//	cvWaitKey(0);
//	//误差分析
//	//利用已知的内部参数，使用cvProjectPoints2()计算出世界坐标系中的坐标在图片中的坐标也就是校准后的坐标
//	//将校准后的坐标和原来求得的坐标进行比较
//	for (CurrentImage = 0; CurrentImage<NImages; CurrentImage++)
//	{
//		CvMat *object_matrix = cvCreateMat(NPoints, 3, CV_32FC1);
//		CvMat *image_matrix = cvCreateMat(NPoints, 2, CV_32FC1);
//		CvMat *project_image_matrix = cvCreateMat(NPoints, 2, CV_32FC1);
//		CvMat *rotation_matrix_1 = cvCreateMat(1, 3, CV_32FC1);
//		CvMat *translation_matrix_1 = cvCreateMat(1, 3, CV_32FC1);
//		CvMat *rotation_matrix = cvCreateMat(3, 1, CV_32FC1);
//		CvMat *translation_matrix = cvCreateMat(3, 1, CV_32FC1);
//		double err = 0;
//
//		cvGetRows(object_points, object_matrix, CurrentImage*NPoints, (CurrentImage + 1)*NPoints, 1);
//		cvGetRow(rotation_vectors, rotation_matrix_1, CurrentImage);
//		cvReshape(rotation_matrix_1, rotation_matrix, 0, 3);
//		cvGetRow(translation_vectors, translation_matrix_1, CurrentImage);
//		cvReshape(translation_matrix_1, translation_matrix, 0, 3);
//		cvGetRows(image_points, project_image_matrix, CurrentImage*NPoints, (CurrentImage + 1)*NPoints, 1);
//
//		cvProjectPoints2(object_matrix, rotation_matrix, translation_matrix,
//			intrinsic_matrix, distortion_coeffs, image_matrix, 0, 0, 0, 0, 0);
//
//		err = cvNorm(image_matrix, project_image_matrix, CV_L2, 0);
//
//		printf("第%d幅图像的误差为%f\n", CurrentImage + 1, err);
//		fprintf(fp, "第%d幅图像的误差为%f\n", CurrentImage + 1, err);
//
//	}
//
//	cvNamedWindow("Undistort_image", 0);
//	//显示校正后的图片
//	printf("\n");
//	printf("校正后的图片....\n");
//	if (L_Or_R == 'L')
//	{
//		cvSave("imgL\\Intrinsics.xml", intrinsic_matrix);
//		cvSave("imgL\\Distortion.xml", distortion_coeffs);
//	}
//	else if (L_Or_R == 'R')
//	{
//		cvSave("imgR\\Intrinsics.xml", intrinsic_matrix);
//		cvSave("imgR\\Distortion.xml", distortion_coeffs);
//	}
//	for (CurrentImage = 0; CurrentImage<NImages; CurrentImage++)
//	{
//		if (L_Or_R == 'L')
//		{
//			sprintf(loadDis, "imgL\\%d.jpg", CurrentImage + 1);
//		}
//		else if (L_Or_R == 'R')
//		{
//			sprintf(loadDis, "imgR\\%d.jpg", CurrentImage + 1);
//		}
//		//加载图片
//		if ((srcimage = cvLoadImage(loadDis, 3)) != 0)
//		{
//			//色彩转换
//			cvCvtColor(srcimage, grayimage, CV_BGR2GRAY);
//			cvUndistort2(grayimage, result_image, intrinsic_matrix, distortion_coeffs);
//			cvShowImage("Undistort_image", result_image);
//			printf("按任意键显示下一幅图片。。。\n");
//			cvWaitKey(0);
//		}
//	}
//
//	//关闭文件  
//	fclose(fp);
//	//释放内存
//	cvWaitKey(0);
//
//	free(corner_counter);
//	free(temppoints);
//	free(corners);
//	cvDestroyWindow("Undistort_image");
//	cvDestroyWindow("Image");
//
//	cvReleaseMat(&intrinsic_matrix);
//	cvReleaseMat(&distortion_coeffs);
//	cvReleaseMat(&rotation_vectors);
//	cvReleaseMat(&translation_vectors);
//	cvReleaseMat(&rotation_matrix);
//	cvReleaseMat(&translation_matrix);
//	cvReleaseMat(&points_counts);
//	cvReleaseMat(&object_points);
//	cvReleaseMat(&image_points);
//	cvReleaseMat(&temp_matrix);
//
//	cvReleaseImage(&srcimage);
//	cvReleaseImage(&result_image);
//	cvReleaseImage(&grayimage);
//	return 0;
//}
//#define CALC_FIRST 1
//float xs = 2552, ys = 0, theta = 0;
//int calcRobotMove()
//{
//
//#ifdef CALC_FIRST
//	float x0 = 2706, y0 = 3879;//robot's 
//	float x1 = 2522, y1 = 3914;//real
//	theta = atan2(y0 - ys, x0 - xs) - atan2(y1 - ys, x1 - xs);
//	FILE*fp = fopen("theta.txt", "w");
//	fprintf(fp, "%.4f\n", theta);
//	fclose(fp);
//#else
//	static int ii = 0;
//	if (ii == 0)
//	{
//		ii = 1;
//		FILE*fp = fopen("theta.txt", "r+");
//		fscanf(fp, "%f", &theta);
//		fclose(fp);
//
//	}
//	theta = 1.705*3.1415926 / 180;
//	float x = 2000, y = 2000;
//	float l = 0;
//	float sdaf = atan2((y - ys), (x - xs));
//
//	//下面是让到x,y,实际给底盘的坐标
//	l = (x - xs)*(x - xs) + (y - ys)*(y - ys);
//	float ss = 3.1415926 / 2 - sdaf + theta;
//	y = sqrt(l)*cos(ss) + ys;
//	x = sqrt(l)*sin(ss) + xs;
//#endif
//	return 0;
//}