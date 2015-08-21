#include "stdafx.h"
#include "windows.h"
#include <process.h>
#include <opencv2/opencv.hpp>
#include <IDevice.h>

#define TEST

#define IMGWIDTH	640
#define IMGHEIGHT	480

volatile bool quitProcess = false;
volatile bool debugMode = false;

//ȫ�ֱ���
IDevice *LeftDevice = NULL;//ָ������豸�����ָ��
IDevice *RightDevice = NULL;
IplImage *LeftImg = NULL;
IplImage *RightImg = NULL;

//Theread
HANDLE LeftCamReceive = NULL;
HANDLE RightCamReceive = NULL;
HANDLE LeftImgProcess = NULL;
HANDLE RightImaProcess = NULL;

#ifdef SHOW
	HANDLE ShowImgProcess = NULL;
#endif

//Semaphore
HANDLE LeftImgOver;
HANDLE RightImgOver;
HANDLE LeftProcessOver;
HANDLE RightProcessOver;
HANDLE LeftCalcOver;
HANDLE RightCalcOver;

//Function Declaration
DeviceStatus CameraInit(IDevice **pDevice, UCHAR devIndex);
unsigned int __stdcall LeftCaptureThread(LPVOID lpParameter);
unsigned int __stdcall RightCaptureThread(LPVOID lpParameter);
unsigned int __stdcall ImgShowThread(LPVOID lpParameter);
unsigned int __stdcall LeftImgCalcThread(LPVOID lpParameter);
unsigned int __stdcall RightImgCalcThread(LPVOID lpParameter);

//Main Process
int _tmain(int argc, _TCHAR* argv[])
{
	CameraInit(&LeftDevice, 0);
	CameraInit(&RightDevice, 1);

	LeftImgOver = CreateSemaphore(NULL, 0, 1, NULL);
	RightImgOver = CreateSemaphore(NULL, 0, 1, NULL);
	LeftProcessOver = CreateSemaphore(NULL, 0, 1, NULL);
	RightProcessOver = CreateSemaphore(NULL, 0, 1, NULL);

	LeftImg = cvCreateImageHeader(cvSize(IMGWIDTH, IMGHEIGHT), IPL_DEPTH_8U, 3);
	RightImg = cvCreateImageHeader(cvSize(IMGWIDTH, IMGHEIGHT), IPL_DEPTH_8U, 3);



#ifdef SHOW
	ShowImgProcess = _beginthreadex(NULL, 0, ImgShowThread, NULL, 0, NULL);
#endif
	LeftCamReceive = (HANDLE)_beginthreadex(NULL, 0, LeftCaptureThread, NULL, 0, NULL);
	LeftCamReceive = (HANDLE)_beginthreadex(NULL, 0, RightCaptureThread, NULL, 0, NULL);
	LeftImgProcess = (HANDLE)_beginthreadex(NULL, 0, LeftImgCalcThread, NULL, 0, NULL);
	RightImaProcess = (HANDLE)_beginthreadex(NULL, 0, RightImgCalcThread, NULL, 0, NULL);


	while (true)
	{
		switch (cvWaitKey(33))
		{
		case 27: //'esc' key has been pressed, exit program.
			LeftDevice->Stop();
			LeftDevice->CloseDevice();
			LeftDevice->DeviceUnInit();
			LeftDevice->Release();
			RightDevice->Stop();
			RightDevice->CloseDevice();
			RightDevice->DeviceUnInit();
			RightDevice->Release();
			IDevice::DeviceUnInitialSDK();
			return 0;
		case 100: //'d' has been pressed. this will debug mode
			debugMode = !debugMode;
			if (debugMode == false) printf("Debug mode disabled.\n");
			else printf("Debug mode enabled.\n");
			break;
		//case 112: //'p' has been pressed. this will pause/resume the code.
		//	pause = !pause;
		//	if (pause == true)
		//	{
		//		printf("Code paused, press 'p' again to resume\n");
		//		while (pause == true)
		//		{
		//			//stay in this loop until 
		//			switch (cvWaitKey())
		//			{
		//			case 112:
		//				//change pause back to false
		//				pause = false;
		//				printf("Code resumed.\n");
		//				break;
		//			}
		//		}
		//	}
		}
	}
	


	HANDLE multitask[4];
	multitask[0] = LeftCamReceive;
	multitask[1] = RightCamReceive;
	multitask[2] = LeftImgProcess;
	multitask[3] = RightImaProcess;
	WaitForMultipleObjects(2, multitask, TRUE, INFINITE);

	//for (int i = 0; i < 2; i++)
	//	CloseHandle(hThread[i]);
	//CloseHandle(g_hEmpty);
	//CloseHandle(g_hFull);
	//DeleteCriticalSection(&g_cs);
}

