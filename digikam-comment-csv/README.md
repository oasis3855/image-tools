## 画像管理ソフトdigiKamの画像コメントをSQLiteデータベースとの間でインポート／エクスポートするスクリプト (Linux)<!-- omit in toc -->

[Home](https://oasis3855.github.io/webpage/) > [Software](https://oasis3855.github.io/webpage/software/index.html) > [Software Download](https://oasis3855.github.io/webpage/software/software-download.html) > [image-tools](../README.md) > ***digikam-comment-csv*** (this page)

<br />
<br />

Last Updated : Jan. 2012 -- ***this is discontinued software 開発終了***

- [ソフトウエアのダウンロード](#ソフトウエアのダウンロード)
- [機能の概要](#機能の概要)
- [動作環境](#動作環境)
- [データ構造](#データ構造)
  - [スクリプトが読み書きするdigiKamのSQLiteテーブルの構造](#スクリプトが読み書きするdigikamのsqliteテーブルの構造)
  - [スクリプトがインポート／エクスポートするCSVファイルのサンプル例](#スクリプトがインポートエクスポートするcsvファイルのサンプル例)
- [バージョン履歴](#バージョン履歴)
- [ライセンス](#ライセンス)

<br />
<br />

## ソフトウエアのダウンロード

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [このGitHubリポジトリを参照する](../digikam-comment-csv/src/) 

<br />
<br />

## 機能の概要

画像処理ソフトウエア[digiKam](https://www.digikam.org/)の画像コメントを、CSVファイルから一括インポート・CSVファイルに一括エクスポートするスクリプト。

<br />
<br />

## 動作環境

- Ubuntu 10.04

<br />
<br />

## データ構造

### スクリプトが読み書きするdigiKamのSQLiteテーブルの構造

.schema Albums

```SQL
CREATE TABLE Albums
 (id INTEGER PRIMARY KEY,
  albumRoot INTEGER NOT NULL,
  relativePath TEXT NOT NULL,
  DATE DATE,
  caption TEXT,
  collection TEXT,
  icon INTEGER,
  UNIQUE(albumRoot, relativePath)
);
```

.schema Images

```SQL
CREATE TABLE Images
 (id INTEGER PRIMARY KEY,
  album INTEGER,
  name TEXT NOT NULL,
  STATUS INTEGER NOT NULL,
  category INTEGER NOT NULL,
  modificationDate DATETIME,
  fileSize INTEGER,
  uniqueHash TEXT,
  UNIQUE (album, name)
);
```

.schema ImageComments

```SQL
CREATE TABLE ImageComments
 (id INTEGER PRIMARY KEY,
  imageid INTEGER,
  TYPE INTEGER,
  LANGUAGE TEXT,
  author TEXT,
  DATE DATETIME,
  comment TEXT,
  UNIQUE(imageid, TYPE, LANGUAGE, author)
);
```

<br />
<br />

### スクリプトがインポート／エクスポートするCSVファイルのサンプル例

```CSV
DSC01031.jpg,"リューベック旧市街, マルクト広場"
DSC01032.jpg,"フロン・ド・セーヌの高層ビル群"
DSC01033.jpg,"ビル・アケム橋と地下鉄6号線"
DSC01034.jpg,"チェスケー・ブジェヨヴィツェのオタカル2世広場"
DSC01035.jpg,"ミラノのドゥオーモとガッレリア"
```

<br />
<br />

## バージョン履歴

- Version 1.0 (2012/01/28)

<br />
<br />

## ライセンス

このソフトウエアは [GNU General Public License v3ライセンスで公開する](https://gpl.mhatta.org/gpl.ja.html) フリーソフトウエア
