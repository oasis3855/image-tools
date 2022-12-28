#include "stdafx.h"
#include "ShellLink.h"

// ファイルへのショートカット・アイコンを作成する。
// SDK では Shell Links と言う。
// pszShortcutFile : 実体ファイルのフルパス名（C:\WINDOWS\NOTEPAD.EXE）
// pszLink : ショートカットファイルのフルパス名（ C:\WINDOWS\….lnk）
// pszDesc : 表示名
//
// この項目に関しては、 MSDNライブラリから"Using Shell Links"のタイトルを検索する
// 関数の説明などは "The IShellLink Interface"にある
HRESULT CreateShellLink(LPCSTR pszShortcutFile,
							LPSTR pszLink, LPSTR pszDesc, LPSTR pszParam)
{
    HRESULT hres;
    IShellLink *psl;

	CoInitialize(NULL);	// MSDNサンプルファイルではここが抜けているので注意する

	// IShellLink オブジェクトを(pslに)得る
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                            IID_IShellLink, (void **)&psl);
    if (SUCCEEDED(hres))
    {
       IPersistFile *ppf;
       
	   // Query IShellLink for the IPersistFile interface for
	   // saving the shortcut in persistent storage.
       hres = psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
	   if (SUCCEEDED(hres))
	   {   
	        WORD wsz[MAX_PATH];   // buffer for Unicode string

	   		// 実体ファイル（ターゲット）のパスを設定
          	hres = psl->SetPath(pszShortcutFile);
/*
			if (!SUCCEEDED(hres))
				if(!n_NDispE)
					AfxMessageBox("ショートカット作成中にエラーを起こしました",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
*/
	   		// パラメータの設定を設定
			if(pszParam != "")
          		hres = psl->SetArguments(pszParam);

            // ショートカットの説明文(Description)をセット
			// 何のために使われる説明文か、不明？
          	hres = psl->SetDescription(pszDesc);
/*
			if (!SUCCEEDED(hres))
				if(!n_NDispE)
					AfxMessageBox("ショートカット作成中にエラーを起こしました",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
*/
	     	// ショートカットファイル名のMBCS を ワイドキャラクタ文字列へ
	     	MultiByteToWideChar(CP_ACP, 0, pszLink, -1, wsz, MAX_PATH);

          	// ショートカットファイルをディスクに書き込む
			// Save the shortcut via the IPersistFile::Save member function.
          	hres = ppf->Save(wsz, TRUE);
/*
			if (!SUCCEEDED(hres))
				if(!n_NDispE)
					AfxMessageBox("ショートカット作成中にエラーを起こしました",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
*/
            // Release the pointer to IPersistFile.
          	ppf->Release();
       }
       // Release the pointer to IShellLink.
       psl->Release();
    }
	CoUninitialize();
    return hres;

}