/*------------------------------------------------------------------------*\
| ��  �ܣ������������
|
| ����ֵ�����óɹ�����STATUS_OK�����򷵻ش������
|
| ��  ����
|         1  IDevice			**pDevice		�豸�ӿ�			 �������
|         2  UCHAR				dev				�豸��������(0-n)	 �������
\*------------------------------------------------------------------------*/
DeviceStatus CameraInit(IDevice **pDevice, UCHAR devIndex)
{
	INT nDevice = 0;
	DeviceStatus devStatus = STATUS_OK;
	if (SUCCEEDED(IDevice::DeviceInitialSDK(NULL, FALSE)))//SDK��ʼ��
	{
		if (SUCCEEDED(IDevice::EnumDevice(NULL, &nDevice)) && nDevice > 0)//numDeviceö���豸����õ�ǰ�豸��Ϣ
		{
			UionOpenDeviceParam deParam;
			deParam.devIndex = devIndex;
			if (SUCCEEDED(IDevice::OpenDevice(deParam, pDevice, OF_BYPOSITION)))//��ָ�������
			{
				if (SUCCEEDED((*pDevice)->DeviceInit(NULL, TRUE, FALSE)))//��������豸ö����Ϣ��ʼ�����
				{
					//devStatus = pDevice->DeviceCreateSettingPage(NULL);//����������������ô���
					//printf("CreateSettingPage the camera! code is %d\n", devStatus);
					//devStatus = pDevice->DeviceShowSettingPage(TRUE);//��ʾ������ô���
					//printf("ShowSettingPage the camera! code is %d\n", devStatus);
					//devStatus = pDevice->DeviceSetISPDataType(DATA_ISP_RGB24);//����ͼ�����ݴ����������RGB8λ���

					//INT devHeight, devWidth;
					//(*pDevice)->GetCapabilityImageSize(&devHeight, &devWidth);
					//DeviceROISize devSize;
					//devSize.iWidth = 640;
					//devSize.iHeight = 480;
					//devSize.iHOffset = (devHeight - devSize.iHeight) / 2;
					//devSize.iVOffset = (devWidth - devSize.iWidth)/ 2;
					//ResolutionParam reParam = {{devSize, 0}, RT_ROI };
					//devStatus = (*pDevice)->SetResolution(reParam);//���������ǰ�ֱ���


					ResolutionParam reParam;
					reParam.type = RT_PRESET;
					reParam.dri.ucIndex = 6;//�ֱ�������ֵ(2592x1944����ֵΪ0)
					devStatus = (*pDevice)->SetResolution(reParam);//���������ǰ�ֱ���

					devStatus = (*pDevice)->SetMediaType(DATA_TYPE_BAYGR8);//�������ͼ�����ݵ�����
					devStatus = (*pDevice)->SetFrameSpeed(HIGHEST_SPEED);//����HIGHEST_SPEED���֡��
					devStatus = (*pDevice)->SetFrameSpeedTune(0.9);
					devStatus = (*pDevice)->SetCaptureMode(SOFT_TRIGGER_MODE);//���ô���ģʽ
					devStatus = (*pDevice)->SetSoftTriggerFrameNumber(1);//��������һ��֡��
					(*pDevice)->SetAutoExposureState(false);//�����Զ��ع�״̬
					devStatus = (*pDevice)->SetExposureTime(10);//�����ع�ʱ��
					//(*pDevice)->SetAutoExposureState(true);//�����Զ��ع�״̬
					//devStatus = (*pDevice)->SetAutoExposureTarget(10);//�����ع�ʱ��
					devStatus = (*pDevice)->Start();//����Ƶ����SDK ����ͼ��ɼ�ģʽ

					DeviceCapability pDeviceInfo;
					devStatus = (*pDevice)->GetCapability(&pDeviceInfo);
					printf("time:%dms\n", pDeviceInfo.pFrameSpeed);

					printf("Camera_%d:Init OK\n", devIndex);
					return(STATUS_OK);
				}
				else
				{
					printf("Init camera Failed!\n");
					return(STATUS_FAILED);
				}
			}
			else
			{
				printf("Open camera Failed!\n");
				return(STATUS_FAILED);
			}
		}
		else
		{
			printf("No camera was found!\n");
			return(STATUS_FAILED);
		}
	}
	else
	{
		printf("Device Init Failed!\n");
		return(STATUS_FAILED);
	}
}

