//--------------------------------------------------------------------------------------
// HttpClient.h
//
// XNA Developer Connection.
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#ifndef HttpClient_H
#define HttpClient_H

#include <xtl.h>
#include <winsockx.h>

//-----------------------------------------------------------------------------
// Internal buffer length
//-----------------------------------------------------------------------------
#define HTTP_HOST_IP_STRING_LENGTH  128
#define INIT_BUFFER_SIZE            512
#define TCP_RECV_BUFFER_SIZE        512
#define HTTP_COMMAND_BUFFER_SIZE    512


//--------------------------------------------------------------------------------------
// Name: class MemoryBuffer
// Desc: Memory buffer, automatically expands as needed to hold more data
//--------------------------------------------------------------------------------------
class MemoryBuffer_socket
{
public:

	MemoryBuffer_socket(DWORD dwSize = INIT_BUFFER_SIZE)
	{
		m_pBuffer = NULL;
		m_dwDataLength = 0;
		m_dwBufferSize = 0;

		if ((dwSize < UINT_MAX) && (dwSize != 0))
		{
			m_pBuffer = (BYTE*)malloc(dwSize + 1);    // one more char, in case when using string funcions
			if (m_pBuffer)
			{
				m_dwBufferSize = dwSize;
				m_pBuffer[0] = 0;
			}
		}
	};

	~MemoryBuffer_socket()
	{
		if (m_pBuffer)
			free(m_pBuffer);

		m_pBuffer = NULL;
		m_dwDataLength = 0;
		m_dwBufferSize = 0;
	};

	// Add chunk of memory to buffer
	BOOL    Add(const void* p, DWORD dwSize)
	{
		if (CheckSize(dwSize))
		{
			memcpy(m_pBuffer + m_dwDataLength, p, dwSize);
			m_dwDataLength += dwSize;
			*(m_pBuffer + m_dwDataLength) = 0;    // fill end zero
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	};

	// Get the data in buffer
	BYTE* GetData() const
	{
		return m_pBuffer;
	};

	// Get the lenght of data in buffer
	DWORD   GetDataLength() const
	{
		return m_dwDataLength;
	};

	// Rewind the data pointer to the begining
	void    Rewind()
	{
		m_dwDataLength = 0; m_pBuffer[0] = 0;
	};

private:

	BYTE * m_pBuffer;

	DWORD m_dwDataLength;

	DWORD m_dwBufferSize;

	// Automatically adjust increase buffer size if necessary
	BOOL    CheckSize(DWORD dwSize)
	{
		if (m_dwBufferSize >= (m_dwDataLength + dwSize))
		{
			return TRUE;    // Enough space
		}
		else
		{
			// Try to double it
			DWORD dwNewSize = max(m_dwDataLength + dwSize, m_dwBufferSize * 2);


			BYTE* pNewBuffer = (BYTE*)malloc(dwNewSize + 1);
			memset(pNewBuffer, 0, (dwNewSize + 1));
			memcpy(pNewBuffer, m_pBuffer, m_dwBufferSize);
			free(m_pBuffer);


			//BYTE* pNewBuffer = (UCHAR*)realloc(m_pBuffer, dwNewSize + 1);        // one more char
			if (pNewBuffer)
			{
				m_pBuffer = pNewBuffer;
				m_dwBufferSize = dwNewSize;

				return TRUE;
			}
			else
			{
				// Failed
				return FALSE;
			}
		}
	}
};

class memoryAlloc
{
public:

	void ClearAndAlloc();

	void ClearBuffer();
	memoryAlloc(unsigned int Size = 1024);
	~memoryAlloc();

	unsigned char* GetData() {

		return AllocatedData;
	}

	unsigned int GetSize() {
		return Size;
	}
	void AddData(unsigned char* NewData, unsigned int NewDataSize);

private:
	unsigned char* AllocatedData;
	unsigned int Size;

};

//-----------------------------------------------------------------------------
// Internal Http buffer
//-----------------------------------------------------------------------------
struct HTTP_BUFFER
{
	MemoryBuffer_socket MB_request;
	MemoryBuffer_socket MB_response;
	CHAR serverName[HTTP_HOST_IP_STRING_LENGTH];
	DWORD port;
	WSAOVERLAPPED overlapped;
};


//--------------------------------------------------------------------------------------
// Name: class HttpClient
// Desc: Http Client 
//--------------------------------------------------------------------------------------
class HttpClient
{
public:

	enum HTTP_STATUS
	{
		HTTP_STATUS_READY,
		HTTP_STATUS_BUSY,
		HTTP_STATUS_DONE,
		HTTP_STATUS_ERROR
	};

	unsigned char* smallBUffer;
	unsigned char* TempBuffer;

	memoryAlloc* ResponseData;
	// Default is port 80 and processing in worker thread
	HttpClient(BOOL bThread = TRUE, DWORD dwPort = 80);
	~HttpClient();

	// We only implement these 2 HTTP Methods
	// Check the Protocol for detail: http://www.w3.org/
	HRESULT     GET(const CHAR* strServerName, const CHAR* strFileName);
	HRESULT     POST(const CHAR* strServerName, const CHAR* strFileName, const CHAR* pCmd, DWORD dwCmdLength);

	// Content Data, exclude the HTTP header
	BYTE* GetResponseContentData() const
	{
		return m_pResponseContentData;
	};

	// Content Data Length, exclude the HTTP header
	DWORD       GetResponseContentDataLength() const
	{
		return m_dwResponseContentDataLength;
	};

	// Simple status, HTTP_STATUS_BUSY means it's still waiting for the last HTTP response
	HTTP_STATUS GetStatus()
	{
		return m_status;
	};
	VOID        SetStatus(HTTP_STATUS status)
	{
		m_status = status;
	};

	// HTTP 200, 500 etc
	DWORD       GetResponseCode()
	{
		return m_dwResponseCode;
	};

	// Socket level error
	DWORD       GetSocketErrorCode()
	{
		return m_dwSocketErrorCode;
	};
	VOID        SetSocketErrorCode(DWORD dwCode)
	{
		m_dwSocketErrorCode = dwCode;
	};

	// Internal http buffer
	HTTP_BUFFER& GetInternalBuffer()
	{
		return m_buffer;
	};

	// Find HTTP response code, content length from Http response header
	// and set the Content data start point
	HRESULT ParseIncomingHttpResponse(unsigned char* Buffer, unsigned int Size);



private:


	HRESULT     SendCommand();

	VOID        SetResponseContentData(BYTE* p)
	{
		m_pResponseContentData = p;
	};

	VOID        SetResponseContentDataLength(DWORD dwLen)
	{
		m_dwResponseContentDataLength = dwLen;
	};

	// Flag for whether start worker thread
	BOOL m_bUsingThread;

	HTTP_STATUS m_status;

	// HTTP buffer, include outgoing and incoming buffer
	HTTP_BUFFER m_buffer;

	BYTE* m_pResponseContentData;
	DWORD m_dwResponseContentDataLength;
	DWORD m_dwResponseCode;
	DWORD m_dwSocketErrorCode;
};

#endif

bool DownloadFile(const char* Server, const char* FileName, unsigned char** Out, int* length);