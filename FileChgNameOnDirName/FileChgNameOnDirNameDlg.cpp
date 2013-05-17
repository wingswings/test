 // FileChgNameOnDirNameDlg.cpp : implementation file
//
//2013.04.21.bj
//功能：将源目录中的文件，按扩展名，文件大小，复制到目标目录，为防止有重名，目标文件名加上了最后两级目录
//#include <stdio.h>
#include "stdafx.h"
#include "FileChgNameOnDirName.h"
#include "FileChgNameOnDirNameDlg.h"
#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#define MAXDEALCOUNT 300

CString csFileFlagName[] = 
{
	">",
	"<",
	"=",
};
CString csFileSizeUnit[] = 
{
	"B",
	"KB",
	"MB",
};
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileChgNameOnDirNameDlg dialog

CFileChgNameOnDirNameDlg::CFileChgNameOnDirNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileChgNameOnDirNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileChgNameOnDirNameDlg)
	m_bCheckRenameSP = FALSE;
	m_bCheckKeepOrgFile = FALSE;
	m_bCheckLogRecord = FALSE;
	m_bCheckCreateSubDir = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bStopFinding = FALSE;
	m_dwTmpDirTotalSize = 0;
	m_nDealedCount = 0;
}

void CFileChgNameOnDirNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileChgNameOnDirNameDlg)
	DDX_Check(pDX, IDC_CHECK_RENAMESP, m_bCheckRenameSP);
	DDX_Check(pDX, IDC_CHECK_KEEPORGFILE, m_bCheckKeepOrgFile);
	DDX_Check(pDX, IDC_CHECK_LOG, m_bCheckLogRecord);
	DDX_Check(pDX, IDC_CHECK_CREATESUBDIR, m_bCheckCreateSubDir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileChgNameOnDirNameDlg, CDialog)
	//{{AFX_MSG_MAP(CFileChgNameOnDirNameDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENDIR, OnButtonOpendir)
	ON_BN_CLICKED(IDC_BUTTON_FINDALL, OnButtonFindall)
	ON_BN_CLICKED(IDC_BUTTON_STOPFIND, OnButtonStopfind)
	ON_EN_CHANGE(IDC_EDIT_FILEFILTER, OnChangeEditFilefilter)
	ON_EN_CHANGE(IDC_EDIT_DIR, OnChangeEditDir)
	ON_EN_CHANGE(IDC_EDIT_FILESIZE, OnChangeEditFilesize)
	ON_CBN_CLOSEUP(IDC_COMBO_SIZEFLAG, OnCloseupComboSizeflag)
	ON_CBN_CLOSEUP(IDC_COMBO_SIZEUNIT, OnCloseupComboSizeunit)
	ON_EN_CHANGE(IDC_EDIT_DESDIR, OnChangeEditDesdir)
	ON_CBN_CLOSEUP(IDC_COMBO_SIZEUNIT_DIR, OnCloseupComboSizeunitDir)
	ON_EN_CHANGE(IDC_EDIT_FILESIZE_DIR, OnChangeEditFilesizeDir)
	ON_BN_CLICKED(IDC_CHECK_CREATESUBDIR, OnCheckCreatesubdir)
	ON_BN_CLICKED(IDC_CHECK_KEEPORGFILE, OnCheckKeeporgfile)
	ON_BN_CLICKED(IDC_CHECK_RENAMESP, OnCheckRenamesp)
	ON_BN_CLICKED(IDC_CHECK_LOG, OnCheckLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileChgNameOnDirNameDlg message handlers

BOOL CFileChgNameOnDirNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	InitComboBox();
	InitCFG();

	SetInitFileCFG();
	DispDesDirPath();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileChgNameOnDirNameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileChgNameOnDirNameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileChgNameOnDirNameDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFileChgNameOnDirNameDlg::OnButtonOpendir() 
{
	// TODO: Add your control notification handler code here	
	CFileDialog fileDialog(TRUE);
	if (fileDialog.DoModal() == IDOK) 
	{
		m_csOpenDir = fileDialog.GetPathName();
		m_csOpenDir.Replace(fileDialog.GetFileName(),"");
		GetDlgItem(IDC_EDIT_DIR)->SetWindowText(m_csOpenDir);
		DispOpenDirPath();
	}
}

BOOL CFileChgNameOnDirNameDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if(pMsg->message==WM_KEYDOWN 
//		&& pMsg->wParam==VK_RETURN)//屏住ENTER
//	{
//		return TRUE;
//	}
//	else
//    if (pMsg->message == WM_KEYDOWN && 
 //   pMsg->wParam == VK_ESCAPE) //屏蔽ESC
 //    return TRUE; 
	
	if (pMsg->message >=WM_KEYFIRST
		&&pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->wParam==VK_RETURN)
		{
			HWND hWnd = ::GetFocus();
			int nID = ::GetDlgCtrlID(hWnd);

			switch(nID) {
			case IDC_EDIT_DIR://input dir name,press Enter to Display
				DispOpenDirPath();
				break;
			case IDC_EDIT_DESDIR:
				DispDesDirPath();
				break;
			default:
				;
			}

			return TRUE;
		}
		
		if (pMsg->wParam==VK_ESCAPE) //屏蔽ESC
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CFileChgNameOnDirNameDlg::DispDesDirPath()
{
	CString csTmp;
	GetDlgItem(IDC_EDIT_DESDIR)->GetWindowText(csTmp);

	if (csTmp.Right(1) != "\\")
		csTmp = csTmp + "\\";

	if (GetFileAttributes(csTmp) == 0XFFFFFFFF) 
	{
		::CreateDirectory(csTmp,NULL);
	}

	WriteCFGFile("common","DesFile",csTmp);
	m_csDesDir = csTmp;
	
}
void CFileChgNameOnDirNameDlg::DispOpenDirPath()
{
	CString csTmp;
	GetDlgItem(IDC_EDIT_DIR)->GetWindowText(csTmp);

//	DWORD AAA = GetFileAttributes(csTmp);
//	if (GetFileAttributes(csTmp) != FILE_ATTRIBUTE_DIRECTORY) 
	if (GetFileAttributes(csTmp) == 0XFFFFFFFF) 
		AfxMessageBox("Cant Find the Directory : " + csTmp);
	else
	{
		WriteCFGFile("common","OpenFile",m_csOpenDir);
	}
	
}

bool CFileChgNameOnDirNameDlg::FindAllDirectAndFile(CString csPah)
{	
	if (m_bStopFinding)
		return FALSE;

	if (csPah.Right(1) != "\\")
		csPah = csPah + "\\";
	
	CFileFind finder;
	BOOL bWorking = finder.FindFile(csPah + _T("*.*"));

	CString csTmp,csName;

	while (bWorking
		&&!m_bStopFinding
//		&&((m_nDealedCount < m_nMaxDealFileCount
//				&&m_bCheckRenameSP)
//			||!m_bCheckRenameSP)
			)
	{
		bWorking = finder.FindNextFile();
		csName=finder.GetFileName();

		if (finder.IsDots())
			continue;

		if (!finder.IsDirectory() )
		{
			csTmp = csName;
			csTmp.MakeLower();
			if (csTmp.Find(m_csFileFilter) <= 0) 
				continue;

			RenameFile(csPah,csName,finder.GetLength());
			
			if (m_bCheckLogRecord)
				RecordFileName(csPah , csName);
		}
		else if (finder.IsDirectory())
		{
			csName = finder.GetFilePath();
			if (csPah == csName)		//防止死循环
				continue;
			
			FindAllDirectAndFile(csName);
		}
	}
	return TRUE;
}

void CFileChgNameOnDirNameDlg::OnButtonFindall() 
{
	// TODO: Add your control notification handler code here
	if (GetFileAttributes(m_csOpenDir) == 0XFFFFFFFF) 
		AfxMessageBox("Cant Find the Directory : " + m_csOpenDir);
	else
	{
		GetDlgItem(IDC_BUTTON_STOPFIND)->EnableWindow(TRUE);
		m_bCheckLogRecord = ((CButton *) GetDlgItem(IDC_CHECK_LOG))->GetCheck();
		m_bCheckCreateSubDir = ((CButton *)GetDlgItem(IDC_CHECK_CREATESUBDIR))->GetCheck();
		m_bCheckKeepOrgFile = ((CButton *)GetDlgItem(IDC_CHECK_KEEPORGFILE))->GetCheck();
		m_bStopFinding = FALSE;
		m_dwRealFileSize = GetFileSizeCondition(0);
		m_dwRealDirFileSize = GetFileSizeCondition(1);
		m_nDealedCount = 0;
		::DeleteFile(CreateRecdFileName(m_csOpenDir));
		if(!FindAllDirectAndFile(m_csOpenDir))
			AfxMessageBox("Finding Stopped");
		else
		{
			CString x;
			x.Format("%d Files dealed",m_nDealedCount);
			AfxMessageBox(x);
//			PostMessage(WM_CLOSE);
//			Sleep(3000);
//			if (m_nDealedCount == m_nMaxDealFileCount) 
//				OnButtonFindall();
		}
		GetDlgItem(IDC_BUTTON_STOPFIND)->EnableWindow(FALSE);

	}
}

void CFileChgNameOnDirNameDlg::RecordFileName(CString csDirPath, CString csFileName)
{
	CStdioFile file;

	CFileException e;
	if (!file.Open(CreateRecdFileName(m_csOpenDir),
			CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite, &e))
	{
		TRACE ("Open error\n");
		AfxThrowFileException(e.m_cause, e.m_lOsError, e.m_strFileName);
		return;
	}

	file.SeekToEnd();
	file.WriteString(csDirPath + csFileName + "\n");
	
	POINT ptSize = GetJpgFileHeightAndWeight(csDirPath , csFileName);
	CString x;
	x.Format("\tW:%d,\tH:%d",ptSize.x,ptSize.y);
	file.WriteString(x + "\n");

	file.Close();
}
 
void CFileChgNameOnDirNameDlg::RenameFile(CString csFilePath, CString csFileName,DWORD dwFieSize)
{	
//	CFileFind finder;
//	if( finder.FindFile(csFilePath+csFileName))
	{
//		finder.FindNextFile();

		if(dwFieSize > m_dwRealFileSize)
		{
			CString csNewName = GetNewFileName(csFilePath,csFileName);
			CString csNewPath = GetNewDirName(csFilePath,csFileName,dwFieSize);							

			if (m_bCheckKeepOrgFile)
			{
				CopyNewFile(csFilePath,csFileName,csNewPath,csNewName);
			}			
			else
			{
				MoveNewFile(csFilePath,csFileName,csNewPath,csNewName);
			}				
//			Sleep(10);
			m_nDealedCount ++;
		}
	}
}

void CFileChgNameOnDirNameDlg::InitCFG()
{
	CString csTemp;
	GetModuleFileName(NULL,csTemp.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	m_csCurDir = csTemp.Left(csTemp.ReverseFind('\\')+1);

	CIniFile iniFile;
	iniFile.SetFileName(m_csCurDir + CFGFILE);

	m_csFileFilter	= iniFile.GetString("common","FileFilter","*");
	m_csFileFilter.MakeLower();
	m_csFileFilter = "." + m_csFileFilter;
	m_csOpenDir		= iniFile.GetString("common","OpenFile","c:\\");
	if (m_csOpenDir.Right(1) != "\\")
		m_csOpenDir = m_csOpenDir + "\\";
	m_csDesDir		= iniFile.GetString("common","DesFile","c:\\");
	if (m_csDesDir.Right(1) != "\\")
		m_csDesDir = m_csDesDir + "\\";

	m_nFileSize		= iniFile.GetInt("common","FileSize",100);
	m_nSizeUnit		= iniFile.GetInt("common","SizeUnit",1);
	m_nSizeFlag		= iniFile.GetInt("common","SizeFlag",1);

	m_nDirFileSize		= iniFile.GetInt("dir","dirTotalSize",100);
	m_nDirSizeUnit		= iniFile.GetInt("dir","SizeUnit",1);
	m_nMaxDealFileCount		= iniFile.GetInt("dir","MaxDealFileCount",100);
	m_bCheckCreateSubDir		= iniFile.GetInt("dir","CreateSubDir",FALSE);

	m_bCheckKeepOrgFile		= iniFile.GetInt("file","KeepOrgFile",FALSE);
	m_bCheckRenameSP		= iniFile.GetInt("file","CheckRenameSP",FALSE);
	m_bCheckLogRecord		= iniFile.GetInt("file","LogRecord",FALSE);

	UpdateData(FALSE);
	
}

void CFileChgNameOnDirNameDlg::SetInitFileCFG()
{
	((CComboBox *) GetDlgItem(IDC_COMBO_SIZEUNIT))->SetCurSel(m_nSizeUnit);
	((CComboBox *) GetDlgItem(IDC_COMBO_SIZEFLAG))->SetCurSel(m_nSizeFlag);
	((CComboBox *) GetDlgItem(IDC_COMBO_SIZEUNIT_DIR))->SetCurSel(m_nDirSizeUnit);	

	CString csTmp;
	csTmp.Format("%d",m_nFileSize);
	GetDlgItem(IDC_EDIT_FILESIZE)->SetWindowText(csTmp);
	csTmp.Format("%d",m_nDirFileSize);
	GetDlgItem(IDC_EDIT_FILESIZE_DIR)->SetWindowText(csTmp);

	GetDlgItem(IDC_STATIC_OPENDIR)->SetWindowText(m_csOpenDir);
	GetDlgItem(IDC_EDIT_DIR)->SetWindowText(m_csOpenDir);
		
	GetDlgItem(IDC_EDIT_FILEFILTER)->SetWindowText(m_csFileFilter);
	GetDlgItem(IDC_EDIT_DESDIR)->SetWindowText(m_csDesDir);
	
}

void CFileChgNameOnDirNameDlg::OnButtonStopfind() 
{
	// TODO: Add your control notification handler code here
	m_bStopFinding = TRUE;
}

void CFileChgNameOnDirNameDlg::OnChangeEditFilefilter() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_FILEFILTER)->GetWindowText(m_csFileFilter);
	if (m_csFileFilter.Left(1) != ".")
		m_csFileFilter = "." + m_csFileFilter;
	
	WriteCFGFile("common","FileFilter",m_csFileFilter.Mid(1,m_csFileFilter.GetLength()));
}

void CFileChgNameOnDirNameDlg::OnChangeEditDir() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DIR)->GetWindowText(m_csOpenDir);

	WriteCFGFile("common","OpenFile",m_csOpenDir);	
}