/*------------------------------------------------------------------------*\
| ��  �ܣ�����������ɼ��߳�
|
| ����ֵ��NULL
|
| ��  ����
|         1  IDevice			**pDevice		�豸�ӿ�			 �������
|         2  UCHAR				dev				�豸��������(0-n)	 �������
|
| ��  ����
\*------------------------------------------------------------------------*/
unsigned int __stdcall LeftCaptureThread(LPVOID lpParameter)
{
	BYTE *pRGB24Buff = new BYTE[IMGWIDTH * IMGHEIGHT * 3];//����ͼ�����ݵĻ�����
	DeviceFrameInfo dfInfo;
	while (true)
	{
		LeftDevice->SoftTriggerOnce();
		LeftDevice->DeviceGetImageBufferEx2(pRGB24Buff, -1, &dfInfo);
		cvSetData(LeftImg, pRGB24Buff, dfInfo.uiWidth * 3);
		ReleaseSemaphore(LeftImgOver, 1, NULL);
	}
	//delete[]pRGB24Buff;
	//return 0;
}


unsigned int __stdcall RightCaptureThread(LPVOID lpParameter)
{
	BYTE *pRGB24Buff = new BYTE[IMGWIDTH * IMGHEIGHT * 3];//����ͼ�����ݵĻ�����
	DeviceFrameInfo dfInfo;
	while (true)
	{
		RightDevice->SoftTriggerOnce();
		RightDevice->DeviceGetImageBufferEx2(pRGB24Buff, -1, &dfInfo);
		cvSetData(RightImg, pRGB24Buff, dfInfo.uiWidth * 3);
		ReleaseSemaphore(RightImgOver, 1, NULL);
	}
}

unsigned int __stdcall ImgShowThread(LPVOID lpParameter)
{
	//ReleaseSemaphore(LeftCalcOver, 1, NULL);
	return 0;
}

unsigned int __stdcall LeftImgCalcThread(LPVOID lpParameter)
{
	WaitForSingleObject(LeftImgOver, INFINITE); 

	ReleaseSemaphore(LeftCalcOver, 1, NULL);
	return 0;
}

unsigned int __stdcall RightImgCalcThread(LPVOID lpParameter)
{
	WaitForSingleObject(RightImgOver, INFINITE);

	ReleaseSemaphore(RightCalcOver, 1, NULL);
	return 0;
}






void on_trackbar(int pos);
void createTrackbars();

