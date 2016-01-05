// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "export.h"
#include <stdio.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	TCHAR str[50]={0};
	int a = add(1,2);
	swprintf(str,L"process attach 1+2=%d",a);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL,str,NULL,0);
		break;
	case DLL_THREAD_ATTACH:
		MessageBox(NULL,L"Thread attach",NULL,0);
		break;
	case DLL_THREAD_DETACH:
		MessageBox(NULL,L"Thread detach",NULL,0);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(NULL,L"Process detach",NULL,0);
		break;
	}
	return TRUE;
}

int add(int a,int b)
{
	return a+b;
}

int sub(int a,int b)
{
	return a-b;
}

