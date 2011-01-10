#!/usr/bin/perl

# save this file in << UTF-8  >> encode !
# ******************************************************
# Software name : Make Thumbnail HTML （サムネイルHTML作成）
#
# Copyright (C) INOUE Hirokazu, All Rights Reserved
#     http://oasis.halfmoon.jp/
#
# thumbnail-dir.pl
# version 1.1 (2010/November/23)
# version 1.2 (2010/December/16)
# version 1.3 (2011/January/09)
#
# GNU GPL Free Software
#
# このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフトウェア財
# 団によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、希望によっては
# それ以降のバージョンのうちどれか)の定める条件の下で再頒布または改変することが
# できます。
# 
# このプログラムは有用であることを願って頒布されますが、*全くの無保証* です。
# 商業可能性の保証や特定の目的への適合性は、言外に示されたものも含め全く存在し
# ません。詳しくはGNU 一般公衆利用許諾契約書をご覧ください。
# 
# あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を一部受け取
# ったはずです。もし受け取っていなければ、フリーソフトウェア財団まで請求してく
# ださい(宛先は the Free Software Foundation, Inc., 59 Temple Place, Suite 330
# , Boston, MA 02111-1307 USA)。
#
# http://www.opensource.jp/gpl/gpl.ja.html
# ******************************************************

use strict;
use warnings;
use utf8;

my $flag_os = 'linux';	# linux/windows
my $flag_charcode = 'utf8';		# utf8/shiftjis

use Encode::Guess qw/euc-jp shiftjis iso-2022-jp/;	# 必要ないエンコードは削除すること
use Switch;
use POSIX;		# mktime用
use File::Find::Rule;
use File::Basename;
use Image::Magick;
use Image::ExifTool;
use Image::Size;
use pQuery;
use HTML::Scrubber;
use HTML::TagParser;
use Time::Local;
use File::Copy;

# use Data::Dumper;

# IOの文字コードを規定
if($flag_charcode eq 'utf8'){
	binmode(STDIN, ":utf8");
	binmode(STDOUT, ":utf8");
	binmode(STDERR, ":utf8");
}
if($flag_charcode eq 'shiftjis'){
	binmode(STDIN, "encoding(sjis)");
	binmode(STDOUT, "encoding(sjis)");
	binmode(STDERR, "encoding(sjis)");
}


my $strBaseDir = './';		# 基準ディレクトリ
my $strImageRelativeDir = '';		# 画像ディレクトリを1つに限定する場合に利用
my $strThumbRelativeDir = 'thumb/';	# サムネイル格納ディレクトリ
my $strOutputHTML = './index.html';	# 出力HTML（基準ディレクトリに出力）
my $nLongEdge = 150;		# サムネイルの長辺ピクセル数（ImageMagickで縮小時に利用）
my $nFindMinDepth = 2;		# File::Find::Ruleでの検索深さ（デフォルトは1段目のみ）
my $nFindMaxDepth = 2;		# File::Find::Ruleでの検索深さ（デフォルトは1段目のみ）

my $flag_read_html = 0;		# 0:既存HTMLを読まない, 1:既存HTMLを全て読み込む, 2:コメントのみ読み込む
my $flag_read_encode = '';	# 読み込むファイルのエンコード形式を強制指定する場合
my $flag_overwrite = 0;		# サムネイルを作成するときに、既存ファイルに上書きするフラグ
my $flag_verbose = 0;		# 詳細表示するフラグ
my $flag_sort_order = 'file-name';	# ソート順
my $flag_copy_prev = 1;		#「 空白時、前行値のコピーを行う」スイッチ (0:Off, 1:Comment1, 2:Comment1+2)
my $flag_html_style = 'line-style';	# line-style:1行1画像table, grid-style:画像をグリッド表示

my @arrImageFiles = ();		# 画像ファイルを格納する配列


# ファイル検索のパターン
my @arrFileScanMask;
if($flag_os eq 'linux'){
	@arrFileScanMask = ('*.jpg', '*.jpeg', '*.png', '*.gif', '*.JPG', '*.JPEG');}
if($flag_os eq 'windows'){
	# Windowsの場合は、ファイル名の大文字と小文字は同一に扱われているようだ
	@arrFileScanMask = ('*.jpg', '*.jpeg', '*.png', '*.gif');
}

# HTML出力時のファイル名で省略する拡張子
my @arrKnownSuffix = ('.jpg', '.jpeg', '.png', '.gif', '.JPG', '.JPEG');

print("\n".basename($0)." - サムネイルHTML作成 Perlスクリプト\n\n");

sub_user_input_init();	# 初期データの入力
if(sub_confirm_init_data() != 1){
	die("終了（ユーザによるキャンセル）\n");
}

if($flag_read_html != 0){ sub_parse_html(); }	# 既存HTMLを全て読み込む

sub_scan_imagefiles();
sub_sort_imagefiles();
sub_disp_files();	# 入力確認

sub_make_thumbnail();

if($flag_read_html != 0){
	for(my $i=0; $i<1000; $i++){
		my $strBackupFile = sprintf("%s\.%03d",$strOutputHTML,$i);
		if(-e sub_conv_to_local_charset($strBackupFile)){ next; }
		File::Copy::copy(sub_conv_to_local_charset($strOutputHTML), sub_conv_to_local_charset($strBackupFile)) or next;
		print("バックアップファイル ".$strBackupFile." を作成しました\n");
		last;
	}
}

sub_create_html();

print("正常終了\n");

exit();


