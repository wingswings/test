// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile()
{
}

CIniFile::~CIniFile()
{
	if(m_csFileContainer.GetSize() > 0)
	{
		m_csFileContainer.RemoveAll();
	}
	if (m_csAllSections.GetSize() > 0) 
	{
		m_csAllSections.RemoveAll();
	}
}

void CIniFile::SetFileName(CString FileName)
{
	m_sFileName = FileName;
	if (!ReadIniFile()) 
		AfxMessageBox("Failed to Read File "+FileName);
}

void CIniFile::SetInt(CString Section, CString Item, int Value)
{
	CString strtemp;
	strtemp.Format("%d",Value);
	//���浽m_csFileContainer
	WriteFileString(Section, Item, strtemp);
	//���浽ini�ļ�
	WriteIniFile();
}

void CIniFile::SetString(CString Section, CString Item, CString Value)
{
	//���浽m_csFileContainer
	WriteFileString(Section, Item, Value);
	//���浽ini�ļ�
	WriteIniFile();
}

CString CIniFile::GetString(CString Section, CString Item, CString Value)
{
	return GetFileString(Section, Item, Value);
}

int CIniFile::GetInt(CString Section, CString Item, int Value)
{
	CString strtemp;
	strtemp.Format("%d",Value);
	return atoi(GetFileString(Section, Item, strtemp));
}

BOOL CIniFile::ReadIniFile()
{
	bFileExist=IniFile.Open(m_sFileName, CFile::modeRead);

	if(!bFileExist)
	{
		return FALSE;
	}

	CString csLine;
	CString csTemp;
	int nTemp;
	m_csFileContainer.RemoveAll();
	m_csAllSections.RemoveAll();

	//��IniFile�ļ����ݶ���m_csFileContainer
	while(IniFile.ReadString(csTemp))
	{
		nTemp = csTemp.Find("//");
		if (nTemp >=0)
		{
			csLine = csTemp.Left(nTemp);
			csTemp = csLine;
		}
		nTemp = csTemp.Find(";");
		if (nTemp >=0)
		{
			csLine = csTemp.Left(nTemp);
			csTemp = csLine;
		}
		csLine = csTemp;
		csLine.TrimLeft();
		csLine.TrimRight();
		if (csLine.IsEmpty()) 
			continue;

		m_csFileContainer.Add(csLine);
	}

	IniFile.Close();

	GetAllSections();
	
	return TRUE;
}

BOOL CIniFile::WriteIniFile()
{
	bFileExist=IniFile.Open(m_sFileName, CFile::modeCreate | CFile::modeWrite);
	if(!bFileExist)
		return FALSE;
	
	CString strline;

	//��m_csFileContainerд��IniFile�ļ�
	for(int i = 0; i< m_csFileContainer.GetSize(); i++)
	{
		strline = m_csFileContainer[i];
		IniFile.WriteString(strline+"\n");
	}

	IniFile.Close();

	return bFileExist;
}

CString CIniFile::GetFileString(CString Section, CString Item, CString Value)
{
	if(bFileExist == FALSE || m_csFileContainer.GetSize() < 0)
		return Value;//�ļ��򿪳�����ļ�Ϊ�գ�����Ĭ��ֵ

	Section.MakeLower();
	Item.MakeLower();
	int i = 0,nTemp = -1;
	int iFileLines = m_csFileContainer.GetSize();
	CString strline,str;

	for(i=0; i<m_Section.GetSize(); i++)
	{
		if (Section == m_Section[i].csSecName) 
			break;
	}
	if (i == m_Section.GetSize())
		return "";
	else
		i = m_Section[i].nSecLine;
//	while(i<iFileLines)
//	{
//		strline = m_csFileContainer.GetAt(i++);
//
//		if (strline.IsEmpty()) 
//			continue;
//		
//		strline.TrimLeft();
//		if(strline.GetAt(0)=='[')//����Section����һ������Ϊ[
//		{
//			str=strline.Left(strline.Find("]"));//ȥ��]�ұ�
//			str=str.Right(str.GetLength()-str.Find("[")-1);//ȥ��[���
//
//			str.TrimLeft();
//			str.TrimRight();
//
//			str.MakeLower();
//			if(Section == str)//�ҵ�Section
			{
				while(i<iFileLines-1)
				{
					i++;
					strline = m_csFileContainer.GetAt(i);
					strline.TrimLeft();

					if(strline.GetAt(0)=='[')
						return Value;//���������һ��[]�����Ҳ���,����Ĭ��ֵ
					
					nTemp = 0;
					nTemp = strline.Find("=");
					str = strline.Left(nTemp);//ȥ��=�ұ�

					str.TrimLeft();
					str.TrimRight();
					str.MakeLower();
					if(Item == str)//�ҵ�Item
					{
						str=strline.Right(strline.GetLength()-nTemp-1);//ȥ��=���
						
						nTemp = 0;
						nTemp = str.Find("//");			// "//" ����Ϊע��
						if (nTemp >0)
							str = str.Left(nTemp);

						str.Remove(';');
						str.TrimLeft();
						str.TrimRight();
						return str;
					}
				}
				return Value;//�Ҳ���,����Ĭ��ֵ
			}

//		}

//	}
	return Value;//�Ҳ���,����Ĭ��ֵ
}

