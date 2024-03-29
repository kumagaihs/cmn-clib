# cmn-clib

C/C++用の関数ライブラリ＋ツールです。  
Windows/Linuxのマルチ環境で開発／利用が可能です。  

日曜プログラミング用に共通化した関数群をライブラリとしてまとめたものですが
まだ作りこみの最中なので既存関数のI/Fも変更する可能性は大いにあります。バグもあります。  
このライブラリはWindowsおよびLinuxで開発／利用が可能です。

## 開発／利用環境
- Windows  
  開発はVisualStudio(2019)もしくはEclipse(MinGw)。  
  DLLもしくはStatic-libとしてリンクして利用可能  
  ※DLLリンクは現在外しているためそのままではリンク不可。DLL用にビルド設定を変えてください。（そのうちやる予定）

- Linux  
  開発はgccおよびmakeです。  
  利用はarからのスタティックリンクです。  
  そのうちsoとしてダイナミックリンク可能にする予定（TODO）

- MacOS  
  自分がMac使ってないので対象外ですが、gcc+makeで標準C/C++ライブラリくらいしか使っていないのでMacでも開発／利用できると思う。  
  あ、一部ファイル操作かソケット通信でLinux用ライブラリ使ってしまっているかも？

## 各サブプロジェクトの概要
- cmn-clib  
  C言語用ライブラリ。extern "C"すればC++から利用することも可能です。（当たり前か）  
  VisualStudio用のプロジェクトファイルとEclipse（MinGw）向けのプロジェクトファイルが同居しているのでどちらでも開発可能です。  
  ただし私は普段VisualStudioかLinuxでビルドしているのでMinGwでは警告出る場合もあるかもしれません。  
  （たまにMinGwでビルドしてみて気が付いたら直していますが）  
  なのでVisualStudioもしくはLinuxでの利用がおすすめです。  
  ビルド方法・利用方法は`/cmn-clib/readme.txt`を参照してください。

- cmn-cpplib  
  C++用ライブラリ。ただまだ作りはじめでほとんど中身空っぽです。

- cmn-tools
  CUIベースのツール群を作成していく予定です。（まだあんまりない）  
  C++で開発しています。

## Documents
https://ityorozu.net/dev/cmn-clib/doc/html/index.html