# 初期データの入力
sub sub_user_input_init {

	# プログラムの引数は、対象ディレクトリとする
	if($#ARGV == 0 && length($ARGV[0])>1)
	{
		$strBaseDir = sub_conv_to_flagged_utf8($ARGV[0]);
	}

	# 基準ディレクトリの入力
	print("基準ディレクトリを、絶対または相対ディレクトリで入力。\n（例：/home/user/, ./）");
	if(length($strBaseDir)>0){ print("[$strBaseDir] :"); }
	else{ print(":"); }
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0){
		if(length($strBaseDir)>0){ $_ = $strBaseDir; }	# スクリプトの引数のデフォルトを使う場合
		else{ die("終了（理由：ディレクトリが入力されませんでした）\n"); }
	}
	if(substr($_,-1) ne '/'){ $_ .= '/'; }	# ディレクトリは / で終わるように修正
	unless(-d sub_conv_to_local_charset($_)){ die("終了（理由：ディレクトリ ".$_." が存在しません）\n"); }
	$strBaseDir = $_;
	print("基準ディレクトリ : " . $strBaseDir . "\n\n");


	# 対象ディレクトリを限定する場合の入力
	print("画像があるディレクトリを限定する場合のディレクトリを入力\n改行のみで全てのディレクトリを対象とします（例：image/）： ");
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0){
		$strImageRelativeDir = undef;
		print("（基準ディレクトリ以下の）全てのディレクトリの画像ファイル対象とします\n\n");
	}
	else{
		if(substr($_,0,1) eq '/' || substr($_,0,2) eq './'){ die("終了（理由：/ や ./ で始まらない相対ディレクトリを入力してください）\n"); }
		if(substr($_,-1) ne '/'){ $_ .= '/'; }	# ディレクトリは / で終わるように修正
		unless(-d sub_conv_to_local_charset($strBaseDir.$_)){ die("終了（理由：ディレクトリ ".$_." が存在しません）\n"); }
		$strImageRelativeDir = $_;
		print("画像ディレクトリの限定（基準ディレクトリからの相対） : " . $strImageRelativeDir . "\n\n");
	}

	# File::Find::Ruleでの検索深さの入力
	if(!defined($strImageRelativeDir)){
		print("画像ディレクトリの検索深さの開始値。基準ディレクトリを1とする。\n (1-10) [2]： ");
		$_ = <STDIN>;
		chomp();
		if(length($_)<=0){ $_ = 2; }
		if(int($_)<1 || int($_)>10){ die("終了（入力範囲は 1 - 10 です）\n"); }
		$nFindMinDepth = int($_);

		print("画像ディレクトリの検索深さの終了値。基準ディレクトリを1とする。\n ($nFindMinDepth-10) [$nFindMinDepth] ： ");
		$_ = <STDIN>;
		chomp();
		if(length($_)<=0){ $_ = $nFindMinDepth; }
		if(int($_)<$nFindMinDepth || int($_)>10){ die("終了（入力範囲は $nFindMinDepth - 10 です）\n"); }
		$nFindMaxDepth = int($_);
	
		print("画像ディレクトリの検索深さ : $nFindMinDepth - $nFindMaxDepth\n\n");
	}


	# サムネイル ディレクトリの入力（無い場合は、新規作成）
	print("サムネイル ディレクトリ名（例：thumb/）[thumb]： ");
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0){ $_ = 'thumb'; }
	if(substr($_,0,1) eq '/' || substr($_,0,2) eq './'){ die("終了（理由：/ や ./ で始まらない相対ディレクトリを入力してください）\n"); }
	if(substr($_,-1) ne '/'){ $_ .= '/'; }	# ディレクトリは / で終わるように修正
	print("サムネイル ディレクトリ（画像ディレクトリ下） : " . $_ . "\n\n");
	$strThumbRelativeDir = $_;

	# サムネイル画像のサイズを入力する
	print("サムネイル画像の長辺ピクセル (10-320) [180]： ");
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0){ $_ = 180; }
	if(int($_)<10 || int($_)>320){ die("終了（入力範囲は 10 - 320 です）\n"); }
	$nLongEdge = int($_);
	print("サムネイルの長辺（px） : " . $nLongEdge . "\n\n");

	# サムネイル作成時の上書き設定
	print("サムネイル作成時に、ファイルがすでにある場合上書きする (Y/N) [N]：");
	$_ = <STDIN>;
	chomp();
	if(uc($_) eq 'Y'){
		$flag_overwrite = 1;
		print("既存のサムネイルファイルには上書きします\n\n");
	}
	elsif(uc($_) eq 'N' || length($_)<=0){
		$flag_overwrite = 0;
		print("既存のサムネイルファイルがある場合は、それを使います（上書き無し）\n\n");
	}
	else{
		die("終了（Y/Nの選択肢以外が入力された）\n");
	}

	# 出力HTMLファイル名の入力
	print("出力HTMLファイル名（例：index.html）： ");
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0){ die("終了（理由：ファイル名が入力されませんでした）\n"); }
	if($_ =~ /\//){ die("終了（理由：ファイル名に / が入っています）\n"); }
	if(-f sub_conv_to_local_charset($strBaseDir . $_) && -w sub_conv_to_local_charset($strBaseDir . $_)){
		print("出力HTMLファイル名（既存HTMLのアップデート） : " . $_ . "\n\n");
		$flag_read_html = 1;	# 既存ファイルデータがあることを示すフラグ
	}
	elsif(-f sub_conv_to_local_charset($strBaseDir . $_)){
		die("終了（理由：出力HTMLファイル " . $_ . " に書き込めません）\n");
	}
	else{
		print("出力HTMLファイル名（新規作成） : " . $_ . "\n\n");
	}
	$strOutputHTML = $strBaseDir . $_;

	# 既存HTMLを読み込むかどうか
	if($flag_read_html == 1){
		print("既存HTMLファイルを読み込んで反映しますか\n 0: 読み込まない（新たにつくる）\n 1: 全て読み込む\n 2: コメントのみ一致するJPEGがあれば読み込む\n 選択してください (0-2) [2]：");
		$_ = <STDIN>;
		chomp;
		if(length($_)<=0){  $flag_read_html = 2; }
		elsif(uc($_) eq '0'){ $flag_read_html = 0; }
		elsif(uc($_) eq '1'){ $flag_read_html = 1; }
		elsif(uc($_) eq '2'){ $flag_read_html = 2; }
		else { die("選択肢 0-2 以外が入力されたため終了します") }

		if($flag_read_html == 0){ print("既存HTMLの読み込み : OFF\n\n"); }
		if($flag_read_html == 1){ print("既存HTMLの読み込み : ON（全データ）\n\n"); }
		if($flag_read_html == 2){ print("既存HTMLの読み込み : ON（コメントのみ）\n\n"); }
	}

	# コメント欄が空白の場合、前行のデータで保管するかの選択
	if($flag_read_html != 0) {
		printf("既存HTML読み込み時、空白項目は前行の値をコピーしますか？\n1:Comment1（日時の右隣）のみ対象\n2:Comment1 & Comment2（コメント欄2つ全て）対象\nN:コピーしない（空白の場合も元のまま）\n選択してください (1/2/N) [N] : ");
		$_ = <STDIN>;
		chomp;
		if(length($_)<=0){  $flag_copy_prev = 0; }
		elsif(uc($_) eq '1'){ $flag_copy_prev = 1; }
		elsif(uc($_) eq '2'){ $flag_copy_prev = 2; }
		elsif(uc($_) eq 'N'){ $flag_copy_prev = 0; }
		else { die("選択肢 1/2/N 以外が入力されたため終了します") }

		if($flag_copy_prev == 0){ print("空白項目は放置します（コピー機能無し）\n\n"); }
		if($flag_copy_prev == 1){ print("Comment 1が空白の場合、前行をコピーします\n\n"); }
		if($flag_copy_prev == 2){ print("Comment 1,2が空白の場合、前行をコピーします\n\n"); }
	}

	# 読み込むHTMLファイルのエンコード形式の強制指定
	if($flag_read_html != 0) {
		printf("ファイル読み込み時の文字コードの強制指定をしますか\n 0:自動判定（強制指定しない）\n 1:UTF-8\n 2:Shift JIS (cp932)\n 3:iso-2022-jp (JIS)\n 4:EUC-JP\n 5:UTF-16\n 6:UTF-32\n 選択してください (0-6) [0] : ");
		$_ = <STDIN>;
		chomp;
		if(length($_)<=0 || $_ eq '0'){  $flag_read_encode = ''; }
		elsif($_ eq '1'){ $flag_read_encode = 'utf8'; }
		elsif($_ eq '2'){ $flag_read_encode = 'shiftjis'; }
		elsif($_ eq '3'){ $flag_read_encode = 'iso-2022-jp'; }
		elsif($_ eq '4'){ $flag_read_encode = 'euc'; }
		elsif($_ eq '5'){ $flag_read_encode = 'utf16'; }
		elsif($_ eq '6'){ $flag_read_encode = 'utf32'; }
		else{ die("0-6 以外が入力されました\n"); }

		print("文字コードの強制指定 : ". ($flag_read_encode eq '' ? 'OFF' : $flag_read_encode) . "\n\n");
	}

	# ソート順の選択
	print("ソート順を選択\n1: ファイル順（A...Z）\n2: ファイル順（Z...A）\n3: Exif/タイムスタンプ順（過去->未来）\n4: Exif/タイムスタンプ順（未来->過去）\n (1-4) ?  [1]：");
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0){ $_ = 1; }
	if(int($_)<1 || int($_)>4){ die("終了（入力範囲は 1 - 4 です）\n"); }
	switch(int($_)){
		case 1	{ $flag_sort_order='file-name'; }
		case 2	{ $flag_sort_order='file-name-reverse'; }
		case 3	{ $flag_sort_order='file-date'; }
		case 4	{ $flag_sort_order='file-date-reverse'; }
		else	{ $flag_sort_order='file-name'; }
	}
	print("ソート順 : " . $flag_sort_order . "\n\n");


	# HTML形式の選択
	print("HTMLレイアウトの選択\n 1: 1画像 1行のtable（再読込対応版）\n 2: 画像グリッド（再読込不可）\n (1/2) ? [1] ：");
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0 || $_ eq '1'){ $flag_html_style = 'line-style'; }
	elsif($_ eq '2'){ $flag_html_style = 'grid-style'; }
	else{ die("終了（入力範囲は 1/2 です）\n"); }
	print("HTMLレイアウト : " . $flag_html_style . "\n\n");


}


