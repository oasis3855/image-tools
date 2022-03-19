## 画像一括リサイズ スクリプト（サムネイルHTMLのCSV読込対応も可）<br />Image Resizer, Perl Script<!-- omit in toc -->

[Home](https://oasis3855.github.io/webpage/) > [Software](https://oasis3855.github.io/webpage/software/index.html) > [Software Download](https://oasis3855.github.io/webpage/software/software-download.html) > [image-tools](../README.md) > ***image-resize*** (this page)

<br />
<br />

Last Updated : Jul. 2011

- [ソフトウエアのダウンロード](#ソフトウエアのダウンロード)
- [機能の概要](#機能の概要)
- [依存関係](#依存関係)
- [バージョン履歴](#バージョン履歴)
- [ライセンス](#ライセンス)

## ソフトウエアのダウンロード

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [このGitHubリポジトリを参照する](../image-resize/) 

## 機能の概要

画像を一括リサイズして、別ディレクトリに書き出すスクリプトです。次のような機能を実装しています。

- 元画像ディレクトリの全ての画像をリサイズ（上書きON）
- 元画像ディレクトリの全ての画像をリサイズ（上書きOFF）
- 変換先画像ディレクトリに存在するすべての画像をリサイズ（上書きON）
- CSVの0カラム目の全てのファイル（が存在すれば）その画像をリサイズ
- CSVファイルでコピー制御が書きこまれているカラムによる制御

## 依存関係

制御用CSV読み込み機能に対応したCSVファイルは、[サムネイル HTML 作成スクリプト](../thumbnail-html/README.md) により作成されたものを前提としている。

## バージョン履歴

- Version 1.0 2011/01/13
    - 新規
- Version 2.0 2011/01/29
    - CSV読み込み機能
    - DirToの余剰ファイル削除機能
- Version 3.0 2011/07/20
    - Exif回転がある場合、回転フラグをリセットして書き込む

## ライセンス

このスクリプトは [GNU General Public License v3ライセンスで公開する](https://gpl.mhatta.org/gpl.ja.html) フリーソフトウエア


