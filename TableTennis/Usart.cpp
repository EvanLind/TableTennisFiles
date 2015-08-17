#include "Usart.h"
#include "stdafx.h"


void Serial_Port_Init(char *_portname, int _rate);//Serial_Port_Init("\\\\.\\COM14", 57600);
void ReceiveChar_api(char com[], char *RevBuff);
int Wait4StrIs(char com[], char string[]);
void WriteChar_api(char com[], char string[]);
//HANDLE hCom1;
//OVERLAPPED m_ov1;
//COMSTAT comstat1;
//
//HANDLE hCom2;
//OVERLAPPED m_ov2;
//COMSTAT comstat2;
//
//char MODEL_CHAR1[20];
//char MODEL_CHAR2[20];
//static int ComCnt = 0;
////portname大于COM10时需要这样传参 \\\\.\\COM74
//void Serial_Port_Init(char *_portname, int _rate)
//{
//	ComCnt++;
//	if (ComCnt == 1)
//	{
//		strcpy(MODEL_CHAR1, _portname);
//		hCom1 = CreateFile(_portname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
//		if (hCom1 == INVALID_HANDLE_VALUE)
//			printf("open comport error\n");
//		DCB dcb;
//		int rate = _rate;
//		memset(&dcb, 0, sizeof(dcb));
//		if (!GetCommState(hCom1, &dcb)) //获取当前DCB配置
//		{
//			printf("open set error\n");
//		}
//		dcb.DCBlength = sizeof(dcb);
//		dcb.BaudRate = rate;
//		dcb.Parity = NOPARITY;
//		dcb.fParity = 0;
//		dcb.StopBits = ONESTOPBIT;
//		dcb.ByteSize = 8;
//		dcb.fOutxCtsFlow = 0;
//		dcb.fOutxDsrFlow = 0;
//		dcb.fDtrControl = DTR_CONTROL_DISABLE;
//		dcb.fDsrSensitivity = 0;
//		dcb.fRtsControl = RTS_CONTROL_DISABLE;
//		dcb.fOutX = 0;
//		dcb.fInX = 0;
//		dcb.fErrorChar = 0;
//		dcb.fBinary = 1;
//		dcb.fNull = 0;
//		dcb.fAbortOnError = 0;
//		dcb.wReserved = 0;
//		dcb.XonLim = 2;
//		dcb.XoffLim = 4;
//		dcb.XonChar = 0x13;
//		dcb.XoffChar = 0x19;
//		dcb.EvtChar = 0;
//		if (!SetCommState(hCom1, &dcb))
//		{
//			printf("setupDCB error\n");
//		}
//		COMMTIMEOUTS timeouts;
//		timeouts.ReadIntervalTimeout = 0;
//		timeouts.ReadTotalTimeoutConstant = 0;
//		timeouts.ReadTotalTimeoutMultiplier = 0;
//		timeouts.WriteTotalTimeoutConstant = 0;
//		timeouts.WriteTotalTimeoutMultiplier = 0;
//		if (!SetCommTimeouts(hCom1, &timeouts))
//		{
//			printf("setuptimeout error\n");
//		}
//		PurgeComm(hCom1, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
//	}
//	else if (ComCnt == 2)
//	{
//		strcpy(MODEL_CHAR2, _portname);
//		hCom2 = CreateFile(_portname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
//		if (hCom2 == INVALID_HANDLE_VALUE)
//			printf("open comport error\n");
//		DCB dcb;
//		int rate = _rate;
//		memset(&dcb, 0, sizeof(dcb));
//		if (!GetCommState(hCom2, &dcb)) //获取当前DCB配置
//		{
//			printf("open set error\n");
//		}
//		dcb.DCBlength = sizeof(dcb);
//		dcb.BaudRate = rate;
//		dcb.Parity = NOPARITY;
//		dcb.fParity = 0;
//		dcb.StopBits = ONESTOPBIT;
//		dcb.ByteSize = 8;
//		dcb.fOutxCtsFlow = 0;
//		dcb.fOutxDsrFlow = 0;
//		dcb.fDtrControl = DTR_CONTROL_DISABLE;
//		dcb.fDsrSensitivity = 0;
//		dcb.fRtsControl = RTS_CONTROL_DISABLE;
//		dcb.fOutX = 0;
//		dcb.fInX = 0;
//		dcb.fErrorChar = 0;
//		dcb.fBinary = 1;
//		dcb.fNull = 0;
//		dcb.fAbortOnError = 0;
//		dcb.wReserved = 0;
//		dcb.XonLim = 2;
//		dcb.XoffLim = 4;
//		dcb.XonChar = 0x13;
//		dcb.XoffChar = 0x19;
//		dcb.EvtChar = 0;
//		if (!SetCommState(hCom2, &dcb))
//		{
//			printf("setupDCB error\n");
//		}
//		COMMTIMEOUTS timeouts;
//		timeouts.ReadIntervalTimeout = 0;
//		timeouts.ReadTotalTimeoutConstant = 0;
//		timeouts.ReadTotalTimeoutMultiplier = 0;
//		timeouts.WriteTotalTimeoutConstant = 0;
//		timeouts.WriteTotalTimeoutMultiplier = 0;
//		if (!SetCommTimeouts(hCom2, &timeouts))
//		{
//			printf("setuptimeout error\n");
//		}
//		PurgeComm(hCom2, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
//	}
//}
//int Wait4StrIs(char com[], char string[])
//{
//	if (strcmp(MODEL_CHAR1, com) == 0)
//	{
//		BOOL bRead = TRUE;
//		BOOL bResult = TRUE;
//		DWORD dwError = 0;
//		DWORD BytesRead = 0;
//		char RXBuff;
//		int Length = -1;
//		for (;;)
//		{
//			bResult = ClearCommError(hCom1, &dwError, &comstat1);
//			if (comstat1.cbInQue == 0)
//			{
//				continue;
//			}
//			bResult = ReadFile(hCom1,      // Handle to COMM port
//				&RXBuff,         // RX Buffer Pointer
//				1,                   // Read one byte
//				&BytesRead,          // Stores number of bytes read
//				&m_ov1);          // pointer to the m_ov1 structure
//
//			if (RXBuff == '#')
//				break;
//			if (Length != -1)
//			{
//				string[Length++] = RXBuff;
//			}
//			if (RXBuff == '*')
//				Length = 0;
//			//printf("%c", RXBuff);
//		}
//		return Length;
//	}
//	else if (strcmp(MODEL_CHAR2, com) == 0)
//	{
//		BOOL bRead = TRUE;
//		BOOL bResult = TRUE;
//		DWORD dwError = 0;
//		DWORD BytesRead = 0;
//		char RXBuff;
//		int Length = -1;
//		for (;;)
//		{
//			bResult = ClearCommError(hCom2, &dwError, &comstat2);
//			if (comstat2.cbInQue == 0)
//			{
//				continue;
//			}
//			bResult = ReadFile(hCom2,      // Handle to COMM port
//				&RXBuff,         // RX Buffer Pointer
//				1,                   // Read one byte
//				&BytesRead,          // Stores number of bytes read
//				&m_ov2);          // pointer to the m_ov1 structure
//
//			if (RXBuff == '#')
//				break;
//			if (Length != -1)
//			{
//				string[Length++] = RXBuff;
//			}
//			if (RXBuff == '*')
//				Length = 0;
//			//printf("%c", RXBuff);
//		}
//		return Length;
//	}
//}
//
//void ReceiveChar_api(char com[], char *RevBuff)
//{
//	if (strcmp(MODEL_CHAR1, com) == 0)
//	{
//		BOOL bRead = TRUE;
//		BOOL bResult = TRUE;
//		BOOL sucFlag = TRUE;
//		DWORD dwError = 0;
//		DWORD BytesRead = 0;
//		char RXBuff;
//		int _Length = 0;
//		for (;;)
//		{
//			bResult = ClearCommError(hCom1, &dwError, &comstat1);
//			if (comstat1.cbInQue == 0)
//			{
//				sucFlag = TRUE;
//				continue;
//			}
//			if (bRead)
//			{
//				bResult = ReadFile(hCom1,      // Handle to COMM port
//					&RXBuff,         // RX Buffer Pointer
//					1,                   // Read one byte
//					&BytesRead,          // Stores number of bytes read
//					&m_ov1);          // pointer to the m_ov1 structure
//				printf("%c", RXBuff);
//				if (!bResult)
//				{
//					switch (dwError = GetLastError())
//					{
//					case ERROR_IO_PENDING:
//					{
//						bRead = FALSE;
//						break;
//					}
//					default:
//					{
//						break;
//					}
//					}
//				}
//				else
//				{
//					bRead = TRUE;
//				}
//			} // close if (bRead)
//			if (!bRead)
//			{
//				bRead = TRUE;
//				bResult = GetOverlappedResult(hCom1,  // Handle to COMM port
//					&m_ov1,  // Overlapped structure
//					&BytesRead,     // Stores number of bytes read
//					TRUE);          // Wait flag
//				if (sucFlag)
//				{
//					sucFlag = FALSE;
//					ZeroMemory(RevBuff, _Length);
//					_Length = 0;
//				}
//				_Length += 1;
//				*RevBuff = RXBuff;
//				RevBuff += 1;
//			}
//		}
//	}
//	else if (strcmp(MODEL_CHAR2, com) == 0)
//	{
//		BOOL bRead = TRUE;
//		BOOL bResult = TRUE;
//		BOOL sucFlag = TRUE;
//		DWORD dwError = 0;
//		DWORD BytesRead = 0;
//		char RXBuff;
//		int _Length = 0;
//		for (;;)
//		{
//			bResult = ClearCommError(hCom2, &dwError, &comstat2);
//			if (comstat2.cbInQue == 0)
//			{
//				sucFlag = TRUE;
//				continue;
//			}
//			if (bRead)
//			{
//				bResult = ReadFile(hCom2,      // Handle to COMM port
//					&RXBuff,         // RX Buffer Pointer
//					1,                   // Read one byte
//					&BytesRead,          // Stores number of bytes read
//					&m_ov2);          // pointer to the m_ov2 structure
//				printf("%c", RXBuff);
//				if (!bResult)
//				{
//					switch (dwError = GetLastError())
//					{
//					case ERROR_IO_PENDING:
//					{
//						bRead = FALSE;
//						break;
//					}
//					default:
//					{
//						break;
//					}
//					}
//				}
//				else
//				{
//					bRead = TRUE;
//				}
//			} // close if (bRead)
//			if (!bRead)
//			{
//				bRead = TRUE;
//				bResult = GetOverlappedResult(hCom2,  // Handle to COMM port
//					&m_ov2,  // Overlapped structure
//					&BytesRead,     // Stores number of bytes read
//					TRUE);          // Wait flag
//				if (sucFlag)
//				{
//					sucFlag = FALSE;
//					ZeroMemory(RevBuff, _Length);
//					_Length = 0;
//				}
//				_Length += 1;
//				*RevBuff = RXBuff;
//				RevBuff += 1;
//			}
//		}
//	}
//}
//
//
//// void WriteChar_api(char *m_szWriteBuffer, DWORD m_nToSend)
//void WriteChar_api(char com[], char string[])
//{
//	if (strcmp(MODEL_CHAR1, com) == 0)
//	{
//		char * m_szWriteBuffer = string;
//		DWORD m_nToSend = strlen(string);
//		BOOL bWrite = TRUE;
//		BOOL bResult = TRUE;
//		DWORD BytesSent = 0;
//		//HANDLE m_hWriteEven;
//		//ResetEvent(m_hWriteEven);
//		if (bWrite)
//		{
//			m_ov1.Offset = 0;
//			m_ov1.OffsetHigh = 0;
//			// Clear buffer
//			bResult = WriteFile(hCom1, // Handle to COMM Port
//				m_szWriteBuffer, // Pointer to message buffer in calling finction
//				m_nToSend,      // Length of message to send
//				&BytesSent,     // Where to store the number of bytes sent
//				&m_ov1);        // Overlapped structure
//
//			if (!bResult)
//			{
//				DWORD dwError = GetLastError();
//				switch (dwError)
//				{
//				case ERROR_IO_PENDING:
//				{
//					// continue to GetOverlappedResults()
//					BytesSent = 0;
//					bWrite = FALSE;
//					break;
//				}
//				default:
//				{
//					// all other error codes
//					break;
//				}
//				}
//			}
//		} // end if(bWrite)
//		if (!bWrite)
//		{
//			bWrite = TRUE;
//			bResult = GetOverlappedResult(hCom1,   // Handle to COMM port
//				&m_ov1,     // Overlapped structure
//				&BytesSent,    // Stores number of bytes sent
//				TRUE);         // Wait flag
//			// deal with the error code
//			if (!bResult)
//			{
//				printf("GetOverlappedResults() in WriteFile()");
//			}
//		} // end if (!bWrite)
//
//		// Verify that the data size send equals what we tried to send
//		if (BytesSent != m_nToSend)
//		{
//			printf("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char *)m_szWriteBuffer));
//		}
//	}
//	else if (strcmp(MODEL_CHAR2, com) == 0)
//	{
//		char * m_szWriteBuffer = string;
//		DWORD m_nToSend = strlen(string);
//		BOOL bWrite = TRUE;
//		BOOL bResult = TRUE;
//		DWORD BytesSent = 0;
//		//HANDLE m_hWriteEven;
//		//ResetEvent(m_hWriteEven);
//		if (bWrite)
//		{
//			m_ov2.Offset = 0;
//			m_ov2.OffsetHigh = 0;
//			// Clear buffer
//			bResult = WriteFile(hCom2, // Handle to COMM Port
//				m_szWriteBuffer, // Pointer to message buffer in calling finction
//				m_nToSend,      // Length of message to send
//				&BytesSent,     // Where to store the number of bytes sent
//				&m_ov2);        // Overlapped structure
//
//			if (!bResult)
//			{
//				DWORD dwError = GetLastError();
//				switch (dwError)
//				{
//				case ERROR_IO_PENDING:
//				{
//					// continue to GetOverlappedResults()
//					BytesSent = 0;
//					bWrite = FALSE;
//					break;
//				}
//				default:
//				{
//					// all other error codes
//					break;
//				}
//				}
//			}
//		} // end if(bWrite)
//		if (!bWrite)
//		{
//			bWrite = TRUE;
//			bResult = GetOverlappedResult(hCom2,   // Handle to COMM port
//				&m_ov2,     // Overlapped structure
//				&BytesSent,    // Stores number of bytes sent
//				TRUE);         // Wait flag
//			// deal with the error code
//			if (!bResult)
//			{
//				printf("GetOverlappedResults() in WriteFile()");
//			}
//		} // end if (!bWrite)
//
//		// Verify that the data size send equals what we tried to send
//		if (BytesSent != m_nToSend)
//		{
//			printf("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char *)m_szWriteBuffer));
//		}
//	}
//}