# 初期設定の確認
sub sub_confirm_init_data {

	printf("\n===============\n".
		"基準ディレクトリ：%s\n".
		"画像ディレクトリ：%s\n".
		"%s".
		"サムネイルディレクトリ：%s\n".
		"サムネイル長辺：%d px\n".
		"サムネイル強制上書き：%s\n".
		"出力HTMLファイル名：./%s\n".
		"ソート順：%s\n",
		$strBaseDir,
		defined($strImageRelativeDir) ? $strBaseDir . $strImageRelativeDir : '全てのディレクトリ',
		defined($strImageRelativeDir) ? '' : '検索深さの設定：'.$nFindMinDepth.' - '.$nFindMaxDepth."\n",
		$strBaseDir . $strThumbRelativeDir,
		$nLongEdge,
		$flag_overwrite == 1 ? 'ON' : 'OFF',
		$strOutputHTML,
		$flag_sort_order);

	# Y/N 確認
	print("この内容で処理しますか (y/N)：");
	$_ = <STDIN>;
	chomp();
	if(uc($_) eq 'Y'){
		return(1);
	}
	elsif(uc($_) eq 'N' || length($_)<=0){
		return(0);
	}
	else{
		die("終了（Y/Nの選択肢以外が入力された）\n");
	}

}