void CFileChgNameOnDirNameDlg::OnChangeEditFilesize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	CString csTmp;
	csTmp.Format("%d",m_nFileSize);
	GetDlgItem(IDC_EDIT_FILESIZE)->GetWindowText(csTmp);
	m_nFileSize = atoi(csTmp);
	
	WriteCFGFile("common","FileSize",csTmp);
}

void CFileChgNameOnDirNameDlg::OnCloseupComboSizeflag() 
{
	// TODO: Add your control notification handler code here
	
	m_nSizeFlag = ((CComboBox *) GetDlgItem(IDC_COMBO_SIZEFLAG))->GetCurSel();

	CString csTmp;
	csTmp.Format("%d", m_nSizeFlag);
	WriteCFGFile("common","SizeFlag",csTmp);
}

void CFileChgNameOnDirNameDlg::OnCloseupComboSizeunit() 
{
	// TODO: Add your control notification handler code here
	
	m_nSizeUnit = ((CComboBox *) GetDlgItem(IDC_COMBO_SIZEUNIT))->GetCurSel();
	CString csTmp;
	csTmp.Format("%d", m_nSizeUnit);
	WriteCFGFile("common","SizeUnit",csTmp);
}

CString CFileChgNameOnDirNameDlg::CreateRecdFileName(CString csDir)
{
	CString csTmpFileName;
	csTmpFileName = m_csOpenDir;
	csTmpFileName.Replace(":\\","_");
	csTmpFileName.Replace("\\","_");
	csTmpFileName.Replace(":","_");
	if (csTmpFileName.Right(1) == "_" ) 
		csTmpFileName.Delete(csTmpFileName.GetLength()-1);
	
	csTmpFileName = "c:\\" + csTmpFileName + ".txt";
	return csTmpFileName;
}

