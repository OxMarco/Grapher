#pragma once

class COSFDialog
{
public:
	COSFDialog();
	virtual ~COSFDialog();

	BOOL FileOpenDlg(PTSTR szFilter, PTSTR szDefExt, PTSTR szDlgTitle, BOOL fMultSel);
	BOOL FileSaveDlg(PTSTR szFilter, PTSTR szDefExt, PTSTR szDlgTitle);

	/*----- retrieving file full name ------------*/
	PCTSTR GetFileName()const { return m_szFileName;} 
	/*----- retrieving file title ----------------*/
	PCTSTR GetTitleName()const { return m_szTitleName; }
	PCTSTR GetNextFileName();
	BOOL   IsMultiSelect(){ return(m_ofn.Flags & OFN_ALLOWMULTISELECT);}



 	// members
private:
    OPENFILENAME  m_ofn;         
	TCHAR m_szFileName[MAX_PATH * 10];
	TCHAR m_szTitleName[MAX_PATH * 10];
    static const TCHAR szUntitled [10];
};



