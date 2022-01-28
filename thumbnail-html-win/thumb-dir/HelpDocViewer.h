#if !defined(AFX_HELPDOCVIEWER_H__59E54E85_F25A_4948_9232_5760D51FD2FD__INCLUDED_)
#define AFX_HELPDOCVIEWER_H__59E54E85_F25A_4948_9232_5760D51FD2FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpDocViewer.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CHelpDocViewer ダイアログ

class CHelpDocViewer : public CDialog
{
// コンストラクション
public:
	CHelpDocViewer(CWnd* pParent = NULL);   // 標準のコンストラクタ
	CFont fntEditbox;

// ダイアログ データ
	//{{AFX_DATA(CHelpDocViewer)
	enum { IDD = IDD_HELPDOC };
	CEdit	m_edit_ctrl;
	CString	m_edit;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHelpDocViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CHelpDocViewer)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HELPDOCVIEWER_H__59E54E85_F25A_4948_9232_5760D51FD2FD__INCLUDED_)