void CFileChgNameOnDirNameDlg::WriteCFGFile(CString Section, CString Item, CString Value)
{
	CIniFile inifile;
	inifile.SetFileName(m_csCurDir + CFGFILE);
	if (Item == "OpenFile") 
	{
		if (m_csOpenDir.Right(1) != "\\")
			m_csOpenDir = m_csOpenDir + "\\";
	}
	inifile.WriteFileString(Section,Item,Value);
	inifile.WriteIniFile();
	DispQueryCondition();
}

void CFileChgNameOnDirNameDlg::DispQueryCondition()
{
	CString csTmp;
	
	csTmp.Format("查询条件　源目录:%s，扩展名:%s，条件:%s %d %s，目标目录:%s",
		m_csOpenDir,
		m_csFileFilter,
		csFileFlagName[((CComboBox *)GetDlgItem(IDC_COMBO_SIZEFLAG))->GetCurSel()],
		m_nFileSize,
		csFileSizeUnit[((CComboBox *)GetDlgItem(IDC_COMBO_SIZEUNIT))->GetCurSel()],
		m_csDesDir);
	GetDlgItem(IDC_STATIC_OPENDIR)->SetWindowText(csTmp);	
}

void CFileChgNameOnDirNameDlg::InitComboBox()
{
	int i;
	for(i=0; i<sizeof(csFileFlagName)/sizeof(CString); i++)
		((CComboBox *)GetDlgItem(IDC_COMBO_SIZEFLAG))->AddString(csFileFlagName[i]);
	
	for(i=0; i<sizeof(csFileSizeUnit)/sizeof(CString); i++)
		((CComboBox *)GetDlgItem(IDC_COMBO_SIZEUNIT))->AddString(csFileSizeUnit[i]);

	
	for(i=0; i<sizeof(csFileSizeUnit)/sizeof(CString); i++)
		((CComboBox *)GetDlgItem(IDC_COMBO_SIZEUNIT_DIR))->AddString(csFileSizeUnit[i]);
}