# 対象画像ファイルを配列に格納する
sub sub_scan_imagefiles {

	my @arrScan = undef;	# ファイル一覧を一時的に格納する配列
	my $tmpDate = undef;	# UNIX秒（ファイル/Exifのタイムスタンプ）
	my $nCountNewfile = 0;		# 新規追加されたファイル数を数える
	my $nCountUpdatefile = 0;		# 新規追加されたファイル数を数える
	my $exifTool = Image::ExifTool->new();
#	$exifTool->Options(DateFormat => "%s", StrictDate=> 1);		# Windows版ActivePerlでは%sはサポート外
	$exifTool->Options(DateFormat => "%Y,%m,%d,%H,%M,%S", StrictDate=> 1);

	if(defined($strImageRelativeDir)){
		my $strScanPattern = '';
		foreach(@arrFileScanMask){
			if(length($strScanPattern)>1 && substr($strScanPattern,-1) ne ' '){$strScanPattern .= ' ';}
			$strScanPattern .= $strBaseDir.$strImageRelativeDir.$_;
		}
		@arrScan = glob(sub_conv_to_local_charset($strScanPattern));
	}
	else{
		@arrScan = File::Find::Rule->file->name(@arrFileScanMask)->maxdepth($nFindMaxDepth)->mindepth($nFindMinDepth)->in(sub_conv_to_local_charset($strBaseDir));
	}

	foreach(@arrScan)
	{
		if(length($_) <= 0){ next; }
		$_ = sub_conv_to_flagged_utf8($_);
		if($_ =~ /$strThumbRelativeDir$/ || $_ =~ /$strThumbRelativeDir\/$/){ next; }
		my $strFullPath = $_;
		my ($basename, $path, $ext) = File::Basename::fileparse($strFullPath, @arrKnownSuffix);
		$path =~ s/^\.\///g;	# 先頭の ./ を削除
		# pathからstrBasenameを除去
		my $str = $strBaseDir;
		$str =~ s/^\.\///g;	# 先頭の ./ を削除
		$path =~ s/^$str//g;	# パス名から基準ディレクトリを取る
		# サムネイルファイルに付けるpath文字列を抽出
		my $dirname = $path;
		$dirname =~ s/\/$//g;		# 末端の / を除去

		# 既存ファイル（HTMLまたはCSV）に一致データがある場合、そのインデックス（一致しない場合は -1）
		my $nMatchLine = -1;
		if($flag_read_html != 0){ $nMatchLine = sub_check_match_file($dirname.'/'.$basename.$ext); }

		# Exif日時を読み込む
		$exifTool->ImageInfo(sub_conv_to_local_charset($strFullPath));
		$tmpDate = $exifTool->GetValue('CreateDate');
		if(!defined($tmpDate)){ $tmpDate = (stat(sub_conv_to_local_charset($strFullPath)))[9]; }	# Exifが無い場合は最終更新日
		else{
			my @arrTime_t = split(/,/,$tmpDate);
			$tmpDate = mktime($arrTime_t[5], $arrTime_t[4], $arrTime_t[3], $arrTime_t[2], $arrTime_t[1]-1, $arrTime_t[0]-1900);
		}

		# 回転情報を得る（1:0 deg, 3:180 deg, 6:90 deg(CW), 8:270 deg (CW))
		my $exifRotate = int($exifTool->GetValue('Orientation', 'Raw'));
		if(!defined($exifRotate) || $exifRotate == 0){ $exifRotate = 1; }

		if($nMatchLine >= 0 && $flag_read_html == 1){
			# 全てのデータを移行する場合
			$arrImageFiles[$nMatchLine][7] = $exifRotate;		# [7]: Exif回転情報
		}
		elsif($nMatchLine < 0){
			# 新しいデータを追加する
			my @arrTemp = ($strFullPath,		# [0]: 画像ファイルへのパス（dir + basename）
					$dirname,	# [1]: 画像ファイルの相対dir ($strBaseDirと末尾の/を除去済み）
					$basename.$ext,	# [2]: 画像ファイルのbasename
					$dirname . '/' . $strThumbRelativeDir . $basename.$ext,	# [3]: サムネイルの相対パス
					$tmpDate,	# [4]: unix秒
					'',		# [5]: comment 1
					'',		# [6]: comment 2
					$exifRotate);		# [7]: Exif回転情報
			push(@arrImageFiles, \@arrTemp);
			$nCountNewfile++;
		}
		else{
			# 既存ファイル内のコメントは流用、サムネイルパスと日時をアップデート
			$arrImageFiles[$nMatchLine][3] = $dirname . '/' 
					. $strThumbRelativeDir . $basename.$ext,	# [3]: サムネイルの相対パス
			$arrImageFiles[$nMatchLine][4] = $tmpDate;	# [4]: unix秒
			$arrImageFiles[$nMatchLine][7] = $exifRotate;		# [7]: Exif回転情報
			$nCountUpdatefile++;
		}
	}
	
	print($nCountNewfile." 個のファイルを新規追加、".$nCountUpdatefile." 個のファイルの情報をアップデートしました\n\n");

}


