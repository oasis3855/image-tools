#if !defined(AFX_DIALOGMESSAGE_H__3C31B7F7_301F_4092_8282_2ED7507933B8__INCLUDED_)
#define AFX_DIALOGMESSAGE_H__3C31B7F7_301F_4092_8282_2ED7507933B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMessage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMessage ダイアログ

class CDialogMessage : public CDialog
{
// コンストラクション
public:
	CDialogMessage(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogMessage)
	enum { IDD = IDD_DLG_PREVIEW };
	CEdit	m_edit_main_ctrl;
	CString	m_edit_main_text;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogMessage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogMessage)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIALOGMESSAGE_H__3C31B7F7_301F_4092_8282_2ED7507933B8__INCLUDED_)