DWORD CFileChgNameOnDirNameDlg::GetFileSizeCondition(int nMode)
{	
	DWORD dwFileSize;
	int nSizeSel;
	switch(nMode) {
	case 1:
		nSizeSel = ((CComboBox *)GetDlgItem(IDC_COMBO_SIZEUNIT_DIR))->GetCurSel();
		dwFileSize = m_nDirFileSize;
		break;
	case 0:
	default:
		nSizeSel = ((CComboBox *)GetDlgItem(IDC_COMBO_SIZEUNIT))->GetCurSel();
		dwFileSize = m_nFileSize;
		break;
	} 
	while (nSizeSel
		&&nSizeSel < sizeof(csFileSizeUnit)/sizeof(CString)) 
	{
		dwFileSize = dwFileSize*1024;
		nSizeSel--;
	}
	return dwFileSize;
}

void CFileChgNameOnDirNameDlg::OnChangeEditDesdir() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
//	GetDlgItem(IDC_EDIT_DESDIR)->GetWindowText(m_csDesDir);

//	WriteCFGFile("common","DesFile",m_csDesDir);		
}

void CFileChgNameOnDirNameDlg::OnCloseupComboSizeunitDir() 
{
	// TODO: Add your control notification handler code here
	m_nDirSizeUnit = ((CComboBox *) GetDlgItem(IDC_COMBO_SIZEUNIT_DIR))->GetCurSel();
	CString csTmp;
	csTmp.Format("%d", m_nDirSizeUnit);
	WriteCFGFile("dir","SizeUnit",csTmp);	
}

