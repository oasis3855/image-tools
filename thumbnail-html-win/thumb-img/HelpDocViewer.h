#if !defined(AFX_HELPDOCVIEWER_H__91DB7E41_47AE_406D_812B_401F70260B81__INCLUDED_)
#define AFX_HELPDOCVIEWER_H__91DB7E41_47AE_406D_812B_401F70260B81__INCLUDED_

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
	CFont fntEditbox;
	CHelpDocViewer(CWnd* pParent = NULL);   // 標準のコンストラクタ

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

#endif // !defined(AFX_HELPDOCVIEWER_H__91DB7E41_47AE_406D_812B_401F70260B81__INCLUDED_)