# 対象画像ファイルの配列をソートする
sub sub_sort_imagefiles {

	# まず、ディレクトリを A...Z でソートし、ディレクトリの内部を指定された条件でソートする

	switch($flag_sort_order){
		case 'file-name'	{
			@arrImageFiles = sort { @$a[1] cmp @$b[1] || @$a[2] cmp @$b[2] } @arrImageFiles;
		}
		case 'file-name-reverse'	{
			@arrImageFiles = sort { @$a[1] cmp @$b[1] || @$b[2] cmp @$a[2] } @arrImageFiles;
		}
		case 'file-date'	{
			@arrImageFiles = sort { @$a[1] cmp @$b[1] || @$a[4] cmp @$b[4] } @arrImageFiles;
		}
		case 'file-date-reverse'	{
			@arrImageFiles = sort { @$a[1] cmp @$b[1] || @$b[4] cmp @$a[4] } @arrImageFiles;
		}
	}

}

# 対象ファイルのデバッグ表示
sub sub_disp_files {

	if($flag_verbose == 1){
		foreach(@arrImageFiles){
			my @tm = localtime($_->[4]);
			printf("%s, %s, %s, %04d/%02d/%02d %02d:%02d:%02d\n", $_->[1], $_->[2], $_->[0],
				$tm[5]+1900, $tm[4]+1, $tm[3], $tm[2], $tm[1], $tm[0]);
		}
	}

	printf("対象画像が %d 個見つかりました\n", $#arrImageFiles + 1);
}


# サムネイル画像の作成
sub sub_make_thumbnail {

	my $image = Image::Magick->new();
	my $image_check = undef;

	my $strFilenameInput = undef;
	my $strFilenameOutput = undef;

	print("サムネイル作成中 ...\n");

	eval{

		my $nCountWrite = 0;
		my $nCountSkip = 0;
		my $nCountError = 0;
		foreach(@arrImageFiles)
		{
			$strFilenameInput = $_->[0];		# 画像ファイルへのフルパス
			chomp($strFilenameInput);
			if(length($strFilenameInput) <= 0){ next; }
			$strFilenameOutput = $strBaseDir . $_->[3];	# サムネイル画像ファイルへのフルパス
			my $exifRotate = $_->[7];		# exif回転情報

			unless(-d sub_conv_to_local_charset(dirname($strFilenameOutput))){
				mkdir(sub_conv_to_local_charset(dirname($strFilenameOutput))) or die("サムネイルディレクトリ".dirname($strFilenameOutput)."が作成できない\nプログラムを終了します\n");
			}

			if(-e sub_conv_to_local_charset($strFilenameOutput) && $flag_overwrite == 0)
			{
				if($flag_verbose == 1){ print("サムネイル :" . $strFilenameOutput. " は既存\n"); }
				$nCountSkip++;
				next;
			}

			@$image = ();		# 読み込まれている画像をクリア

			$image_check = $image->Read($strFilenameInput);
			if($image_check)
			{
				print("\n画像ファイル :" . $strFilenameInput. " の読み込み不能\n");
				$nCountError++;
				next;
			}

			my ($nWidth, $nHeight) = $image->Get('width', 'height');
			if($nWidth <= 0 || $nHeight <= 0){ die("ジオメトリ読み込み失敗"); }
			my $nNewWidth = $nWidth > $nHeight ? $nLongEdge : int($nLongEdge*$nWidth/$nHeight);
			my $nNewHeight = $nHeight > $nWidth ? $nLongEdge : int($nLongEdge*$nHeight/$nWidth);

#			$image->AdaptiveResize(width=>$nNewWidth, height=>$nNewHeight);
			$image->Thumbnail(width=>$nNewWidth, height=>$nNewHeight);
			$image->Sharpen(radius=>0.0, sigma=>1.0);
			# exif情報による回転
			if($exifRotate == 3){$image->Rotate(degrees=>180.0); }
			if($exifRotate == 6){$image->Rotate(degrees=>90.0); }
			if($exifRotate == 8){$image->Rotate(degrees=>270.0); }
			$image->Set(quality=>90);
			$image_check = $image->Write(sub_conv_to_local_charset($strFilenameOutput));
			if($image_check)
			{
				print("\nサムネイル :" . $strFilenameOutput. " の書き込み不能\n");
				$nCountError++;
				next;
			}
			if($nCountWrite % 10 == 0){ print("作成中 ... ".$nCountWrite."\n"); }
			$nCountWrite++;

		}
		print("\nサムネイル作成処理 成功：".$nCountWrite.", 既存スキップ：".$nCountSkip.", エラー：".$nCountError."\n");

	};
	if($@){
		# evalによるエラートラップ：エラー時の処理
		print("プログラム エラー : ".$@."\n");
		@$image = ();
		undef $image;
		exit();
	}

	@$image = ();
	undef $image;

}


# HTMLファイルの出力
sub sub_create_html {


	print("出力HTMLファイル : " . $strOutputHTML . "\n");

	eval{	
		open(FH_OUT, '>'.sub_conv_to_local_charset($strOutputHTML)) or die;
		binmode(FH_OUT, ":utf8");

		printf(FH_OUT "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n" .
			"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"ja\" lang=\"ja\" dir=\"ltr\">\n" .
			"<head>\n" .
			"  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n" .
			"  <title></title>\n" .
			"  <style type=\"text/css\">\n<!--\n" .
			"  table {" .
			"      border:1px solid #aaa;" .
			"      border-collapse:collapse;" .
			"      font-size: 10pt;" .
			"      margin: 10px;" .
			"  }" .
			"  th {" .
			"      font-weight: normal;" .
			"      background:#f8ede2;" .
			"      border:1px solid #aaa;" .
			"      padding: 0.2em 0.4em;" .
			"  }" .
			"  td {" .
			"      border:1px solid #aaa;" .
			"      padding: 0.2em 0.4em;" .
			"  }" .
			"  a img {" .
			"      border: 0;" .
			"      margin: 0;" .
			"      padding: 0;" .
			"  }" .
			"  div.gallerybox {".
			"      display: block;".
			"      position: relative;".
			"      float: left;".
			"      margin: 5px;".
			"      min-width: 50px;".
			"      min-height: 50px;".
			"      font-size: 10pt;".
			"  }".
			"-->\n  </style>\n" .
			"</head>\n" .
			"<body>\n" .
			"<p>%d files</p>\n",
			$#arrImageFiles + 1);

		if($flag_html_style eq 'line-style') {
			# 1行1画像形式のとき
			printf(FH_OUT "<table>\n  <tr><th>dir</th><th>file</th><th>thumbnail</th><th>time</th><th>comment 1</th><th>comment 2</th></tr>\n");
		}
		
		foreach(@arrImageFiles)
		{
			my $strFilenameInput = $_->[1] . '/' . $_->[2];		# 画像への相対パス
			my @tm = localtime($_->[4]);
			chomp($strFilenameInput);
			if(length($strFilenameInput) <= 0){ next; }
			my $strFilenameOutput = $_->[3];	# サムネイル画像への相対パス
#				$strFilenameOutput =~ s/^\.\///g;	# 先頭の ./ を削除
			my @arrSize = imgsize(sub_conv_to_local_charset($strBaseDir . $strFilenameOutput));
			if(!defined($arrSize[0]) || !defined($arrSize[1])){ @arrSize = (0,0); }
			if($flag_html_style eq 'line-style') {
				printf(FH_OUT "  <tr><td>%s</td><td>%s</td><td><a href=\"%s\"><img src=\"%s\" alt=\"\" width=\"%d\" height=\"%d\" /></a></td><td>%04d/%02d/%02d %02d:%02d:%02d</td><td>%s</td><td>%s</td></tr>\n",
					dirname($strFilenameInput),
					basename($strFilenameInput, @arrKnownSuffix),
					$strFilenameInput,	# [0]: 画像へのパス
					$strFilenameOutput,	# [3]: サムネイル画像へのパス
					$arrSize[0], $arrSize[1],
					$tm[5]+1900, $tm[4]+1, $tm[3], $tm[2], $tm[1], $tm[0],	# [4] : unix秒
					$_->[5],	# [5]: comment 1
					$_->[6]);	# [6]: comment 2
			}
			elsif($flag_html_style eq 'grid-style') {
				printf(FH_OUT "<div class=\"gallerybox\" style=\"width:%dpx; height:%dpx;\"><div class=\"g-photo\"><a href=\"%s\"><img src=\"%s\" alt=\"\" width=\"%d\" height=\"%d\" /></a></div><div class=\"g-date\">%04d/%02d/%02d %02d:%02d:%02d</div><div class=\"g-comment1\">%s</div><div class=\"g-comment2\">%s</div></div>\n",
					$arrSize[0] > $arrSize[1] ? $arrSize[0]+10 : $arrSize[1]+10,
					$arrSize[0] > $arrSize[1] ? $arrSize[0]+40 : $arrSize[1]+40,
					$strFilenameInput,
					$strFilenameOutput,
					$arrSize[0], $arrSize[1],
					$tm[5]+1900, $tm[4]+1, $tm[3], $tm[2], $tm[1], $tm[0],	# [4] : unix秒
					$_->[5],	# [5]: comment 1
					$_->[6]);	# [6]: comment 2
			}

		}
		if($flag_html_style eq 'line-style') { print(FH_OUT "</table>\n"); }
		
		print(FH_OUT "</body>\n</html>\n");

		close(FH_OUT);


	};
	if($@){
		# evalによるエラートラップ：エラー時の処理
		print("プログラム エラー : ".$@."\n");
		exit();
	}


}


# HTMLファイルを読み込んで、CSVデータに切り分ける
# 
# thumb-html2csv/pl の関数を流用
sub sub_parse_html {

	my @arrCsvRaw = (); # CSV_XSに渡すCSV作成用の配列
	my @arrCsvPrevLine = ();	# 1行前のデータを保存する（空白桁補完用）
	my $flag_indata = 0;	# A LINKを検出したら1。この値が1の時、CSVデータ対象
	my $scrubber = HTML::Scrubber->new();
	$scrubber->allow(qw[ br ]);	# <br> タグは通過させる

	my $strTemp = undef;

	my $enc = undef;
	if($flag_read_encode ne ''){ $enc = $flag_read_encode; }
	else{ $enc = sub_get_encode_of_file($strOutputHTML); }
	if($enc eq ''){
		print("入力ファイルのエンコードが正しく判定できませんでした。$flag_charcode で読み込みます\n");
		$enc = $flag_charcode;
	}

	pQuery(sub_conv_to_local_charset($strOutputHTML))->find("tr")->each( sub{
		@arrCsvRaw = ();
		$flag_indata = 0;
		pQuery($_)->find("td")->each( sub{
			$strTemp = sub_conv_to_flagged_utf8($_->innerHTML(), $enc);
			$strTemp =~ s/\x0D\x0A|\x0D|\x0A/<br \/>/g; # 改行の除去
			$strTemp =~ s/\x09/\x20/g; # タブをスペースに変換
			$strTemp =~ s/\x20+/\x20/g; # 連続したスペースの統合

			# <td></td>間に通常の"文字列"が存在する場合
			if(length($scrubber->scrub($strTemp))>0){
				if($flag_indata == 1){
					# HTML文字列からタグを取り除く
					$strTemp = $scrubber->scrub($strTemp);

					# 文字列の調整
					$strTemp =~ s/　/ /g;	# 全角空白→半角空白
					$strTemp =~ s/<br><\/br>/<br>/g;		# ActivePerlの場合<br></br>→<br>
					$strTemp =~ s/[ ]*<br>[ ]*/<br>/g;		# <br>前後の空白文字を削除
					$strTemp =~ s/<br><br>|<br><br><br>/<br>/g;		# 連続<br>を1個に
					$strTemp =~ s/<br>$//g;		# 行末の<br>を除去
					$strTemp =~ s/^<br>//g;		# 行頭の<br>を除去
					if($strTemp eq ' '){ $strTemp = ''; }		# "空白1文字のみ"は切り捨て

					# 日時文字列をUNIX秒に変換
					#  (YYYY/MM/DD HH:MM → 16文字、YYYY/MM/DD<br>HH:MM:SS → 22文字）
					if(length($strTemp)>=16 && length($strTemp)<=22){
						my $strDate = $strTemp;
						$strDate =~ s/<br>/ /g;	# <br>を除去して空白文字に
						# まず、YYYY/MM/DD HH:MM:SS 形式で解析
						my($year,$mon,$day, $hour, $min, $sec) =
							($strDate =~ /(\d{4})\/(\d\d)\/(\d\d) (\d\d):(\d\d):(\d\d)/);
						if(defined($year)){
							$strTemp = timelocal($sec,$min,$hour,$day,$mon-1,$year)
						}
						else{
							# 次に、YYYY/MM/DD HH:MM 形式で解析
							($year,$mon,$day, $hour, $min) =
								($strDate =~ /(\d{4})\/(\d\d)\/(\d\d) (\d\d):(\d\d)/);
							if(defined($year)){
								$strTemp = timelocal(0,$min,$hour,$day,$mon-1,$year)
							}
						}
					}
					push(@arrCsvRaw, $strTemp);
				}
			}
			else {
				# A LINK が検出された場合（CSVは2データ扱い（LINK先とIMG SRCの二つ）
				if(length(GetAttribValue($strTemp, 'a', 'href'))>0){
					if($flag_indata == 1) {
						# CSV1行完成。ファイルに出力
						# テーブル1行に複数のデータがある場合にココで引っかかる
						# （空白時の前行からのコピーはこのモードでは行わない）
						if($#arrCsvRaw > 1){ sub_read_from_csv(\@arrCsvRaw); }
						@arrCsvRaw = ();
						$flag_indata = 0;
					}
					push(@arrCsvRaw, GetAttribValue($strTemp, 'a', 'href'));
					if(length(GetAttribValue($strTemp, 'img', 'src'))>0){ push(@arrCsvRaw, GetAttribValue($strTemp, 'img', 'src')); }
					else{ push(@arrCsvRaw, ""); }
					$flag_indata = 1;	# A LINKを検出したフラグ
				}
				# IMG が検出された場合
				elsif(length(GetAttribValue($strTemp, 'img', 'src'))>0){
					if($flag_indata == 1) {
						push(@arrCsvRaw, GetAttribValue($strTemp, 'img', 'src'));
					}
				}
				# それ以外のタグが検出された場合は、空白データ扱い
				else{
					if($flag_indata == 1) {
						push(@arrCsvRaw, "");
					}
				}
			}
		});

		# 桁データが空白の場合、前行から値をコピーする
		if($flag_copy_prev >= 1)
		{
			for(my $i=0; $i<=$#arrCsvRaw; $i++)
			{
				if($arrCsvRaw[$i] eq '' && defined($arrCsvPrevLine[$i]) && length($arrCsvPrevLine[$i])>1)
				{
					if($i==3){ $arrCsvRaw[$i] = $arrCsvPrevLine[$i]; }
					if($i==4 && $flag_copy_prev == 2){ $arrCsvRaw[$i] = $arrCsvPrevLine[$i]; }
				}
			}
			@arrCsvPrevLine = @arrCsvRaw;
		}

		# CSV1行完成。ファイルに出力
		if($#arrCsvRaw > 1){ sub_read_from_csv(\@arrCsvRaw); }
	});

	print("既存HTMLファイルから ".($#arrImageFiles+1)." 行のデータをインポートしました\n");
}

#
# 引数：$strHTML, $strTagName, $strElementName
# 引数の例： '<img src="x.jpg">', 'img', 'src'
# 戻り値：$str（値が見つからないときは長さゼロの文字列）
# 戻り値の例：'x.jpg'
sub GetAttribValue
{
	my $html = HTML::TagParser->new();

	$html->parse("<html><body>".$_[0]."</body></html>");
	my $elem = $html->getElementsByTagName($_[1]);
	my $strValue = $elem->getAttribute($_[2]) if ref $elem;

	return($strValue) if defined $strValue;
	return("");
}


# CSVファイルからデータを読み込んで、配列に格納する
# 
# CSVデータの形式："a href","img src", "unix date", "comment1", "comment2"
#
# csv2html-thumb.pl の関数を流用
sub sub_read_from_csv {
	my $ref_arrFields = shift;	# 引数：CSVデータ配列のリファレンス

	if($#$ref_arrFields < 1){ return; }		# 要素数2以下のときはスキップ
	my @arrTemp = ($$ref_arrFields[0],		# [0]:画像ファイル名（dir + basename)
			dirname($$ref_arrFields[0]),	# [1]:画像ファイルのdir
			basename($$ref_arrFields[0]),	# [2]:画像ファイルのbasename
			$$ref_arrFields[1],		# [3]:サムネイルファイル名 (dir + basename)
			defined($$ref_arrFields[2]) ? $$ref_arrFields[2] : 0,	# [4]:unix時間
			defined($$ref_arrFields[3]) ? $$ref_arrFields[3] : '',	# [5]:comment1
			defined($$ref_arrFields[4]) ? $$ref_arrFields[4] : '',	# [6]:comment2
			''		# [7]: Exif回転情報
			);
	$arrTemp[5] =~ s/<br>/<br \/>/g;		# <br>→<br />
	$arrTemp[6] =~ s/<br>/<br \/>/g;		# <br>→<br />

	push(@arrImageFiles, \@arrTemp);

	return;
}


# 引数で与えられたファイルの、arrImageFiles内のインデックスを返す
# 戻り値：0 - $#arrImageFiles の場合一致した。-1の場合一致するものが無い
sub sub_check_match_file {
	my $str = shift;	# 引数：ファイルパス
	my $i;
	for($i=0; $i<=$#arrImageFiles; $i++){
		if($str eq $arrImageFiles[$i][1].'/'.$arrImageFiles[$i][2]){ return($i); }
	}
	return(-1);		# 一致せず
}

# 任意の文字コードの文字列を、UTF-8フラグ付きのUTF-8に変換する
sub sub_conv_to_flagged_utf8{
	my $str = shift;
	my $enc_force = undef;
	if(@_ >= 1){ $enc_force = shift; }		# デコーダの強制指定
	
	# デコーダが強制的に指定された場合
	if(defined($enc_force)){
		if(ref($enc_force)){
			$str = $enc_force->decode($str);
			return($str);
		}
		elsif($enc_force ne '')
		{
			$str = Encode::decode($enc_force, $str);
		}
	}

	my $enc = Encode::Guess->guess($str);	# 文字列のエンコードの判定

	unless(ref($enc)){
		# エンコード形式が2個以上帰ってきた場合 （shiftjis or utf8）
		my @arr_encodes = split(/ /, $enc);
		if(grep(/^$flag_charcode/, @arr_encodes) >= 1){
			# $flag_charcode と同じエンコードが検出されたら、それを優先する
			$str = Encode::decode($flag_charcode, $str);
		}
		elsif(lc($arr_encodes[0]) eq 'shiftjis' || lc($arr_encodes[0]) eq 'euc-jp' || 
			lc($arr_encodes[0]) eq 'utf8' || lc($arr_encodes[0]) eq 'us-ascii'){
			# 最初の候補でデコードする
			$str = Encode::decode($arr_encodes[0], $str);
		}
	}
	else{
		# UTF-8でUTF-8フラグが立っている時以外は、変換を行う
		unless(ref($enc) eq 'Encode::utf8' && utf8::is_utf8($str) == 1){
			$str = $enc->decode($str);
		}
	}

	return($str);
}


# 任意の文字コードの文字列を、UTF-8フラグ無しのUTF-8に変換する
sub sub_conv_to_unflagged_utf8{
	my $str = shift;

	# いったん、フラグ付きのUTF-8に変換
	$str = sub_conv_to_flagged_utf8($str);

	return(Encode::encode('utf8', $str));
}


# UTF8から現在のOSの文字コードに変換する
sub sub_conv_to_local_charset{
	my $str = shift;

	# UTF8から、指定された（OSの）文字コードに変換する
	$str = Encode::encode($flag_charcode, $str);
	
	return($str);
}


# 引数で与えられたファイルの エンコードオブジェクト Encode::encode を返す
sub sub_get_encode_of_file{
	my $fname = shift;		# 解析するファイル名

	$fname = sub_conv_to_local_charset($fname);

	# ファイルを一気に読み込む
	open(FH, "<$fname");
	my @arr = <FH>;
	close(FH);

	my $str = join('', @arr);		# 配列を結合して、一つの文字列に
	my $enc = Encode::Guess->guess($str);	# 文字列のエンコードの判定

	# エンコード形式の表示（デバッグ用）
#	if(ref($enc) eq 'Encode::utf8'){ print("detect : utf8\n"); }
#	elsif(ref($enc) eq 'Encode::Unicode'){
#		print("detect : ".$$enc{'Name'}."\n");
#	}
#	elsif(ref($enc) eq 'Encode::XS'){
#		print("detect : ".$enc->mime_name()."\n");
#	}
#	elsif(ref($enc) eq 'Encode::JP::JIS7'){
#		print("detect : ".$$enc{'Name'}."\n");
#	}
#	else{
#		# 二つ以上のエンコードが推定される場合は、$encに文字列が返る
#		print("unknown (".$enc.")\n");
#	}

	unless(ref($enc)){
		# エンコード形式が2個以上帰ってきた場合 （例：shiftjis or utf8）
		$enc = '';
	}

	return($enc);
}