void CFileChgNameOnDirNameDlg::OnChangeEditFilesizeDir() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString csTmp;
	GetDlgItem(IDC_EDIT_FILESIZE_DIR)->GetWindowText(csTmp);

	m_nDirFileSize = atoi(csTmp);
	WriteCFGFile("dir","dirTotalSize",csTmp);	
	
}

CString CFileChgNameOnDirNameDlg::GetSubDirName()
{
	CString csDirName;

	int nQuotient = m_dwTmpDirTotalSize / m_dwRealDirFileSize;
	int nRemainder = m_dwTmpDirTotalSize % m_dwRealDirFileSize;

	if (nRemainder > 0) 
		nQuotient++;

	csDirName.Format("%03d\\",nQuotient);
	
//	if (GetFileAttributes(m_csDesDir + csDirName) == 0XFFFFFFFF) //INVALID_FILE_ATTRIBUTES
//	{
//		::CreateDirectory(m_csDesDir + csDirName,NULL);
//	}
	return csDirName;
}

POINT CFileChgNameOnDirNameDlg::GetJpgFileHeightAndWeight(CString csPath, CString csFile)
{
//	//////////////////////////////////////////////////////////////////////////
//	//---------------------可获取到JPG个数图像的高度和宽度-----------------------//
//	CFileDialog dlg(TRUE, _T("jpg"), _T("*.jpg"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("JPEG文件(*.jpg)|*.jpg|GIF文件(*.gif)|*.gif||"),NULL);   
//	CString   m_Path; 
//	if(dlg.DoModal()==IDOK)   
//	{   
//		m_Path=dlg.GetPathName();   
//		Invalidate();   
//	}   
//	IStream *pStm;
//	CFileStatus fstatus; 
//	CFile file;
//	LONG cb;
//	LPVOID pvData = NULL;
//	HGLOBAL hGlobal;
//	
//	if(file.Open(m_Path,CFile::modeRead)
//		&& file.GetStatus(m_Path,fstatus)
//		&& ((cb=fstatus.m_size)!=-1))
//	{     
//		hGlobal=GlobalAlloc(GMEM_MOVEABLE,cb);
//		if(hGlobal!=NULL)
//		{
//			if((pvData=GlobalLock(hGlobal))!=NULL)     
//			{
//				file.ReadHuge(pvData,cb);
//				GlobalUnlock(hGlobal);
//				CreateStreamOnHGlobal(hGlobal,TRUE,&pStm);
//			}   
//		}   
//	}   
//	
//	
//	IPicture *pPic; 
//	if(SUCCEEDED(OleLoadPicture(pStm,fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)))   
//	{ 
//		OLE_XSIZE_HIMETRIC   hmWidth=0;     
//		OLE_YSIZE_HIMETRIC   hmHeight=0;     
//		
//		pPic-> get_Width(&hmWidth);     
//		pPic-> get_Height(&hmHeight);     
//		CDC *p=GetDC(); 
//		
//		int lWidthPixels = MulDiv(hmWidth, p->GetDeviceCaps(LOGPIXELSX),2540); 
//		int lHeightPixels = MulDiv(hmHeight, p->GetDeviceCaps(LOGPIXELSY),2540); 
//		
//		CString   x; 
//		x.Format( "宽度:%d,高度:%d",lWidthPixels,lHeightPixels); 
//		AfxMessageBox(x); 
//	}
//	GlobalFree(hGlobal);
//	//------------------------------------END------------------------------------//
//	//////////////////////////////////////////////////////////////////////////
	
	POINT ptSize;  
	ptSize.x = -1; 
	ptSize.y = -1;
	CString x = csFile;
	x.MakeLower();

	if (x.Find(".jpg") <= 0) 
		return ptSize;

	CString m_csPath = csPath + csFile;

	IStream *pStm;
	CFileStatus fstatus; 
	CFile file;
	LONG cb;
	LPVOID pvData = NULL;
	HGLOBAL hGlobal;
	if(file.Open(m_csPath,CFile::modeRead)
		&& file.GetStatus(m_csPath,fstatus)
		&& ((cb=fstatus.m_size)!=-1))
	{     
		hGlobal=GlobalAlloc(GMEM_MOVEABLE,cb);
		if(hGlobal!=NULL)
		{
			if((pvData=GlobalLock(hGlobal))!=NULL)     
			{
				file.ReadHuge(pvData,cb);
				GlobalUnlock(hGlobal);
				CreateStreamOnHGlobal(hGlobal,TRUE,&pStm);
			}   
		}   
	}   
	
	IPicture *pPic; 
	if(SUCCEEDED(OleLoadPicture(pStm,fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)))   
	{ 
		OLE_XSIZE_HIMETRIC   hmWidth=0;     
		OLE_YSIZE_HIMETRIC   hmHeight=0;     
		
		pPic-> get_Width(&hmWidth);     
		pPic-> get_Height(&hmHeight);     
		CDC *p=GetDC(); 
		
		int lWidthPixels = MulDiv(hmWidth, p->GetDeviceCaps(LOGPIXELSX),2540); 
		int lHeightPixels = MulDiv(hmHeight, p->GetDeviceCaps(LOGPIXELSY),2540); 
		
//		CString   x; 
//		x.Format( "宽度:%d,高度:%d",lWidthPixels,lHeightPixels); 
		//		AfxMessageBox(x); 
		ptSize.x = lWidthPixels;
		ptSize.y = lHeightPixels;
		
		ReleaseDC(p);
		pPic->Release();
		pStm->Release();
	}
	GlobalFree(hGlobal);
//	delete hGlobal;

	return ptSize;
}