int theObject[2] = { 0, 0 };
CvFont font;
CvRect objectBoundingRectangle = cvRect(0, 0, 0, 0);
int H_MIN = 15;
int H_MAX = 82;
int S_MIN = 44;
int S_MAX = 184;
int V_MIN = 208;
int V_MAX = 256;


void searchForMovement(IplImage *thresholdImage, IplImage *cameraFeed)
{
	bool objectDetected = false;
	CvSeq *contours = NULL;
	CvMemStorage *storage = cvCreateMemStorage(0);
	IplImage* temp = cvCreateImage(cvGetSize(thresholdImage), thresholdImage->depth, thresholdImage->nChannels);
	cvCopy(thresholdImage, temp, NULL);
	cvFindContours(thresholdImage, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);// retrieves external contours

	if (contours > 0)
	{
		objectDetected = true;
	}
	else
	{
		objectDetected = false;
	}

	if (objectDetected)
	{
		//CvSeq *largestContourVec = 0;
		//cvContourArea();
		objectBoundingRectangle = cvBoundingRect(contours);//Ѱ�ұ߽����
		//contourTmpMin = cvMinAreaRect2(pContour, 0);//Ѱ����С����İ�Χ����
		int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
		int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
		theObject[0] = xpos, theObject[1] = ypos;
		int x = theObject[0];
		int y = theObject[1];

		cvCircle(cameraFeed, cvPoint(x, y), 10, cvScalar(0, 255, 0), 2);
		cvLine(cameraFeed, cvPoint(x, y), cvPoint(x, y - 25), cvScalar(0, 255, 0), 2);
		cvLine(cameraFeed, cvPoint(x, y), cvPoint(x, y + 25), cvScalar(0, 255, 0), 2);
		cvLine(cameraFeed, cvPoint(x, y), cvPoint(x - 25, y), cvScalar(0, 255, 0), 2);
		cvLine(cameraFeed, cvPoint(x, y), cvPoint(x + 25, y), cvScalar(0, 255, 0), 2);
		char showText[30];
		sprintf(showText, "X;%d, Y:%d", x, y);
		cvPutText(cameraFeed, showText, cvPoint(x, y + 30), &font, cvScalar(255, 0, 0));
	}
}

