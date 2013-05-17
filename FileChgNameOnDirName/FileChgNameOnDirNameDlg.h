// FileChgNameOnDirNameDlg.h : header file
//

#if !defined(AFX_FILECHGNAMEONDIRNAMEDLG_H__D2481B0A_676E_4A0E_BCB2_DB1433C05CB6__INCLUDED_)
#define AFX_FILECHGNAMEONDIRNAMEDLG_H__D2481B0A_676E_4A0E_BCB2_DB1433C05CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileChgNameOnDirNameDlg dialog

class CFileChgNameOnDirNameDlg : public CDialog
{		
	int m_nMaxDealFileCount;		//一次最多处理文件数
	int m_nDealedCount;				//已经处理的文件数
//	bool m_bKeepOrgFile;			//是否保留旧文件
//	bool m_bCreatSubDir;			//是否创建子目录
//	bool m_bRenameFileSP;			//是否水平显示时 加以改名
//	bool m_bLogRecord;				//记录查询结果
	bool m_bStopFinding;			//停止查询
	CString m_csCurDir;				//当前程序目录
	CString m_csFileFilter;			//文件扩展名
	int m_nSizeFlag;				//  >  <  >=
	int m_nSizeUnit;				//文件大小　数字单位
	int m_nFileSize;				//文件大小
	CString m_csOpenDir;			//打开文件目录
	CString m_csDesDir;				//目标目录
	DWORD m_dwRealFileSize;			//文件实际大小
	DWORD m_dwRealDirFileSize;		//子目录实际大小
	int m_nDirSizeUnit;				//子目录文件大小　数字单位
	int m_nDirFileSize;				//文件大小
	DWORD m_dwTmpDirTotalSize;			//记录找到的文件合计大小，超过　m_dwRealDirFileSize　新建目录
	CString m_csSubDirName;
// Construction
public:
	void MoveNewFile(CString csOrgDName, CString csOrgFName, CString csNewDName, CString csNewFName);
	void CopyNewFile(CString csOrgDName, CString csOrgFName, CString csNewDName, CString csNewFName);
	CString GetNewDirName(CString csFilePath,CString csFileName,DWORD dwFieSize);
	CString GetNewFileName(CString csFilePath,CString csFileName);
	POINT GetJpgFileHeightAndWeight(CString csPath,CString csFile);		//获得jpg文件的宽(x)、高(y)
	CString GetSubDirName();		//通过比较　m_dwTmpDirTotalSize　和m_dwRealDirFileSize　产生子目录名　类似　001/002
	DWORD GetFileSizeCondition(int nMode);	//nMode:0--限定文件大小,1--目标子目录大小
	void InitComboBox();
	void DispQueryCondition();		//显示全部查询条件
	void WriteCFGFile(CString Section, CString Item, CString Value);
	CString CreateRecdFileName(CString csDir);
	void SetInitFileCFG();
	void InitCFG();
	void RenameFile(CString csFilePath,CString csFileName,DWORD dwFieSize);
	void RecordFileName(CString csDirPath,CString csFileName);
	bool FindAllDirectAndFile(CString csPah);
	void DispOpenDirPath();
	void DispDesDirPath();
	CFileChgNameOnDirNameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileChgNameOnDirNameDlg)
	enum { IDD = IDD_FILECHGNAMEONDIRNAME_DIALOG };
	BOOL	m_bCheckRenameSP;
	BOOL	m_bCheckKeepOrgFile;
	BOOL	m_bCheckLogRecord;
	BOOL	m_bCheckCreateSubDir;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileChgNameOnDirNameDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFileChgNameOnDirNameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonOpendir();
	afx_msg void OnButtonFindall();
	afx_msg void OnButtonStopfind();
	afx_msg void OnChangeEditFilefilter();
	afx_msg void OnChangeEditDir();
	afx_msg void OnChangeEditFilesize();
	afx_msg void OnCloseupComboSizeflag();
	afx_msg void OnCloseupComboSizeunit();
	afx_msg void OnChangeEditDesdir();
	afx_msg void OnCloseupComboSizeunitDir();
	afx_msg void OnChangeEditFilesizeDir();
	afx_msg void OnCheckCreatesubdir();
	afx_msg void OnCheckKeeporgfile();
	afx_msg void OnCheckRenamesp();
	afx_msg void OnCheckLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILECHGNAMEONDIRNAMEDLG_H__D2481B0A_676E_4A0E_BCB2_DB1433C05CB6__INCLUDED_)