void CFileChgNameOnDirNameDlg::OnCheckCreatesubdir() 
{
	// TODO: Add your control notification handler code here
	m_bCheckCreateSubDir = ((CButton *)GetDlgItem(IDC_CHECK_CREATESUBDIR))->GetCheck();
	GetDlgItem(IDC_STATIC_SUBDIRSIZE)->EnableWindow(m_bCheckCreateSubDir);
	GetDlgItem(IDC_EDIT_FILESIZE_DIR)->EnableWindow(m_bCheckCreateSubDir);
	GetDlgItem(IDC_COMBO_SIZEUNIT_DIR)->EnableWindow(m_bCheckCreateSubDir);

	CString x;
	x.Format("%d",m_bCheckCreateSubDir);
	WriteCFGFile("dir","CreateSubDir",x);	
}

CString CFileChgNameOnDirNameDlg::GetNewFileName(CString csFilePath, CString csFileName)
{
	CString csNewName = csFilePath;
	CString csTmp;

	csNewName.Replace(":\\","_");
	csNewName.Replace("\\","_");
	csNewName.Replace(":","_");
	if (csNewName.Right(1) == "_" ) 
		csNewName.Delete(csNewName.GetLength()-1);
	
	int nFind = csNewName.ReverseFind('_');
	if (nFind)
	{
		csTmp = csNewName.Left(nFind);
		nFind = csTmp.ReverseFind('_');
		if (nFind)
		{
			csTmp = csNewName.Left(nFind+1);
		}
		csNewName.Delete(0,csTmp.GetLength());
	}
	if (m_bCheckRenameSP)
	{
		POINT ptSize = GetJpgFileHeightAndWeight(csFilePath,csFileName);
		if (ptSize.x > ptSize.y)
			csNewName = "SP_" + csNewName;
	}
	csNewName = csNewName + "_" + csFileName;
	if (csNewName.GetLength() >= 0XFF)
		csNewName = csNewName.Left(250) + csNewName.Right(5);

	return csNewName;
}

