#!/usr/bin/perl

use strict;
use warnings;
use utf8;

my $flag_os = 'linux';	# linux/windows
my $flag_charcode = 'utf8';		# utf8/shiftjis

use Encode::Guess qw/euc-jp shiftjis iso-2022-jp/;	# 必要ないエンコードは削除すること
use POSIX;		# mktime用
use File::Basename;
use Image::Magick;
use Image::ExifTool;
use Image::Size;
use Time::Local;
use File::Copy;
use Text::CSV_XS;


use Data::Dumper;

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


my $strDirFrom = '';		# 入力：元画像ディレクトリ
my $strCsvName = '';		# 入力：元画像一覧のcsvファイル名
my $strDirTo = '';			# 出力：リサイズ画像書き込みディレクトリ

my $nCsvSwColumn = 5;		# CSVでコピー指令を格納しているカラム（0:1カラム, 1:2カラム ..., -1:常時コピー）
my $nLongEdge = 640;		# 縮小時の長辺ピクセル
my $nSharpness = 0.5;		# シャープネスの度合い

my $flag_write_mode = 'copy_from';	
						# from_all : 元画像すべて変換（上書きあり）
						# to_all : リサイズ画像に存在すれば全て変換（上書きあり）
						# from_no_overwr : 元画像すべて変換（上書きなし）
my $flag_csv_read_mode = 0;	# csvによる対象ファイル指定を行う場合、1
my $flag_sync_delete = 0;	# 変換先ディレクトリで「変換リストに一致しない」ファイルを削除

my $flag_verbose = 1;		# デバッグ出力必要なとき 1

my @arrImageFiles = ();		# 画像ファイルを格納する配列


# ファイル検索のパターン
my @arrFileScanMask;
if($flag_os eq 'linux'){
	@arrFileScanMask = ('*.jpg', '*.jpeg', '*.JPG', '*.JPEG');}
if($flag_os eq 'windows'){
	# Windowsの場合は、ファイル名の大文字と小文字は同一に扱われているようだ
	@arrFileScanMask = ('*.jpg', '*.jpeg');
}


print("\n".basename($0)." - 画像リサイズ、アップデート\n\n");

sub_user_input_init();

# 対象画像一覧を @arrImageFiles に格納する
if($flag_csv_read_mode == 1) {
	sub_read_from_csv();
} else{
	sub_scan_imagefiles();
}

# 処理内容のプレビューと処理開始の確認
if($flag_verbose == 1){ sub_preview_files(); }

# 画像変換（リサイズ）
sub_convert_images();

# 変換先ディレクトリに存在する「一致しない」ファイルを全て削除
if($flag_sync_delete == 1){ sub_sync_dir_to(); }

exit();

