#include "stdafx.h"
#include "windows.h"
#include <process.h>
#include <opencv2/opencv.hpp>
#include <IDevice.h>


#define TEST

DeviceStatus CameraInit(IDevice **pDevice, UCHAR devIndex);

BOOL bExit = FALSE;


					
unsigned int __stdcall CaptureThread(LPVOID lpParameter)
{
	IDevice *pDevice = (IDevice *)lpParameter;
	if(pDevice != NULL)
	{
		DeviceFrameInfo dfInfo;
		INT iWidth, iHeight;
		pDevice->GetImageSize(&iWidth, &iHeight);//获取相机设备最大图像范围大小 
		BYTE *pRGB24Buff = new BYTE[iWidth * iHeight * 3];//接收图像数据的缓冲区
		DeviceStatus devStatus = STATUS_OK;
		IplImage *iplImage = NULL;
		cvNamedWindow("CGiamgetech");
		while (true)
		{
			int t_Pre = GetTickCount();
			pDevice->SoftTriggerOnce(); //软触发//触发后开始曝光
			//devStatus = pDevice->DeviceGetImageBuffer(&pRGB24Buff, -1, &dfInfo);
			//pRGB24Buff = pDevice->DeviceGetImageBufferEx(-1, &dfInfo);
			devStatus = pDevice->DeviceGetImageBufferEx2(pRGB24Buff, -1, &dfInfo);//获得一帧图像数据(该接口获得的图像是经过处理后的ISP数据格式)//主要费时的地方！！需等待曝光完毕！！
			int t_Aft = GetTickCount();

			/*USHORT pusExposureTime = 0;
			pDevice->GetExposureTime(&pusExposureTime);
			printf("pusExposureTime:%dms\n", pusExposureTime);*/

			/*if ((t_Aft - t_Pre) == 0)
			{
				printf("devStatus:%d\n", devStatus);
			}*/
			if (!devStatus)
			{
				//printf("time:%dms\n", t_Aft - t_Pre);
				iplImage =  cvCreateImageHeader(cvSize(dfInfo.uiWidth, dfInfo.uiHeight),IPL_DEPTH_8U, 3);
				cvSetData(iplImage, pRGB24Buff, dfInfo.uiWidth*3);
				cvShowImage("CGiamgetech", iplImage);
				cvReleaseImageHeader(&iplImage);
			}
			int c = cvWaitKey(1);
			if (c == 'q' || c == 'Q' || (c & 255) == 27)
			{
				bExit = TRUE;
				cvReleaseImage(&iplImage);
				cvDestroyAllWindows();
				break;
			}
		}
		delete []pRGB24Buff;
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	IDevice *LeftDevice = NULL;//指向相机设备句柄的指针
	HANDLE LThread = NULL;
	unsigned LThreadID;
	CameraInit(&LeftDevice, 0);
	Sleep(1000);
	LThread = (HANDLE)_beginthreadex(NULL, 0, CaptureThread, LeftDevice, 0, &LThreadID);
	while(!bExit)
	{
		Sleep(1);
	}
	if (LThreadID != NULL)
	{
		CloseHandle(LThread);
	}
	if (LeftDevice != NULL)
	{
		LeftDevice->Stop();
		LeftDevice->CloseDevice();
		LeftDevice->DeviceUnInit();
		LeftDevice->Release();
	}
	IDevice::DeviceUnInitialSDK();
	return 0;
}

/*------------------------------------------------------------------------*\
| 功  能：配置相机参数
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：
|         1  IDevice			**pDevice		设备接口			 输出参数
|         2  UCHAR				dev				设备索引序列(0-n)	 输入参数
\*------------------------------------------------------------------------*/
DeviceStatus CameraInit(IDevice **pDevice, UCHAR devIndex)
{
	INT nDevice = 0;
	DeviceStatus devStatus = STATUS_OK;
	if (SUCCEEDED(IDevice::DeviceInitialSDK(NULL, FALSE)))//SDK初始化
	{
		if (SUCCEEDED(IDevice::EnumDevice(NULL, &nDevice)) && nDevice > 0)//numDevice枚举设备，获得当前设备信息
		{
			UionOpenDeviceParam deParam;
			deParam.devIndex = devIndex;
			if (SUCCEEDED(IDevice::OpenDevice(deParam, pDevice, OF_BYPOSITION)))//打开指定的相机
			{
				if (SUCCEEDED((*pDevice)->DeviceInit(NULL, TRUE, FALSE)))//根据相机设备枚举信息初始化相机
				{
					//devStatus = pDevice->DeviceCreateSettingPage(NULL);//创建相机的属性配置窗口
					//printf("CreateSettingPage the camera! code is %d\n", devStatus);
					//devStatus = pDevice->DeviceShowSettingPage(TRUE);//显示相机配置窗口
					//printf("ShowSettingPage the camera! code is %d\n", devStatus);
					//devStatus = pDevice->DeviceSetISPDataType(DATA_ISP_RGB24);//设置图像数据处理输出类型RGB8位深度

					//INT devHeight, devWidth;
					//(*pDevice)->GetCapabilityImageSize(&devHeight, &devWidth);
					//DeviceROISize devSize;
					//devSize.iWidth = 640;
					//devSize.iHeight = 480;
					//devSize.iHOffset = (devHeight - devSize.iHeight) / 2;
					//devSize.iVOffset = (devWidth - devSize.iWidth)/ 2;
					//ResolutionParam reParam = {{devSize, 0}, RT_ROI };
					//devStatus = (*pDevice)->SetResolution(reParam);//设置相机当前分辨率
					

					ResolutionParam reParam;
					reParam.type = RT_PRESET;
					reParam.dri.ucIndex = 6;//分辨率索引值(2592x1944索引值为0)
					devStatus = (*pDevice)->SetResolution(reParam);//设置相机当前分辨率

					devStatus = (*pDevice)->SetMediaType(DATA_TYPE_BAYGR8);//设置输出图像数据的类型
					devStatus = (*pDevice)->SetFrameSpeed(HIGHEST_SPEED);//设置HIGHEST_SPEED最高帧率
					devStatus = (*pDevice)->SetFrameSpeedTune(0.9);
					devStatus = (*pDevice)->SetCaptureMode(SOFT_TRIGGER_MODE);//设置触发模式
					devStatus = (*pDevice)->SetSoftTriggerFrameNumber(1);//设置软触发一次帧数
					(*pDevice)->SetAutoExposureState(false);//设置自动曝光状态
					devStatus = (*pDevice)->SetExposureTime(10);//设置曝光时间
					//(*pDevice)->SetAutoExposureState(true);//设置自动曝光状态
					//devStatus = (*pDevice)->SetAutoExposureTarget(10);//设置曝光时间
					devStatus = (*pDevice)->Start();//打开视频流，SDK 进入图像采集模式

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


///**************计算右侧标定像素点坐标*************/
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