int main()
{
	
	double time;
	CvScalar hsv_min;
	CvScalar hsv_max;

#ifdef VIDEO
	CvCapture* capture = cvCreateFileCapture("E:\\Evan Lin\\CLUB\\PingPong\\OPENCV\\Table Tennis.flv");
	//CvCapture *capture = cvCreateFileCapture("E:\\Evan Lin\\CLUB\\PingPong\\OPENCV\\Test\\motionTracking(ZC)\\bouncingBall.avi");
	//CvCapture *capture = cvCreateFileCapture("E:\\Evan Lin\\CLUB\\PingPong\\OPENCV\\PingPongTest.avi");
	//CvCapture *capture = cvCreateFileCapture("E:\\Evan Lin\\CLUB\\PingPong\\OPENCV\\WIN_20150803_103818.MP4");//WIN_20150803_102356.MP4
#else
	CvCapture* capture = cvCreateCameraCapture(0);
#endif
	if (!capture)
	{
		printf("ERROR ACQUIRING VIDEO FEED\n");
		getchar();
		return -1;
	}

	CvSize size = cvSize(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH), cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	IplImage* frame = cvCreateImage(size, IPL_DEPTH_8U, 3);
	IplImage* hsv_frame = cvCreateImage(size, IPL_DEPTH_8U, 3);
	IplImage* thresholded = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0, 1, 1);


	while (1)
	{
#ifdef VIDEO

		capture = cvCreateFileCapture("E:\\Evan Lin\\CLUB\\PingPong\\OPENCV\\Table Tennis.flv");
		if (!capture)
		{
			printf("ERROR ACQUIRING VIDEO FEED\n");
			getchar();
			return -1;
		}
		while (cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES) < cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT) - 1)
		{
			time = (double)cvGetTickCount();
			frame = cvQueryFrame(capture);
			if (!frame)
			{
				fprintf(stderr, "ERROR: frame is null...\n");
				getchar();
				exit(1);
			}
			cvCvtColor(frame, hsv_frame, CV_BGR2HSV);
			hsv_min = cvScalar(H_MIN, S_MIN, S_MIN, 0);
			hsv_max = cvScalar(H_MAX, S_MAX, V_MAX, 0);
			cvInRangeS(hsv_frame, hsv_min, hsv_max, thresholded);
			cvErode(thresholded, thresholded, NULL, 1);
			cvDilate(thresholded, thresholded, NULL, 1);
			//cvSmooth(thresholded, thresholded, CV_GAUSSIAN, 9, 9);

			if (debugMode == true)
			{
				createTrackbars();
				cvNamedWindow("HSV", CV_WINDOW_AUTOSIZE);
				cvShowImage("HSV", thresholded);
			}
			else
			{
				cvDestroyWindow("trackbarControl");
				cvDestroyWindow("HSV");
			}

			if (trackingEnabled)
			{
				searchForMovement(thresholded, frame);
			}

			if (showEnabled)
			{
				time = ((double)cvGetTickCount() - time) / cvGetTickFrequency();
				double fps = 100000.0 / time;
				char str[20];
				sprintf(str, "FPS:%.2f", fps);
				cvPutText(frame, str, cvPoint(0, 30), &font, cvScalar(255, 0, 0));
			}

			cvShowImage("Frame1", frame);

			switch (cvWaitKey(33))
			{
			case 27: //'esc' key has been pressed, exit program.
				//cvReleaseMemStorage();
				cvReleaseCapture(&capture);
				cvDestroyAllWindows();
				return 0;
			case 116: //'t' has been pressed. this will toggle tracking
				trackingEnabled = !trackingEnabled;
				if (trackingEnabled == false) printf("Tracking disabled.\n");
				else printf("Tracking enabled.\n");
				break;
			case 100: //'d' has been pressed. this will debug mode
				debugMode = !debugMode;
				if (debugMode == false) printf("Debug mode disabled.\n");
				else printf("Debug mode enabled.\n");
				break;
			case 115: //'s' has been pressed. this will debug mode
				showEnabled = !showEnabled;
				if (showEnabled == false) printf("Show mode disabled.\n");
				else printf("Show mode enabled.\n");
				break;
			case 112: //'p' has been pressed. this will pause/resume the code.
				pause = !pause;
				if (pause == true)
				{
					printf("Code paused, press 'p' again to resume\n");
					while (pause == true)
					{
						//stay in this loop until 
						switch (cvWaitKey())
						{
						case 112:
							//change pause back to false
							pause = false;
							printf("Code resumed.\n");
							break;
						}
					}
				}
			}
		}
	}