# 初期データの入力
sub sub_user_input_init {

	# プログラムの第1引数は、読み込み対象ディレクトリ・ファイル、第二引数は出力ディレクトリとする
	if($#ARGV >= 1 && length($ARGV[0])>1 && length($ARGV[1])>1)
	{
		$strDirFrom = sub_conv_to_flagged_utf8($ARGV[0]);
		$strDirTo = sub_conv_to_flagged_utf8($ARGV[1]);
	}

	# 元画像ディレクトリの入力
	print("元画像のあるディレクトリまたはcsvファイルを、絶対または相対ディレクトリで入力\n（例：/home/user/, ./）");
	if(length($strDirFrom)>0){ print("[$strDirFrom] :"); }
	else{ print(":"); }
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0){
		if(length($strDirFrom)>0){ $_ = $strDirFrom; }	# スクリプトの引数のデフォルトを使う場合
		else{ die("終了（理由：入力ディレクトリ又はcsvファイルが入力されませんでした）\n"); }
	}
	unless(-e sub_conv_to_local_charset($_)){ die("終了（理由：ディレクトリまたはファイル ".$_." が存在しません）\n"); }
	if(-d sub_conv_to_local_charset($_)){
		# ディレクトリの場合
		$flag_csv_read_mode = 0;
		if(substr($_,-1) ne '/'){ $_ .= '/'; }	# ディレクトリは / で終わるように修正
		$strDirFrom = $_;
		print("元画像ディレクトリ (FROM) : " . $strDirFrom . "\n\n");
	}
	elsif(-f sub_conv_to_local_charset($_)){
		# ファイルの場合
		$flag_csv_read_mode = 1;
		$strCsvName = basename($strDirFrom);
		$strDirFrom = dirname($strDirFrom);
		if(substr($strDirFrom,-1) ne '/'){ $strDirFrom .= '/'; }	# ディレクトリは / で終わるように修正
		print("元画像 基準ディレクトリ (FROM DIR) : " . $strDirFrom . "\n");
		print("元画像 一覧ファイル (CSV File) : " . $strCsvName . "\n\n");
	}
	else{ die("終了（理由：ディレクトリまたはファイル ".$_." のタイプが不明）\n"); }


	# CSV内で、コピー指令を示すカラム指定
	if($flag_csv_read_mode == 1){
		print("CSVファイルでコピー制御が書きこまれているカラム（1カラム目を0と表現する）の指定\n (-1:全ファイル指定, 0:ファイル名自体, 1 ... n) [$nCsvSwColumn] : ");
		$_ = <STDIN>;
		chomp;
		if(length($_) <= 0){ $nCsvSwColumn = $nCsvSwColumn; }
		elsif($_ =~ m/\-*[0-9]+/i){ $nCsvSwColumn = int($_); }
		else{die("数値以外が指定されました"); }
		
		if($nCsvSwColumn < -1 || $nCsvSwColumn == 0){ die("-2以下、0は指定できません\n"); }
		if($nCsvSwColumn == -1){ print("CSVの0カラム目の全てのファイル（が存在すれば）、リサイズ対象とします\n\n"); }
		else{ print("コピー制御カラム : ".$nCsvSwColumn."\n\n"); }

	}


	# 作成した画像を格納するディレクトリの入力
	print("リサイズ画像を保存するディレクトリを、絶対または相対ディレクトリで入力\n（例：/home/user/, ./）");
	if(length($strDirTo)>0){ print("[$strDirTo] :"); }
	else{ print(":"); }
	$_ = <STDIN>;
	chomp();
	if(length($_)<=0){
		if(length($strDirTo)>0){ $_ = $strDirTo; }	# スクリプトの引数のデフォルトを使う場合
		else{ die("終了（理由：ディレクトリが入力されませんでした）\n"); }
	}
	if(substr($_,-1) ne '/'){ $_ .= '/'; }	# ディレクトリは / で終わるように修正
	unless(-d sub_conv_to_local_charset($_)){ die("終了（理由：ディレクトリ ".$_." が存在しません）\n"); }
	$strDirTo = $_;
	print("リサイズ画像保存ディレクトリ (TO) : " . $strDirTo . "\n\n");


	# 処理モード（上書きモード）の選択
	print("処理モードの選択\n 1:元画像ディレクトリの全ての画像を変換（上書きON）\n".
			" 2:変換先画像ディレクトリに存在するすべての画像を変換（上書きON）\n".
			" 3:元画像ディレクトリの全ての画像を変換（上書きOFF）\n".
			"モードの選択 (1-3) [3] : ");
	$_ = <STDIN>;
	chomp;
	if(length($_)<=0 || $_ eq '3'){ $flag_write_mode = 'from_no_overwr'; }
	elsif($_ eq '1'){ $flag_write_mode = 'from_all'; }
	elsif($_ eq '2'){ $flag_write_mode = 'to_all'; }
	else{die("1-3以外が入力されました"); }
	
	if($flag_write_mode eq 'from_all'){print("元画像ディレクトリの全ての画像を変換（上書きON）\n\n");}
	if($flag_write_mode eq 'to_all'){print("変換先画像ディレクトリに存在するすべての画像を変換（上書きON）\n\n");}
	if($flag_write_mode eq 'from_no_overwr'){print("元画像ディレクトリの全ての画像を変換（上書きOFF）\n\n");}

	# 変換先ディレクトリで「変換リストに一致しないファイル」を削除するか選択
	print("変換先ディレクトリで、今回変換対象候補外の画像ファイルは削除する (Y/N) [N] :");
	$_ = <STDIN>;
	chomp;
	if(length($_)<=0 || uc($_) ne 'Y'){ $flag_sync_delete = 0; }
	else{ $flag_sync_delete = 1; }
	print("変換先ディレクトリのSync処理 ： ".($flag_sync_delete==1 ? 'YES' : 'NO')."\n\n");

	# 長辺ピクセルの入力
	print("出力画像の長辺ピクセルを指定 (10-2000) [".$nLongEdge."] : ");
	$_ = <STDIN>;
	chomp;
	if(length($_)<=0){ $_ = $nLongEdge; }
	elsif(int($_)<10 || int($_)>2000){ die("10-2000 の範囲外が指定されました\n"); }
	$nLongEdge = int($_);
	print("長辺ピクセル : ".$nLongEdge."\n\n");

	# 長辺ピクセルの入力
	print("シャープネス処理の度合いを入力 (0はOFFを意味する, 0-10.0) [".$nSharpness."] : ");
	$_ = <STDIN>;
	chomp;
	if(length($_)<=0){ $_ = $nSharpness; }
	elsif($_<0.0 || $_>10.0){ die("0-10 の範囲外が指定されました\n"); }
	$nSharpness = $_*1.0;
	if($nSharpness != 0.0){ print("シャープネス : ".$nSharpness."\n\n"); }
	else{ print("シャープネス : OFF\n\n"); }


	# 確認
	print("=======================\n次の内容で実行します\n 元画像 : ".
		$strDirFrom."\n リサイズ画像保存 : ".$strDirTo."\nこの内容でよいですか (Y/N) [N] : ");

	$_ = <STDIN>;
	chomp;
	if(uc($_) ne 'Y'){
		die("\nユーザによりキャンセルされました。\n");
	}

	print("\n");

}

