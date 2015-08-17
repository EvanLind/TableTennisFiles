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
		pDevice->GetImageSize(&iWidth, &iHeight);//��ȡ����豸���ͼ��Χ��С 
		BYTE *pRGB24Buff = new BYTE[iWidth * iHeight * 3];//����ͼ�����ݵĻ�����
		DeviceStatus devStatus = STATUS_OK;
		IplImage *iplImage = NULL;
		cvNamedWindow("CGiamgetech");
		while (true)
		{
			int t_Pre = GetTickCount();
			pDevice->SoftTriggerOnce(); //����//������ʼ�ع�
			//devStatus = pDevice->DeviceGetImageBuffer(&pRGB24Buff, -1, &dfInfo);
			//pRGB24Buff = pDevice->DeviceGetImageBufferEx(-1, &dfInfo);
			devStatus = pDevice->DeviceGetImageBufferEx2(pRGB24Buff, -1, &dfInfo);//���һ֡ͼ������(�ýӿڻ�õ�ͼ���Ǿ���������ISP���ݸ�ʽ)//��Ҫ��ʱ�ĵط�������ȴ��ع���ϣ���
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
	IDevice *LeftDevice = NULL;//ָ������豸�����ָ��
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
