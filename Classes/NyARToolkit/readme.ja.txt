ARToolkit C++ class library NyARToolkitCPP.
Copyright (C)2008 R.Iizuka

version 0.9.0

http://nyatla.jp/
airmail(at)ebony.plala.or.jp
--------------------------------------------------


・NyARToolkitCPP

NyARToolkitCPPは、NyARToolkit for Java 2.0系をベースにした、C++
によるNyARToolkit互換のクラスライブラリです。

処理性能は、オリジナルARToolKit比で、約30%～40%程度高速です。
NyARToolkitと同様に、検出数が多くなるほど高速になります。


****現在β版につき、いくつか機能不足な点があります。****

※現在は計算用のコアクラスのみの実装となっています。
　カメラ入力・3D出力部のコードはまだ実装されていません。

※マーカ検出クラスは、SingleMarkerDetectorのみ（単一マーカ検出クラス）
　が使用可能です。MarkerDetector（複数マーカ検出クラス）は未実装です。

※サンプルプログラムには、BGRA静止画からマーカ検出
　処理を行い、その処理時間を計測するRawTestのみがあります。




ARToolkitは加藤博一先生とHuman Interface Technology Labにより
開発されたAugmented Reality (AR) ライブラリです。
詳しくはこちらをご覧下さい。
http://www.hitl.washington.edu/artoolkit/

NyARToolkitはARToolkit 2.72.1をベースに開発された、ARToolKit互換の
演算結果を持つ、Javaクラスライブラリです。
詳しくはこちらをご覧下さい。
http://nyatla.jp/nyartoolkit/


・動作/開発環境

forWindows
 Visual Stadio 2008　Express Editionのコンパイル環境があります。

forLinux
 gccでコンパイルできるmakefileがあります。


 
・ディレクトリ構成

./data
  マーカーファイル/カメラ設定ファイルがあります。

./forLinux
　Linux/cygwin向けのmakefileがあります。

./forWindows
　Win32向けの、VisualStadio2008用solutionファイルがあります。

./inc
　NyARToolkitCPPのIncludeファイルがあります。

./src
　NyARToolkitCPPのソースファイルがあります。

./extlib
　外部ライブラリを置く予定地です。



・・モジュール構成

現在は、マーカー検出・座標変換を行う計算クラス群のみの実装です。
DirectX、OpenGLとの接続クラス、カメラ画像のキャプチャクラスは
別途実装する必要があります。

ARToolKitと置換するときには、ARToolKit変換行列→OpenGL行列
の変換関数を流用してください。



・NyARToolkitとの差分

クラス構造は、一部C++の記法に合わせて変更されていますが、基本的には
NyARToolkitと同一です。
演算結果には完全な互換性があります。


・ライセンス
extlib以下のものは、各モジュールのライセンスに従ってください。
NyARToolkitは、特にソースに表記の無い限り、GPLライセンスです。
詳しくはLICENCE.txtをみてください。

