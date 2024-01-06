#!/usr/bin/perl
 
# GNU GPL Free Software. (C) 2010 INOUE Hirokazu
# このファイルは UTF-8 で保存すること. save this file in UTF-8
 
use strict;
use warnings;
 
use DBI;
use Text::CSV_XS;
 
my $flagDebug = 1;    # 詳細表示する場合は 1 を代入する
 
my $database = './digikam4.db';        # digiKam のデータベースファイル
my $data_source = "dbi:SQLite:dbname=$database";
my $dbh = undef;
my $sth = undef;
my $strQuery = undef;
my $nSelectAlbum = 0;
my @arrPaths = undef;
my @arrFileAndComment = undef;        # CSVから読み込んだファイル名とコメントの配列
my $arrRefQueryResult = undef;        # SQLクエリ結果を一時的に保存
my $nTargetFiles = 0;        # CSV内のデータ行数
 
my $strTemp = undef;
my $i = 0;
 
print("digiKamの特定フォルダの画像に、CSVよりコメントをインポートします\n\n");
 
eval{
 
    ### CSVファイルを読み込む
    print("インポートするデータが入ったCSVファイル名を入力 : ");
    $strTemp = <STDIN>;
    chomp($strTemp);
    if($strTemp eq '')
    {
        print("ファイル名が入力されなかったため、終了しました\n");
        exit();
    }
 
    my $csv = Text::CSV_XS->new({binary=>1});
 
    open(FH_IN, "<$strTemp") or die("ファイル $strTemp を読み込めません");
    $nTargetFiles = 0;
    while(<FH_IN>)
    {
        # CSV各行をパースして、ファイル名とコメントを配列$arrFileAndCommentに格納
        my $strLine = $_;
        if($strLine eq ''){ next; }
        $csv->parse($strLine) or next;
        my @arrFields = $csv->fields();
        $arrFileAndComment[$nTargetFiles][0] = $arrFields[0];
        $arrFields[1] =~ s/<br \/>/\x0A/g;    # 改行のエスケープ"<br />"を0x0aに戻す
        $arrFileAndComment[$nTargetFiles][1] = $arrFields[1];
        $nTargetFiles++;
    }
    close(FH_IN) or die("ファイル $strTemp を close 出来ませんでした");
 
    ### CSVから読み込んだデータの画面表示
    printf("CSVから読み込み完了。データ行数 = %d\n", $nTargetFiles);
    if($flagDebug != 0)
    {
        print("CSV内のデータ一覧\nid   ファイル名           コメント\n----+--------------------+--------------------\n");
        for($i=0; $i<$nTargetFiles; $i++)
        {
            $strTemp = $arrFileAndComment[$i][1];
            $strTemp =~ s/\x0D\x0A|\x0D|\x0A/<br \/>/g;    # 画面表示での改行を抑止
            printf("%03d  %-20s %s\n", $i, $arrFileAndComment[$i][0], substr($strTemp,0,16));
        }
    }
 
    ### 書き込み対象とするdigiKamディレクトリの選択
    print("データを書き込むdigiKamディレクトリを選択します\n");
    # 自動コミットOFFでデータベースを開く
    $dbh = DBI->connect($data_source, "", "", {PrintError => 1, AutoCommit => 0}) or die("DBI error : fail to open SQLite db (db file = $database)\n");
    $strQuery = "select id,relativePath from Albums;";
    $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
    $sth->execute() or die("$sth->errstr \n");
 
    print("ディレクトリ一覧\nno.  ディレクトリ名\n----+-----------------\n");
    while(my @arrPaths = $sth->fetchrow_array())
    {
        printf("%03d  %s\n", $arrPaths[0], $arrPaths[1]);
    }
 
    print("コメントを書き込むディレクトリの no. を指定してください : ");
 
    $strTemp = <STDIN>;
    chomp($strTemp);
    $nSelectAlbum = $strTemp;
    printf("select : %03d\n", $nSelectAlbum);
 
    ### 選択されたdigiKamのディレクトリが正しいかチェックする
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
        printf("対象ディレクトリ : %s（id = %d）\n", $arrPaths[1], $nSelectAlbum);
    }
    else
    {
        $dbh->disconnect();
        die("ディレクトリの選択が間違っています\n");
    }
 
    ### digiKamディレクトリ内のファイル数を画面表示する
    $strQuery = "select count(*) from Albums,Images on Albums.id=Images.album where Albums.id=?;";
    $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
    $sth->execute($nSelectAlbum) or die("$sth->errstr \n");
    $arrRefQueryResult = $sth->fetchall_arrayref();
 
    printf("CSVデータ行数 = %d\n", $nTargetFiles);
    printf("ディレクトリ内ファイル数=%d\n", $arrRefQueryResult->[0]->[0]);
 
 
    if($flagDebug != 0){ print("書き換え結果表示\n  id   ファイル名            旧コメント        新コメント\n-+----+---------------------+-----------------+----------------\n"); }
    else{ print("N:New, U:Update\n");}
    my $nWriitenLines = 0;
    for($i=0; $i<$nTargetFiles; $i++)
    {
        $strQuery = "select Images.id from Albums,Images on Albums.id=Images.album where Albums.id=? and Images.name=?;";
        $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
        $sth->execute($nSelectAlbum,$arrFileAndComment[$i][0]) or die("$sth->errstr \n");
        $arrRefQueryResult = $sth->fetchall_arrayref();
        if($sth->rows() != 1){ next; }
 
        my $nId = $arrRefQueryResult->[0]->[0];    # DB内のImages.id
 
        $strQuery = "select Images.id, ImageComments.comment from Albums,Images,ImageComments on Albums.id=Images.album and ImageComments.Imageid=Images.id where Albums.id=? and Images.name=?;";
        $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
        $sth->execute($nSelectAlbum,$arrFileAndComment[$i][0]) or die("$sth->errstr \n");
        my $arrRefComment = $sth->fetchall_arrayref();
        if($sth->rows() >= 1){
            # DB内にコメントが存在する
            if($arrRefComment->[0]->[1] ne $arrFileAndComment[$i][1])
            {
                $strQuery = "update ImageComments set comment=? where imageid=? and type=1;";
                $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
                $sth->execute($arrFileAndComment[$i][1],$nId) or die("$sth->errstr \n");
                $sth->finish() or die("データベース 書き込み時 finish 失敗");
                if($flagDebug != 0)
                {
                    $arrFileAndComment[$i][1] =~ s/\x0D\x0A|\x0D|\x0A/<br \/>/g;    # 画面表示での改行を抑止
                    $arrRefComment->[0]->[1] =~  s/\x0D\x0A|\x0D|\x0A/<br \/>/g;
                    printf("U %d  %-20s  %-16s  %-16s\n", $nId, $arrFileAndComment[$i][0], substr($arrRefComment->[0]->[1],0,16), substr($arrFileAndComment[$i][1],0,16));
                }
                else{ print("U"); }
                $nWriitenLines++;
            }
            elsif($flagDebug == 0){ print("."); }
        }
        else{
            # DB内にコメントが存在しない
            if($arrFileAndComment[$i][1] ne '')
            {
                $strQuery = "insert into ImageComments (imageid, type, language,comment) values (?,1,'x-default',?);";
                $sth = $dbh->prepare($strQuery) or die("$dbh->errstr \n");
                $sth->execute($nId,$arrFileAndComment[$i][1]) or die("$sth->errstr \n");
                $sth->finish() or die("データベース 書き込み時 finish 失敗");
                if($flagDebug != 0)
                {
                    $arrFileAndComment[$i][1] =~ s/\x0D\x0A|\x0D|\x0A/<br \/>/g;    # 画面表示での改行を抑止
                    printf("N %d  %-20s  %-16s  %-16s\n", $nId, $arrFileAndComment[$i][0],'', substr($arrFileAndComment[$i][1],0,16));
                }
                else{ print("N"); }
                $nWriitenLines++;
            }
            elsif($flagDebug == 0){ print("."); }
        }
 
    }
 
    if($flagDebug == 0){ print("\n"); }
 
    if($nWriitenLines <= 0)
    {
        print("コメントの書き換え対象がないため、終了しました\n");
        $dbh->rollback;
        $dbh->disconnect();
        exit();
    }
 
    ### エクスポートするかどうか最終確認
    printf("以上の %d 個のコメントをdigikamデータベースに書き込みますか？ (Y) : ", $nWriitenLines);
    $strTemp = <STDIN>;
    chomp($strTemp);
    if(uc($strTemp) ne 'Y')
    {
        print("\ndigiKamデータベースを更新せず、終了しました\n");
        $dbh->rollback;
        $dbh->disconnect();
        exit();
    }
 
    $dbh->commit() or die("データベース commit 失敗");
    printf("\n書き換えファイル数=%d\n", $nWriitenLines);
 
    $dbh->disconnect();
 
};
if($@){
    # evalによるエラートラップ：エラー時の処理
    if(defined($dbh))
    {
        $dbh->rollback;
        $dbh->disconnect();
    }
 
    die("プログラム エラー : ".$@."¥n");
}
 
print("スクリプト終了\n");
 
### スクリプト末端 EOF
