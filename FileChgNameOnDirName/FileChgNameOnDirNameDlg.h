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
	int m_nMaxDealFileCount;		//һ����ദ���ļ���
	int m_nDealedCount;				//�Ѿ�������ļ���
//	bool m_bKeepOrgFile;			//�Ƿ������ļ�
//	bool m_bCreatSubDir;			//�Ƿ񴴽���Ŀ¼
//	bool m_bRenameFileSP;			//�Ƿ�ˮƽ��ʾʱ ���Ը���
//	bool m_bLogRecord;				//��¼��ѯ���
	bool m_bStopFinding;			//ֹͣ��ѯ
	CString m_csCurDir;				//��ǰ����Ŀ¼
	CString m_csFileFilter;			//�ļ���չ��
	int m_nSizeFlag;				//  >  <  >=
	int m_nSizeUnit;				//�ļ���С�����ֵ�λ
	int m_nFileSize;				//�ļ���С
	CString m_csOpenDir;			//���ļ�Ŀ¼
	CString m_csDesDir;				//Ŀ��Ŀ¼
	DWORD m_dwRealFileSize;			//�ļ�ʵ�ʴ�С
	DWORD m_dwRealDirFileSize;		//��Ŀ¼ʵ�ʴ�С
	int m_nDirSizeUnit;				//��Ŀ¼�ļ���С�����ֵ�λ
	int m_nDirFileSize;				//�ļ���С
	DWORD m_dwTmpDirTotalSize;			//��¼�ҵ����ļ��ϼƴ�С��������m_dwRealDirFileSize���½�Ŀ¼
	CString m_csSubDirName;
// Construction
public:
	void MoveNewFile(CString csOrgDName, CString csOrgFName, CString csNewDName, CString csNewFName);
	void CopyNewFile(CString csOrgDName, CString csOrgFName, CString csNewDName, CString csNewFName);
	CString GetNewDirName(CString csFilePath,CString csFileName,DWORD dwFieSize);
	CString GetNewFileName(CString csFilePath,CString csFileName);
	POINT GetJpgFileHeightAndWeight(CString csPath,CString csFile);		//���jpg�ļ��Ŀ�(x)����(y)
	CString GetSubDirName();		//ͨ���Ƚϡ�m_dwTmpDirTotalSize����m_dwRealDirFileSize��������Ŀ¼�������ơ�001/002
	DWORD GetFileSizeCondition(int nMode);	//nMode:0--�޶��ļ���С,1--Ŀ����Ŀ¼��С
	void InitComboBox();
	void DispQueryCondition();		//��ʾȫ����ѯ����
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
