#!/usr/bin/perl
 
# GNU GPL Free Software. (C) 2010 INOUE Hirokazu
# このファイルは UTF-8 で保存すること. save this file in UTF-8
 
use strict;
use warnings;
 
use DBI;
use Text::CSV_XS;
 
my $flagDebug = 0;    # 詳細表示する場合は 1 を代入する
 
my $database = './digikam4.db';        # digiKam のデータベースファイル
my $data_source = "dbi:SQLite:dbname=$database";
my $dbh = undef;
my $sth = undef;
my $strQuery = undef;    # DBIのSQL prepare用
my $nSelectAlbum = 0;    # 選択されたAlbumsテーブルのid
my @arrPaths = undef;    # AlbumsテーブルのクエリでrelativePathの結果配列を格納する
my $arrRefPathsWoComment = undef;    # export対象でコメント無しの(id,ファイル名,コメント）配列
my $arrRefPathsWComment = undef;    # export対象でコメント有りの(id,ファイル名,コメント）配列
my $nTargetFilesWoComment = undef;    # export対象でコメント無しの件数（ファイル数）
my $nTargetFilesWComment = undef;    # export対象でコメント有りの件数（ファイル数）
my $csv = undef;    # CSVファイル作成用
 
my $i = 0;
my $strTemp = undef;
 
print("digiKamの特定ディレクトリの画像とそのコメント一覧をCSVにエクスポートします\n\n");
 
eval{
 
    $dbh = DBI->connect($data_source) or die("DBI error : fail to open SQLite db (db file = $database)\n");
 
 
    ### ディレクトリ一覧を表示して、対象フォルダをユーザに選択させる
    $strQuery = "select id,relativePath from Albums;";
    $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
    $sth->execute() or die("$sth->errstr \n");
 
    print("ディレクトリ一覧\nno.  ディレクトリ名\n----+-----------------\n");
    while(@arrPaths = $sth->fetchrow_array())
    {
        printf("%03d : %s\n", $arrPaths[0], $arrPaths[1]);
    }
 
    print("コメントを書き込むディレクトリの no. を指定してください : ");
 
    $strTemp = <STDIN>;
    chomp($strTemp);
    $nSelectAlbum = $strTemp;
    printf("select : %03d\n", $nSelectAlbum);
 
 
    ### ユーザが選択したディレクトリが存在するか確認し、ディレクトリ名を画面表示する
    $strQuery = "select id,relativePath from Albums where id=?;";
    $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
    $sth->execute($nSelectAlbum) or die("$sth->errstr \n");
 
    if(@arrPaths = $sth->fetchrow_array())
    {
        if($sth->rows() != 1)
        {
            $dbh->disconnect();
            die("ディレクトリの選択が間違っています\n");
        }
        printf("対象ディレクトリ : %s (Albums.id=%d)\n", $arrPaths[1], $nSelectAlbum);
    }
    else
    {
        $dbh->disconnect();
        die("ディレクトリの選択が間違っています\n");
    }
 
 
    ### コメントが無いファイル一覧を配列に格納する
    $strQuery = "select Images.id, Images.name from Albums,Images on Albums.id=Images.album where Albums.id=? and Images.id not in (select Images.id from Albums,Images,ImageComments on Albums.id=Images.album and ImageComments.Imageid=Images.id where Albums.id=? and ImageComments.type=1);";
    $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
    $sth->execute($nSelectAlbum,$nSelectAlbum) or die("$sth->errstr \n");
    $arrRefPathsWoComment = $sth->fetchall_arrayref();
    $nTargetFilesWoComment = $sth->rows();
 
 
    ### コメントが有るファイル一覧を配列に格納する
    $strQuery = "select Images.id, Images.name, ImageComments.comment from Albums,Images,ImageComments on Albums.id=Images.album and ImageComments.Imageid=Images.id where Albums.id=? and ImageComments.type=1;";
    $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
    $sth->execute($nSelectAlbum) or die("$sth->errstr \n");
    $arrRefPathsWComment = $sth->fetchall_arrayref();
    $nTargetFilesWComment = $sth->rows();
 
    ### 見つかったexport対象ファイルの数を画面表示
    printf("ファイル数 = %d (うちコメント有り %d 個, コメント無し %d 個）\n", $nTargetFilesWComment + $nTargetFilesWoComment, $nTargetFilesWComment, $nTargetFilesWoComment);
 
 
    ### 見つかったexport対象ファイルの一覧を画面表示
    if($flagDebug != 0)
    {
        print("対象ファイル一覧\nid   ファイル名            コメント\n----+---------------------+----\n");
        for($i=0; $i<$nTargetFilesWComment; $i++)
        {
            printf("%03d  %-20s  %s\n", $arrRefPathsWComment->[$i]->[0], $arrRefPathsWComment->[$i]->[1], substr($arrRefPathsWComment->[$i]->[2],0,32));
        }
        for($i=0; $i<$nTargetFilesWoComment; $i++)
        {
            printf("%03d  %-20s <コメント無し>\n", $arrRefPathsWoComment->[$i]->[0], $arrRefPathsWoComment->[$i]->[1]);
        }
    }
 
    $dbh->disconnect();
 
    ### エクスポートするかどうか確認し、ファイル名を入力させる
    print("以上の内容をエクスポートしますか？ (Y) : ");
    $strTemp = <STDIN>;
    chomp($strTemp);
    if(uc($strTemp) ne 'Y')
    {
        print("ファイルを出力せず、終了しました\n");
        exit();
    }
 
    print("出力するファイル名を入力 : ");
    $strTemp = <STDIN>;
    chomp($strTemp);
    if($strTemp eq '')
    {
        print("ファイル名が入力されなかったため、終了しました\n");
        exit();
    }
 
    ### 出力用ファイルを開く
    open(FH_OUT, ">$strTemp") or die("ファイル $strTemp に書き込めません");
 
    $csv = Text::CSV_XS->new({binary=>1});    # 日本語の場合は binary を ON にする
 
    ### コメント有りのCSVデータを作成し、出力ファイルに書き込む
    for($i=0; $i<$nTargetFilesWComment; $i++)
    {
        if(defined($arrRefPathsWComment->[$i]->[2])){ $arrRefPathsWComment->[$i]->[2] =~ s/\x0D\x0A|\x0D|\x0A/<br \/>/g; }    # 改行をエスケープ
        $csv->combine($arrRefPathsWComment->[$i]->[1], $arrRefPathsWComment->[$i]->[2]);
        print(FH_OUT $csv->string() . "\n") or die("ファイル $strTemp への書き込み異常");
    }
    ### コメント無しのCSVデータを作成し、出力ファイルに書き込む
    for($i=0; $i<$nTargetFilesWoComment; $i++)
    {
        $csv->combine($arrRefPathsWoComment->[$i]->[1], "");
        print(FH_OUT $csv->string() . "\n") or die("ファイル $strTemp への書き込み異常");
    }
 
    close(FH_OUT) or die("ファイル $strTemp を close 出来ませんでした");
    print("ファイル $strTemp への書き込み正常終了\n");
 
};
if($@){
    # evalによるエラートラップ：エラー時の処理
    die("プログラム エラー : ".$@."¥n");
    exit();
}
 
print("スクリプト終了\n");
 
### スクリプト末端 EOF