# 対象画像ファイルを配列に格納する
sub sub_scan_imagefiles {

	my @arrScan = undef;	# ファイル一覧を一時的に格納する配列
	my $nCountNewfile = 0;		# 新規追加されたファイル数を数える
	my $nCountUpdatefile = 0;		# 新規追加されたファイル数を数える
	my $exifTool = Image::ExifTool->new();
#	$exifTool->Options(DateFormat => "%s", StrictDate=> 1);		# Windows版ActivePerlでは%sはサポート外
	$exifTool->Options(DateFormat => "%Y,%m,%d,%H,%M,%S", StrictDate=> 1);

	# ファイル一覧を得る
	my $strScanPattern = '';
	foreach(@arrFileScanMask){
		if(length($strScanPattern)>1 && substr($strScanPattern,-1) ne ' '){$strScanPattern .= ' ';}
		$strScanPattern .= $strDirFrom.$_;
	}
	@arrScan = glob(sub_conv_to_local_charset($strScanPattern));

	foreach(@arrScan)
	{
		if(length($_) <= 0){ next; }
		$_ = sub_conv_to_flagged_utf8($_);
		my $strFileFrom = $_;
		my $strFileTo = $strDirTo . basename($strFileFrom);

		# 出力先が存在するか
		my $nMatch = 0;
		if(-e sub_conv_to_local_charset($strFileTo)){
			$nMatch = 1;
			$nCountUpdatefile++;
		}
		else{
			$nCountNewfile++;
		}

		# Exif日時を読み込む
		$exifTool->ImageInfo(sub_conv_to_local_charset($strFileFrom));
		my $tmpDate = $exifTool->GetValue('CreateDate');
		if(!defined($tmpDate)){ $tmpDate = (stat(sub_conv_to_local_charset($strFileFrom)))[9]; }	# Exifが無い場合は最終更新日
		else{
			my @arrTime_t = split(/,/,$tmpDate);
			$tmpDate = mktime($arrTime_t[5], $arrTime_t[4], $arrTime_t[3], $arrTime_t[2], $arrTime_t[1]-1, $arrTime_t[0]-1900);
		}

		# 回転情報を得る（1:0 deg, 3:180 deg, 6:90 deg(CW), 8:270 deg (CW))
		my $exifRotate = $exifTool->GetValue('Orientation', 'Raw');
		if(defined($exifRotate)){ $exifRotate = int($exifTool->GetValue('Orientation', 'Raw')); }
		if(!defined($exifRotate) || $exifRotate == 0){ $exifRotate = 1; }

		my @arrTemp = ($strFileFrom,		# [0]: 入力画像フルパス（dir + basename）
				$strFileTo,	# [1]:  出力画像フルパス（dir + basename）
				$tmpDate,	# [2]: unix秒
				$exifRotate,	# [3]: Exif回転情報
				$nMatch);	# [4]: 出力先にファイルが存在する場合 1
		push(@arrImageFiles, \@arrTemp);

	}
	
	print($nCountNewfile." 個のファイルを新規、".$nCountUpdatefile." 個のファイルは変換先に存在\n\n");

}