#else
		time = (double)GetTickCount();
		//time = (double)cvGetTickCount();
		frame = cvQueryFrame(capture);
		if (!frame)
		{
			fprintf(stderr, "ERROR: frame is null...\n");
			getchar();
			exit(1);
		}
		cvCvtColor(frame, hsv_frame, CV_BGR2HSV);
		hsv_min = cvScalar(H_MIN, S_MIN, S_MIN, 0);
		hsv_max = cvScalar(H_MAX, S_MAX, V_MAX, 0);
		cvInRangeS(hsv_frame, hsv_min, hsv_max, thresholded);
		cvErode(thresholded, thresholded, NULL, 1);
		cvErode(thresholded, thresholded, NULL, 1);
		cvErode(thresholded, thresholded, NULL, 1);
		cvDilate(thresholded, thresholded, NULL, 1);
		//cvSmooth(thresholded, thresholded, CV_GAUSSIAN, 9, 9);

		if (debugMode == true)
		{
			createTrackbars();
			cvNamedWindow("HSV", CV_WINDOW_AUTOSIZE);
			cvShowImage("HSV", thresholded);
		}
		else
		{
			cvDestroyWindow("trackbarControl");
			cvDestroyWindow("HSV");
		}

		if (trackingEnabled)
		{
			searchForMovement(thresholded, frame);
		}

		if (showEnabled)
		{
			//time = ((double)cvGetTickCount() - time) / cvGetTickFrequency();
			//double fps = 100000.0 / time;
			time = (double)GetTickCount() - time;
			double fps = 1000.0 / time;
			char str[20];
			sprintf(str, "FPS:%.2f", fps);
			cvPutText(frame, str, cvPoint(0, 30), &font, cvScalar(255, 0, 0));
		}

		cvShowImage("Frame1", frame);

		
		}
}
#endif
	//cvReleaseImage(&frame1);
	//cvReleaseImage(&frame2);
	//cvReleaseImage(&grayImage1);
	//cvReleaseImage(&grayImage2);
	//cvReleaseImage(&differenceImage);
	//cvReleaseImage(&thresholdImage);
	return 0;
}

void on_trackbar(int pos)
{
	;
}

void createTrackbars()
{
	cvNamedWindow("trackbarControl", 0);
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);

	cvCreateTrackbar("H_MIN", "trackbarControl", &H_MIN, 255, on_trackbar);
	cvCreateTrackbar("H_MAX", "trackbarControl", &H_MAX, 255, on_trackbar);
	cvCreateTrackbar("S_MIN", "trackbarControl", &S_MIN, 255, on_trackbar);
	cvCreateTrackbar("S_MAX", "trackbarControl", &S_MAX, 255, on_trackbar);
	cvCreateTrackbar("V_MIN", "trackbarControl", &V_MIN, 255, on_trackbar);
	cvCreateTrackbar("V_MAX", "trackbarControl", &V_MAX, 255, on_trackbar);
}




///**************�����Ҳ�궨���ص�����*************/
////if(Wait4StrIs(COMMUNICATE_COM_R,Calstr) != 1) 
//printf("CALIB ERROR");
//#ifdef AUTO_CALIB
//CvRect furtherROIR[CORNER_NUMR];
//float image_pointR[CORNER_NUMR * 2];
//for (int i = 0; i < CORNER_NUMR; i++)
//{
//	sprintf(loadDis, "matchR\\%d.BMP", i + 1);
//	temp = cvLoadImage(loadDis, 1);
//	furtherROIR[i] = findBlock(CalibrateImg, temp, &ChoseRect(i + 1), &ChosePoint(i + 1), &findPoint, 0);
//	cvSetImageROI(CalibrateImg, furtherROIR[i]);
//	IplImage *largeImg = cvCreateImage(cvSize(11 * furtherROIR[i].width, 11 * furtherROIR[i].height), CalibrateImg->depth, CalibrateImg->nChannels);
//	cvResize(CalibrateImg, largeImg);
//	cvResetImageROI(CalibrateImg);
//	CvPoint pt = MouseClick(largeImg);
//	image_pointR[2 * i] = furtherROIR[i].x + ((float)pt.x) / 7.0;
//	image_pointR[2 * i + 1] = furtherROIR[i].y + ((float)pt.y) / 7.0;
//	cvReleaseImage(&largeImg);
//}
//#else
//float image_pointR[CORNER_NUMR * 2] =
//{
//	202, 849,
//	534, 875,
//	1009, 910,
//	495, 774,
//	729, 781,
//	1010.5, 786.5
//};
//#endif





