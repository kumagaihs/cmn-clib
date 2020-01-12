CommonLibraryのビルド方法や使い方、その他メモなど

◆プロジェクト構成
	・LibTest		# 生成したスタティックリンクライブラリをテストするためのプロジェクト。

◆プロジェクトのビルド方法
	プロジェクトのトップディレクトリに移動し、makeを実行。

◆ドキュメントの生成方法
	プロジェクトのトップディレクトリに移動し、doxygenを実行。
	設定ファイルの指定は不要。
	（デフォルトで「Doxyfile」が読み込まれる。）

	実行するとdocディレクトリが出来る。
	その中にあるdoc/html/index.htmlがドキュメントのトップページ。

◆コーディング上の注意点
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