void CIniFile::WriteFileString(CString Section, CString Item, CString Value)
{
	int i = 0;
	int iFileLines = m_csFileContainer.GetSize();
	CString strline,str;

	while(i<iFileLines)
	{
		strline = m_csFileContainer.GetAt(i++);

		if (strline.IsEmpty()) 
			continue;

		strline.TrimLeft();
		if(strline.GetAt(0)=='[')//����Section����һ������Ϊ[
		{
			str=strline.Left(strline.Find("]"));//ȥ��]�ұ�
			str=str.Right(str.GetLength()-str.Find("[")-1);//ȥ��[���

			str.TrimLeft();
			str.TrimRight();

			if(Section == str)//�ҵ�Section
			{
				while(i<iFileLines)
				{
					strline = m_csFileContainer.GetAt(i++);
					strline.TrimLeft();

					if(strline.GetAt(0)=='[')//���������һ��[]�����Ҳ���Item
					{
						break;
					}
					
					str = strline.Left(strline.Find("="));//ȥ��=�ұ�

					str.TrimLeft();
					str.TrimRight();

					if(Item == str)//�ҵ�Item
					{
						strline = Item + "=" + Value;
						m_csFileContainer[i-1] = strline;
						return;
					}
				}
				//�Ҳ���Item
				strline = Item + "=" + Value;
				m_csFileContainer.InsertAt(i-1, strline);
				return;

			}

		}

	}
	//�Ҳ���Section
	//ֱ����������Section��Item��Value
	m_csFileContainer.Add("[" + Section + "]");
	m_csFileContainer.Add(Item + "=" + Value);
	return;
}

void CIniFile::GetAllSections()
{
	int i = 0;
	int iFileLines = m_csFileContainer.GetSize();
	CString strline;
	SECTION_STRUCT tmpSec;

	CString str;
	while(i<iFileLines)
	{
		strline = m_csFileContainer.GetAt(i++);

		strline.TrimLeft();
		strline.TrimRight();
		if(strline.Left(1)=='['
			&&strline.Right(1) == "]")//����Section����һ������Ϊ[,ĩβΪ]
		{
			strline.Delete(0);//ȥ��]�ұ�
			strline.Delete(strline.GetLength()-1);

			m_csAllSections.Add(strline);
			tmpSec.csSecName = strline;
			tmpSec.csSecName.MakeLower();
			tmpSec.nSecLine = i-1;
			m_Section.Add(tmpSec);
		}
	}
}

int CIniFile::GetAllKeysAndValues(CString strSection, CStringArray &strArrKey, CStringArray &strArrKeyValue)
{
	if(bFileExist == FALSE || m_csFileContainer.GetSize() < 0)
		return -1;//�ļ��򿪳�����ļ�Ϊ�գ�����Ĭ��ֵ

	strSection.MakeLower();
	int i = 0,nTemp = -1;
	int iFileLines = m_csFileContainer.GetSize();
	CString strline,str;

	while(i<iFileLines)
	{
		strline = m_csFileContainer.GetAt(i++);

		if (strline.IsEmpty()) 
			continue;
		
		strline.TrimLeft();
		if(strline.GetAt(0)=='[')//����strSection����һ������Ϊ[
		{
			str=strline.Left(strline.Find("]"));//ȥ��]�ұ�
			str=str.Right(str.GetLength()-str.Find("[")-1);//ȥ��[���

			str.TrimLeft();
			str.TrimRight();

			str.MakeLower();
			if(strSection == str)//�ҵ�strSection
			{
				strArrKey.RemoveAll();
				strArrKeyValue.RemoveAll();
				while(i<iFileLines)
				{
					strline = m_csFileContainer.GetAt(i++);
					strline.TrimLeft();

					if (strline.IsEmpty())
						continue;

					if(strline.GetAt(0)=='[')
						break;//���������һ��[]�����Ѿ�����
					
					nTemp = 0;
					nTemp = strline.Find("=");
					if (nTemp>0)
					{
						str = strline.Left(nTemp);//ȥ��=�ұ�
						
						str.TrimLeft();
						str.TrimRight();
						str.MakeLower();
						strArrKey.Add(str);

						str=strline.Right(strline.GetLength()-nTemp-1);//ȥ��=���						
						nTemp = 0;
						nTemp = str.Find("//");			// "//" ����Ϊע��
						if (nTemp >0)
							str = str.Left(nTemp);						
						str.Remove(';');
						str.TrimLeft();
						str.TrimRight();
						strArrKeyValue.Add(str);
					}
					else
					{
						nTemp = 0;
						str = strline;
						nTemp = strline.Find("//");			// "//" ����Ϊע��
						if (nTemp >0)
							str = strline.Left(nTemp);						
						str.Remove(';');
						str.TrimLeft();
						str.TrimRight();
						strArrKey.Add(str);
						strArrKeyValue.Add("");
					}
				}
				break;
			}
		}
	}        
	return strArrKey.GetSize();
}