//BOOL CreateDisplayWindow()
//{
//	WNDCLASSEX wcx;
//	HMODULE hInstance = GetModuleHandle(NULL);
//	if (GetClassInfoEx(hInstance, CONSOLEDISPCLASS, &wcx) == 0)
//	{
//		// Fill in the window class structure with parameters that describe the main window. 
//		wcx.cbSize = sizeof(wcx);          // size of structure 
//		wcx.style = CS_HREDRAW | CS_NOCLOSE | CS_SAVEBITS | CS_VREDRAW | WS_OVERLAPPED;
//		wcx.lpfnWndProc = ConsoleDispWndProc;   // points to window procedure 
//		wcx.cbClsExtra = 0;                    // no extra class memory 
//		wcx.cbWndExtra = 0;                    // no extra window memory 
//		wcx.hInstance = hInstance;            // handle to instance 
//		wcx.hIcon = NULL;                 // no icon
//		wcx.hCursor = NULL;                 // no cursor
//		wcx.lpszMenuName = NULL;                 // name of menu resource 
//		wcx.lpszClassName = CONSOLEDISPCLASS;     // name of window class 
//		wcx.hIconSm = NULL;                 // small class icon 
//		wcx.hbrBackground = NULL;
//
//		// Register the window class. 
//		m_atom = RegisterClassEx(&wcx);
//
//		if (m_atom != NULL)
//		{
//			// create our display window
//			m_hwndDisp = CreateWindow(CONSOLEDISPCLASS,        // name of window class 
//				"Camera display window",  // title-bar string 
//				WS_OVERLAPPEDWINDOW,      // top-level window 
//				CW_USEDEFAULT,            // default horizontal position 
//				CW_USEDEFAULT,            // default vertical position 
//				m_nDispSizeX,             // default width 
//				m_nDispSizeY,             // default height 
//				(HWND)NULL,              // no owner window 
//				(HMENU)NULL,             // use class menu 
//				hInstance,                // handle to application instance 
//				(LPVOID)NULL);            // no window-creation data 
//
//			if (!m_hwndDisp)
//			{
//				DWORD a = GetLastError();
//				LPVOID lpMsgBuf;
//				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
//					FORMAT_MESSAGE_FROM_SYSTEM |
//					FORMAT_MESSAGE_IGNORE_INSERTS,
//					NULL, a,
//					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
//					(LPTSTR)&lpMsgBuf, 0, NULL);
//				// Free the buffer.
//				LocalFree(lpMsgBuf);
//				::UnregisterClass(CONSOLEDISPCLASS, hInstance);
//				m_atom = NULL;
//			}
//		}
//	}
//	return (m_atom != NULL);
//}
//
//BOOL DeleteDisplayWindow()
//{
//	BOOL boRet = FALSE;
//
//	if (m_atom != NULL)
//	{
//		HMODULE hInstance = GetModuleHandle(NULL);
//
//		SendMessage(m_hwndDisp, WM_CLOSE, 0, 0);
//		SendMessage(m_hwndDisp, WM_DESTROY, 0, 0);
//
//		UnregisterClass(CONSOLEDISPCLASS, hInstance);
//
//		m_atom = NULL;
//		m_hwndDisp = NULL;
//	}
//
//	return boRet;
//}
//LRESULT WINAPI ConsoleDispWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	PAINTSTRUCT ps;
//	RECT rc;
//
//	switch (msg)
//	{
//	case IS_UEYE_MESSAGE:
//		ProcessuEyeMessages(msg, wParam, lParam);
//		return 0;
//
//	case WM_CREATE:
//		// Initialize the window. 
//		return 0;
//
//	case WM_ERASEBKGND:
//		// we need to refresh the background only if live or
//		// display mode is off
//		if (!m_boLive || !m_boDisplay)
//		{
//			if (GetUpdateRect(hwnd, &rc, FALSE))
//			{
//				HDC hdc = BeginPaint(hwnd, &ps);
//				FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
//				EndPaint(hwnd, &ps);
//			}
//		}
//		return 1;
//
//	default:
//		return DefWindowProc(hwnd, msg, wParam, lParam);
//	}
//	return 0;
//}
