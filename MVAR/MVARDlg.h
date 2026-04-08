
// MVARDlg.h : header file
//

#pragma once

#include <types.h>

// CMVARDlg dialog
class CMVARDlg : public CDialogEx
{
// Construction
public:
	CMVARDlg(CWnd* pParent = nullptr);	// standard constructor
    ~CMVARDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MVAR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


private:
    CString GetFileName( CString strFilePath );
    CString retrive_outfontPath(CString& source);
    
    bool make_mvar( MVARTable& mvar, INT axis_count, INT axis_index, INT& asc_min_delta, INT& asc_max_delta, BOOL& asc_mvar, INT& dsc_min_delta, INT& dsc_max_delta, BOOL& dsc_mvar );

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedButton1();
    CString m_fontPath;
    BOOL m_descender;
    CString m_outFontPath;
};
