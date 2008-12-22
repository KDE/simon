======================================================================
                  Large Vocabulary Continuous Speech
                          Recognition Engine

                                Julius

                                                (Rev 4.1   2008/10/03)
                                                (Rev 4.0.2 2008/05/27)
                                                (Rev 4.0   2007/12/19)
                                                (Rev 3.5.3 2006/12/29)
                                                (Rev 3.4.2 2004/04/30)
                                                (Rev 2.0   1999/02/20)
                                                (Rev 1.0   1998/02/20)

 Copyright (c) 1991-2008 京都大学 河原研究室
 Copyright (c) 1997-2000 情報処理振興事業協会(IPA)
 Copyright (c) 2000-2005 奈良先端科学技術大学院大学 鹿野研究室
 Copyright (c) 2005-2008 名古屋工業大学 Julius開発チーム
 All rights reserved
======================================================================

Julius について
=================

Julius は，音声認識システムの開発・研究のためのオープンソースの高性能
な汎用大語彙連続音声認識エンジンです．数万語彙の連続音声認識を一般のPC
上でほぼ実時間で実行できます．また，高い汎用性を持ち，発音辞書や言語モ
デル・音響モデルなどの音声認識の各モジュールを組み替えることで，様々な
幅広い用途に応用できます． 動作プラットフォームは Linux, Windows，
その他 Unix 環境です．詳細・関連情報は以下の URL をご覧下さい．

    http://julius.sourceforge.jp/


Julius-4.1
===========

4.0 から 4.0.2 では，多くのバグが修正され，いくつかの細かい改善が
行われました．新オプションとして "-fallback1pass" と "-usepower" が
追加され，Linux ではオーディオ API のデフォルトが OSS から ALSA に
変更になりました．

4.0.2 から 4.1 では，マルチストリーム音響モデル，MSD-HMM音響モデル，
CVN および frequency warping for VTLN がサポートされました．また，
モジュールモードのクライアントの perl 版サンプル "jclient-perl" が
追加されました．

4.1 で加えられた最も大きな機能は，動的リンクを用いたプラグイン拡張のサ
ポートです．共有オブジェクトをプラグインとして実行時に読み込むことがで
き，エンジンのソースをいじらなくても簡単に機能拡張することができます．
ディレクトリ "plugin" 以下に，サンプルのソースコードが含まれており，テ
ストできるようになっています．コードには定義すべき関数の仕様が記述され
ていますので，参考にしてください．

また，Julius に関連するドキュメントが "The Juliusbook" として同時に公
開されるようになりました．ドキュメントのソースは Docbook XML で書かれてお
り，html 版と pdf 版が提供されています．またアーカイブ中のオンラインマ
ニュアルも同じソースから生成されています．内容は 4.1 に合わせて更新さ
れています．一部未完成の部分もありますが，今後もJulius本体のリリースに
合わせて加筆・更新されていきます．

Juliusbook については別途ダウンロードしてください．

その他，修正点の詳細については Release-ja.txt をご覧下さい．


ファイルの構成
===============

	00readme-ja.txt		最初に読む文書（このファイル）
	LICENSE.txt		ライセンス条項
	Release-ja.txt		リリースノート/変更履歴
	configure		configureスクリプト
	configure.in		
	Sample.jconf		jconf 設定ファイルサンプル
	julius/			Julius 本体ソース
	libsent/		Julius ライブラリソース
	adinrec/		録音ツール adinrec
	adintool/		音声録音/送受信ツール adintool
	generate-ngram/		N-gram文生成ツール
	gramtools/		文法作成ツール群
	jcontrol/		サンプルネットワーククライアント jcontrol
	mkbingram/		バイナリN-gram作成ツール mkbingram
	mkbinhmm/		バイナリHMM作成ツール mkbinhmm
	mkgshmm/		GMS用音響モデル変換ツール mkgshmm
	mkss/			ノイズ平均スペクトル算出ツール mkss
	jclient-perl/		A simple perl version of module mode client
	plugin/			プラグインソースコードのサンプルと仕様文書
	man/			マニュアル類
	support/		開発用スクリプト


使用方法・ドキュメント
=======================

本アーカイブに付属しているのはソースコード，バージョン遍歴，サンプルの
jconf 設定ファイルおよび各種オンラインマニュアル(.man)のみです．多くの
ドキュメントは Julius の Web ページにて最新版を見ることができます．す
べてのオプションの説明やコンパイル方法，チュートリアルから様々な使用方
法，各機能の紹介，制限事項等の資料がありますので，そちらを御覧下さい．

    ホームページ：http://julius.sourceforge.jp/

また，上記ホームページにおいて，Juliusを用いた研究やアプリケーション開
発に関する情報交換を行うための「開発者フォーラム」を設置しております．
最新の Julius の CVS 更新情報なども投稿されます．
どうぞアクセスください．

    Julius Forum: http://julius.sourceforge.jp/forum/


ライセンス
===========

Julius はオープンソースソフトウェアです．
学術用途・商用を含め，利用に関して特に制限はありません．
利用許諾については，同梱の文書 "LICENSE.txt" にありますのでお読み下さい．


連絡先
===========

Julius に関するご質問・お問い合わせは，上記Webページ上のフォーラム，
あるいは下記のメールアドレスまでお問い合わせ下さい
('at' を '@' に読み替えてください)

	julius-info at lists.sourceforge.jp

以上
