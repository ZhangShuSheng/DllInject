// InjectDLLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InjectDLL.h"
#include "InjectDLLDlg.h"
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CInjectDLLDlg 对话框




CInjectDLLDlg::CInjectDLLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInjectDLLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectDLLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInjectDLLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CInjectDLLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInjectDLLDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CInjectDLLDlg 消息处理程序

BOOL CInjectDLLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInjectDLLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInjectDLLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInjectDLLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//open
void CInjectDLLDlg::OnBnClickedButton1()
{
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("DLL|*.dll|"),NULL,0,TRUE);
	if (file.DoModal()==IDOK)
	{
		CString str;
		str=file.GetPathName();
		GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	}

}
//inject
void CInjectDLLDlg::OnBnClickedButton2()
{
	CString strExe; 
	GetDlgItemText(IDC_EDIT2,strExe);
	if (strExe.GetLength()==0)
	{
		MessageBox(L"input exe name");
		return ;
	}
	DWORD dwProID=0;
	dwProID=findProcess(strExe);
	if (dwProID==0)
	{
		MessageBox(L"exe do not exist");
		return ;
	}
	
	CString strDllPath;
	GetDlgItemText(IDC_EDIT1,strDllPath);
	bool rn = inject(strDllPath,dwProID);
	if (rn)
	{
		MessageBox(L"inject successful");
	}
}

DWORD CInjectDLLDlg::findProcess(CString exeName)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProcessSnap==NULL)/*INVALID_HANDLE_VALUE*/
	{
		return 0;
	}
	PROCESSENTRY32 pe32;
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );    // Must clean up the
		//   snapshot object!
		return( FALSE );
	}
	do  
	{ 
		if(!_tcscmp(pe32.szExeFile,exeName))//查找指定进程名的PID 
		{ 
			return pe32.th32ProcessID; 
		
		} 
	} while (Process32Next(hProcessSnap,&pe32)); 


}

bool CInjectDLLDlg::inject(CString dllPath,DWORD processID)
{
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS, FALSE, processID );
	if( hProcess == NULL )
	{
		return false;
	}
	DWORD dwSize = (_tcslen(dllPath)+1)*sizeof(TCHAR);
	DWORD dwHasWrite; 
	LPVOID lpRemoteBuf = VirtualAllocEx(hProcess,NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);
	if (WriteProcessMemory(hProcess,lpRemoteBuf,dllPath,dwSize,&dwHasWrite))
	{
		if(dwHasWrite != dwSize) 
		{ 
			VirtualFreeEx(hProcess,lpRemoteBuf,dwSize,MEM_COMMIT); 
			CloseHandle(hProcess); 
			return -1; 
		} 

		PTHREAD_START_ROUTINE pfnStartAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("kernel32.dll")),"LoadLibraryW");
		if (pfnStartAddress==NULL)
		{
			return false;
		}
		HANDLE hThread = CreateRemoteThread(hProcess,NULL,0,pfnStartAddress,lpRemoteBuf,0,0); 
		if (!hThread)
		{
			return false;
		}
		WaitForSingleObject(hThread,INFINITE); 		
		
		CloseHandle(hThread);
		CloseHandle(hProcess);
		
		return true;
	}

	return false;
}
