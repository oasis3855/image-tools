## ![icon](readme_pics/softdown-ico-j6i.gif) J6I イメージコンバータ for RICOHデジカメDC-1/DC-2 for Windows<!-- omit in toc -->

---
[Home](https://oasis3855.github.io/webpage/) > [Software](https://oasis3855.github.io/webpage/software/index.html) > [Software Download](https://oasis3855.github.io/webpage/software/software-download.html) > [image-tools](../README.md) > ***j6i2jpg*** (this page)

<br />
<br />

Last Updated : Mar. 2000 -- ***this is discontinued software 開発終了***

- [ソフトウエアのダウンロード](#ソフトウエアのダウンロード)
- [機能の概要](#機能の概要)
  - [制限事項](#制限事項)
- [動作環境](#動作環境)
- [バージョンアップ履歴](#バージョンアップ履歴)
- [主な掲載雑誌](#主な掲載雑誌)
- [ライセンス](#ライセンス)

<br />
<br />

## ソフトウエアのダウンロード

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [このGitHubリポジトリを参照する](../j6i2jpg/download/) 

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [Googleドキュメントを参照する](https://drive.google.com/drive/folders/0B7BSijZJ2TAHNzVmZjQ0M2UtYTAyYi00YjQ5LTg5YjQtMjc3ZGJjNWY5NDNh?resourcekey=0-KAQmh4Mzs73kci9uJEXQPw) 

<br />
<br />

## 機能の概要

RICOH製デジタルカメラ DC-1/DC-2シリーズ（1997年ごろの製品) のJ6I形式画像ファイルをJPEGファイルに変換するするソフトウエア。    メーカ純正の「PC接続キット」を購入せずに、PCMCIAメモリーカード内のJ6Iデータを扱えるようになります。

![メインダイアログ](readme_pics/soft-j6iconverter-main.png)


JEIDA規格のヘッダ領域は次の図のようなもので、このソフトウエアでは、ヘッダおよびファイル長の統一のために加えられている余分な領域を削除します 

![J6Iヘッダの構造](readme_pics/soft-j6iconverter-fig.png#gh-light-mode-only)
![J6Iヘッダの構造](readme_pics/soft-j6iconverter-fig-darkmode.png#gh-dark-mode-only)

### 制限事項

このソフトウエアはシングルスレッドで作成されていますので、画像変換処理中は処理ウインドウの表示が更新されません。 

<br />
<br />

## 動作環境

- Windows 98/NT/2000で動作確認済み 

<br />
<br />

## バージョンアップ履歴

- Version 1.0 (1997/05/01)

  - 新規作成 （RICOH DC-2L購入のため） 

- Version 1.1β (1999/03/01)

  - [NEW] 連続変換のダイアログを共通ダイアログに変更（正常に機能せず） 

- Version 1.8 (2000/03/07)

  - [Bug Fix] 連続変換ダイアログのバグ修正 
  - [NEW] 保存先のダイアログを共通ダイアログに変更 
  - [NEW] インストール、アンインストールを装備 
  - [NEW] 年号強制変換（2000年問題対応） 
  - [NEW] ダイアログを表示せずに変換・プレビュー 
  - [NEW] ファイルの関連付けを自動化 
  - [NEW] JPGファイルのプレビューを、標準で関連付けられたプログラムで行える 

<br />
<br />

## 主な掲載雑誌

- ホームページデザイン (エーアイ出版) 
- ウインドウズ・パワー（アスキー） 

<br />
<br />

## ライセンス

このソフトウエアは [GNU General Public License v3ライセンスで公開する](https://gpl.mhatta.org/gpl.ja.html) フリーソフトウエア

※ 配布ソースコード等に書かれているライセンス条項は撤回し、上述GNU General Public License v3ライセンスにて配布する。
