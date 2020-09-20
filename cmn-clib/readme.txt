cmn-clibのビルド方法や使い方、その他メモなど

■プロジェクト構成
	・inc			# ヘッダーファイル格納場所。ヘッダーファイル名はソースファイルのディレクトリ名と同じ
	・src			# ソースファイル格納場所。まとまり単位にディレクトリで分ける。
	・test			# テストコード格納場所
	・doc			# ドキュメント格納場所
	・debug			# Windows用のビルドファイル格納場所
	・build			# Linux用のビルドファイル格納場所

	＜ドキュメント＞
		ドキュメントにはdoxygenを使う。ドキュメント生成手順は以下。
		実行するとdocディレクトリが出来る。
		その中にあるdoc/html/index.htmlがドキュメントのトップページ。

		・Windowsの場合
			プロジェクトフォルダ直下に移動してdoxygen.exeを実行する。
		・Linuxの場合
			$ cd プロジェクトフォルダ
			$ doxygen

	＜その他＞
		・ctags.exe, doxygen.exe		# Windows上でtags作成およびドキュメント作成を行うために用意したEXE
		・Doxyfile						# doxygen設定ファイル

■プロジェクトのビルド方法
	＜Windows＞
		Eclipse（C/C++2019-12）用に設定済み。
		gitチェックアウト後、Eclipseでプロジェクトのビルドを実行すればOK。
		Windowsではtest_mainをエントリーポイントとした実行ファイルが生成される。
		ライブラリを生成する場合はビルド設定を変更すること。

	＜Linux＞
		$ cd プロジェクトフォルダ
		$ make

	＜挙動を変えるマクロ＞
		Common.h：CMN_CLIB_HI_PERFORMANCE　　defineされているとトレースログの省略等を行ったハイパフォーマンスモードでビルドする。

■テスト実施方法
	＜Windows＞
		test_mainを実行
	＜Linux＞
		・テストビルド
			$ cd プロジェクトフォルダ
			$ make
			$ build/test_main

■cmn-clibの利用方法（リンク方法）
	//TODO

	//TODO：利用側からするとヘッダファイルは"Common.h"よりも"cmnclib/Common.h"の方が良さそう。（アプリのヘッダと混じらないように）
	上記リファクタリングすること。


■命名規則
	＜ヘッダファイル名＞
		[ライブラリ名].h
		例：CmnData.h

	＜ソースファイル名＞
		[ライブラリ名][サブモジュール名].c
		例：ライブラリ名：CmnData
		　　サブモジュール名：List
		　　＝CmnDataList.c

	＜構造体＞
		構造体名
			[ライブラリ名][固有名]
			例：CmnDataList, CmnDataListItem
		メンバ名
			変数と同じ。
			非公開メンバはアンダーバーで始まる。

	＜関数名＞
		[ライブラリ名][固有名]_[動詞][名詞]
		例：CmnDataList_Create
		　　CmnDataList_GetFirst
		　　
		※非公開関数は先頭小文字で始まる。
		　cmnLogEx_PutLog

	＜定数マクロ名＞
		全て大文字のアンダーバー区切り
		[ライブラリ名]_[固有名]_[名詞]
		例：CMN_TIME_FORMAT_ALL

	＜関数マクロ名＞
		検討中


■コーディング上の注意点
○ LIBとDLLを両方作る共通関数ヘッダファイルのお約束
	ソースは普通に作ってよい。
	ヘッダに書くプロトタイプは、以下のように書くこと。
	（_USRDLLは、DLLを作成するプロジェクトで定義されるマクロで、COMMON_DLL_IMPORTはCommonDllImport.hで定義されている）

	/* DLL使用かLIB使用かによるプロトタイプ切り替え */
	#ifdef _USRDLL
	  /* DLL作成 */
	  #define D_EXTERN extern __declspec(dllexport)
	#else
	  #ifdef COMMON_DLL_IMPORT
		/* DLL使用 */
		#define D_EXTERN extern __declspec(dllimport)
	  #else
		/* LIB使用 */
		#define D_EXTERN extern
	  #endif
	#endif

	/* プロトタイプ */
	D_EXTERN char *CmnTime_GetFormatTime(int type, char *buf);

	※上記のD_EXTERNの定義は、そのうちCommon.hにまとめる。

