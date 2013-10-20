# TimestampFreezer

## 始めに

このアプリケーションはファイルのタイムスタンプを保存し変更されても元に戻すことができます。
[TimerKeeper](http://www.sharkpp.net/soft/tool/timekeep.html)の後継となるアプリケーションです。

## インストール

### Windows

インストーラはありません。
単体で実行できるので好きなフォルダに移動しショートカットを作成してください。

### Mac

T.B.D.

## アンインストール

### Windows

アンインストーラはありません。
インストール時に作成したファイル(フォルダやショートカット)を削除してください。

### Mac

T.B.D.

## 使い方

1. ファイルをドラッグ＆ドロップや、メニューなどから追加してください。
   (追加した時点の作成日時、更新日時、アクセス日時、が保存されます。)
2. 追加したファイルを編集してください。
   この時、移動や削除を行うとファイルの管理ができなくなります。
3. ファイルがリストから除外された時や、このソフトが終了した時に変更されたファイルのタイムスタンプが元に戻ります。

## 制限事項

* Mac版は更新日時のみ保護することができます。
* Windows版は260文字(MAX_PATH)を超えるパスを扱うことができません。

## 動作環境

* Windows XP 64bit
* Mac OSX 10.8.5

以上の環境で動作確認をしました。

## ライセンス

このアプリケーションは、The MIT License の元で公開されています。

      The MIT License (MIT)
      
      Copyright (c) 2003, 2013 sharkpp All rights reserved.
      
      Permission is hereby granted, free of charge, to any person obtaining a copy
      of this software and associated documentation files (the "Software"), to deal
      in the Software without restriction, including without limitation the rights
      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
      copies of the Software, and to permit persons to whom the Software is
      furnished to do so, subject to the following conditions:
      
      The above copyright notice and this permission notice shall be included in
      all copies or substantial portions of the Software.
      
      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
      THE SOFTWARE.

