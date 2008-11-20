TTrack

About:   TTrack はアクティブなウィンドウを淡々と記録し続けるアプリケーションです。
Author:  mallowlabs (http://d.hatena.ne.jp/mallowlabs/)
Mail:    mallowlabs at gmail.com
IDE:     Visual C++ 2008 Express Edition
License: new BSD License
Update:  2008/11/22

Install:
  適当なディレクトリに展開してください。
  ttrack
    - ttrack.exe
    - ttrackdll.dll
    - readme.txt
    
  実行には sqlite3 の DLL が必要です．
  http://www.sqlite.org/ から
  sqlitedll-3_x_x.zip をダウンロードし，解凍した後
  C:\Windows\system32 以下にコピーしてください．

Usage:
  EXE ファイルを実行すると記録を開始します．
  EXE ファイルの横に TTrack.db というファイルを作成し，
  そこに情報を蓄積します．
  タスクトレイアイコンをダブルクリックすると終了します．
  スタートアップへの登録等は各自行ってください．
 
DB Scheme:
  外部アプリケーションから DB にアクセスする際に参考にしてください．
  CREATE TABLE APPLICATIONS (
      id INTEGER PRIMARY KEY,
      path TEXT NOT NULL,
      title TEXT NOT NULL
  );
  CREATE TABLE ACCESSES (
      time DATETIME NOT NULL,
      application_id INTEGER NOT NULL,
      FOREIGN KEY (application_id)
      REFERENCES APPLICATIONS(id)
  );

License:
  修正 BSD ライセンスに準じます．
   作者: mallowlabs(mallowlabs at gmail.com)
  ただし，アイコンは LGPL ライセンスです．
   作者: http://www.everaldo.com/

TODO:
  - アイドルを取得できるようにする
  - ウィンドウの切り替えの度に DB に Insert するのではなく
    ある程度データをためてから一気に Insert して負荷を減らす
  - タスクバーの再作成に対応 (RegisterWindowMessage("TaskbarCreated"))
  