# 画像一覧を格納したCSVファイルを読み込んで、対象画像を確定する
sub sub_read_from_csv {

	my $csv = Text::CSV_XS->new({binary=>1});
	my $exifTool = Image::ExifTool->new();
#	$exifTool->Options(DateFormat => "%s", StrictDate=> 1);		# Windows版ActivePerlでは%sはサポート外
	$exifTool->Options(DateFormat => "%Y,%m,%d,%H,%M,%S", StrictDate=> 1);

	open(FH_IN, "<".$strDirFrom.$strCsvName) or die("ファイル ".$strDirFrom.$strCsvName." を読み込めません");
	my $nTargetFiles = 0;
	while(<FH_IN>)
	{
		# CSV各行をパースして、ファイル名とコメントを配列$arrFileAndCommentに格納
		my $strLine = sub_conv_to_flagged_utf8($_);
		if($strLine eq ''){ next; }
		$csv->parse($strLine) or next;
		my @arrFields = $csv->fields();
		if($#arrFields < 0){ next; }		# 要素数1以下のときはスキップ（最低、ファイル名は必要）

		if($nCsvSwColumn >= 0){
			unless(defined($arrFields[$nCsvSwColumn])){ next; }		# コピー制御カラム無しの場合、次へ
			if($arrFields[$nCsvSwColumn] eq ''){ next; }		# コピー制御カラム空白の場合、次へ
		}
		my $strFileFrom = $strDirFrom . $arrFields[0];
		my $strFileTo = $strDirTo . basename($arrFields[0]);
		unless(-e sub_conv_to_local_charset($strFileFrom) && -f sub_conv_to_local_charset($strFileFrom)){
			 next;		# ファイルが存在しない場合、次へ
		}
		my $nMatch = (-e sub_conv_to_local_charset($strFileTo) ? 1 : 0);		# 出力先ファイル存在確認

		# Exif日時を読み込む
		$exifTool->ImageInfo(sub_conv_to_local_charset($strFileFrom));
		my $tmpDate = $exifTool->GetValue('CreateDate');
		if(!defined($tmpDate)){ $tmpDate = (stat(sub_conv_to_local_charset($strFileFrom)))[9]; }	# Exifが無い場合は最終更新日
		else{
			my @arrTime_t = split(/,/,$tmpDate);
			$tmpDate = mktime($arrTime_t[5], $arrTime_t[4], $arrTime_t[3], $arrTime_t[2], $arrTime_t[1]-1, $arrTime_t[0]-1900);
		}

		# 回転情報を得る（1:0 deg, 3:180 deg, 6:90 deg(CW), 8:270 deg (CW))
		my $exifRotate = $exifTool->GetValue('Orientation', 'Raw');
		if(defined($exifRotate)){ $exifRotate = int($exifTool->GetValue('Orientation', 'Raw')); }
		if(!defined($exifRotate) || $exifRotate == 0){ $exifRotate = 1; }

		my @arrTemp = ($strFileFrom,		# [0]: 入力画像フルパス（dir + basename）
				$strFileTo,	# [1]:  出力画像フルパス（dir + basename）
				$tmpDate,	# [2]: unix秒
				$exifRotate,	# [3]: Exif回転情報
				$nMatch);	# [4]: 出力先にファイルが存在する場合 1
		push(@arrImageFiles, \@arrTemp);
		$nTargetFiles++;
	}
	close(FH_IN) or die("ファイル $strCsvName を close 出来ませんでした");

	### CSVから読み込んだデータの画面表示
	printf("CSVから読み込み完了。データ行数 = %d\n", $nTargetFiles);

}

sub sub_preview_files {
	print("=======================\n処理プレビュー\n");
	for(my $i=0; $i<=$#arrImageFiles; $i++){
		print $arrImageFiles[$i][0];
		if($arrImageFiles[$i][4] == 1 && $flag_write_mode eq 'from_no_overwr'){ print(" (Skip)\n"); }
		elsif($arrImageFiles[$i][4] != 1 && $flag_write_mode eq 'to_all'){ print(" (Skip)\n"); }
		elsif($arrImageFiles[$i][4] == 1){ print(" (Overwrite)\n"); }
		else{ print("\n"); }
	}
	print("\n画像変換を開始しますか ? (Y/N) [N] :");
	$_ = <STDIN>;
	chomp;
	if(uc($_) ne 'Y'){
		die("\nユーザによりキャンセルされました。\n");
	}
	
	print("\n");
}

# 画像の変換（リサイズ処理）
sub sub_convert_images{

	my $image = Image::Magick->new();

	foreach(@arrImageFiles)
	{
		my $strFileFrom = $_->[0];
		my $strFileTo = $_->[1];
		my $exifRotate = $_->[3];


		if($_->[4] != 1 && $flag_write_mode eq 'to_all'){
			# 保存先に存在しないファイルはスキップ
			print("info : dest ".basename($strFileFrom)." not exist, skip.\n");
			next;
		}
		if($_->[4] == 1 && $flag_write_mode eq 'from_no_overwr'){
			# 保存先に存在しないファイルはスキップ
			print("info : dest ".basename($strFileFrom)." exist, skip.\n");
			next;
		}
		
		print(basename($strFileFrom)."  converting ...  ");

		@$image = ();           # 読み込まれている画像をクリア

		# 画像ファイルの読み込み
		my $image_check = $image->Read($strFileFrom);
		if($image_check){ print("(ERROR : read image)\n"); next; }

		# 画像の縦横サイズの決定
		my ($nWidth, $nHeight) = $image->Get('width', 'height');
		if($nWidth <= 0 || $nHeight <= 0){ print("(ERROR : read image x,y)\n"); next; }
		my $nNewWidth = $nWidth > $nHeight ? $nLongEdge : int($nLongEdge*$nWidth/$nHeight);
		my $nNewHeight = $nHeight > $nWidth ? $nLongEdge : int($nLongEdge*$nHeight/$nWidth);
		print($nWidth."x".$nHeight."->".$nNewWidth."x".$nNewHeight." ");

		# リサイズ
		$image->AdaptiveResize(width=>$nNewWidth, height=>$nNewHeight);
#		$image->Thumbnail(width=>$nNewWidth, height=>$nNewHeight);		# exif消す場合

		# シャープネス
		if($nSharpness != 0.0){ $image->Sharpen(radius=>0.0, sigma=>$nSharpness); }

		# exif情報による回転
		if($exifRotate == 3){$image->Rotate(degrees=>180.0); print("rot180 "); }
		if($exifRotate == 6){$image->Rotate(degrees=>90.0); print("rot90 "); }
		if($exifRotate == 8){$image->Rotate(degrees=>270.0); print("rot270 "); }

		# 新規画像ファイルに保存
		$image->Set(quality=>90);
		$image_check = $image->Write(sub_conv_to_local_charset($strFileTo));
		if($image_check){ print("(ERROR : write image)\n"); next; }

		print("\n");	# 正常に変換が終了した
	}

}

# 変換一覧 @arrImageFiles に無いファイルで、変換先ディレクトリに存在する画像を削除する
sub sub_sync_dir_to {
	my @arrScan = ();	# ファイル一覧を一時的に格納する配列
	my @arrDeleteFiles = ();	# 削除対象ファイルを格納する配列
	my $nCountDeletefile = 0;		# 新規追加されたファイル数を数える

	# ファイル一覧を得る
	my $strScanPattern = '';
	foreach(@arrFileScanMask){
		if(length($strScanPattern)>1 && substr($strScanPattern,-1) ne ' '){$strScanPattern .= ' ';}
		$strScanPattern .= $strDirTo.$_;
	}
	@arrScan = glob(sub_conv_to_local_charset($strScanPattern));

	foreach(@arrScan)
	{
		if(length($_) <= 0){ next; }
		$_ = sub_conv_to_flagged_utf8($_);
		my $strFile = $_;

		# $arrImageFiles に同一名のファイルがあれば、次の候補へ
		my $nFound = 0;
		foreach(@arrImageFiles){
			if(basename($_->[1]) eq basename($strFile)){ $nFound = 1; }
		}
		if($nFound == 1){ next; }
		
		push(@arrDeleteFiles, $strFile);
	}

	if($#arrDeleteFiles < 0){ return; }

	# ユーザに確認を求める
	print("=======================\n変換先ディレクトリのこれらのファイルは削除対象です\n");
	foreach(@arrDeleteFiles){ print(" delete : ".$_."\n"); }
	print("これらのファイルを削除しますか (Y/N) [N] : ");
	$_ = <STDIN>;
	chomp;
	if(uc($_) ne 'Y'){
		die("\nユーザによりキャンセルされました。\n");
	}
	print("\n");

	# 削除処理
	foreach(@arrDeleteFiles){
		if(!unlink($_)){ print("delete error : ".$_."\n"); }
		else{ $nCountDeletefile++; }
	}
	print($nCountDeletefile." 個のファイルを削除しました\n");

}

# 任意の文字コードの文字列を、UTF-8フラグ付きのUTF-8に変換する
sub sub_conv_to_flagged_utf8{
	my $str = shift;
	my $enc_force = undef;
	if(@_ >= 1){ $enc_force = shift; }		# デコーダの強制指定
	
	# デコーダが強制的に指定された場合
	if(defined($enc_force) && $enc_force ne ''){
		$str = $enc_force->decode($str);
		return($str);
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