CString CFileChgNameOnDirNameDlg::GetNewDirName(CString csFilePath, CString csFileName,DWORD dwFieSize)
{
	CString csNewPath;
	//csNewPath = m_csDesDir + csNewName + "_" + csFileName;
	csNewPath = m_csDesDir;
	
	if(m_bCheckCreateSubDir)			
	{
		m_dwTmpDirTotalSize += dwFieSize;
		csNewPath = csNewPath + GetSubDirName();
	}
	return csNewPath;
}

void CFileChgNameOnDirNameDlg::CopyNewFile(CString csOrgDName, CString csOrgFName, CString csNewDName, CString csNewFName)
{
	int bResult = ::CopyFile(csOrgDName+csOrgFName,csNewDName+csNewFName,false);
	if (!bResult) 
	{
		if (GetFileAttributes(csNewDName) == 0XFFFFFFFF) //INVALID_FILE_ATTRIBUTES
		{
			::CreateDirectory(csNewDName,NULL);
			::CopyFile(csOrgDName+csOrgFName,csNewDName+csNewFName,false);
		}
	}
}

void CFileChgNameOnDirNameDlg::MoveNewFile(CString csOrgDName, CString csOrgFName, CString csNewDName, CString csNewFName)
{
	//MoveFile(csFilePath+csFileName,csNewPath);	
	int bResult = ::MoveFile(csOrgDName+csOrgFName,csNewDName+csNewFName);
	if (!bResult) 
	{
		if (GetFileAttributes(csNewDName) == 0XFFFFFFFF) //INVALID_FILE_ATTRIBUTES
		{
			::CreateDirectory(csNewDName,NULL);
			::MoveFile(csOrgDName+csOrgFName,csNewDName+csNewFName);
		}
	}
}

void CFileChgNameOnDirNameDlg::OnCheckKeeporgfile() 
{
	// TODO: Add your control notification handler code here
	m_bCheckKeepOrgFile = ((CButton *)GetDlgItem(IDC_CHECK_KEEPORGFILE))->GetCheck();

	CString x;
	x.Format("%d",m_bCheckKeepOrgFile);
	WriteCFGFile("file","KeepOrgFile",x);	
}

void CFileChgNameOnDirNameDlg::OnCheckRenamesp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_bCheckRenameSP = ((CButton *)GetDlgItem(IDC_CHECK_RENAMESP))->GetCheck();	
	
	CString x;
	x.Format("%d",m_bCheckRenameSP);
	WriteCFGFile("file","CheckRenameSP",x);	
}

void CFileChgNameOnDirNameDlg::OnCheckLog() 
{
	// TODO: Add your control notification handler code here
	m_bCheckLogRecord = ((CButton *)GetDlgItem(IDC_CHECK_LOG))->GetCheck();

	CString x;
	x.Format("%d",m_bCheckLogRecord);
	WriteCFGFile("file","LogRecord",x);	
	
}
