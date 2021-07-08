# hooligan

hooligan is yet another tiny self-hosted C compiler.

### todo
- [x] self-host
- [ ] run test using 2nd gen compiler

## How to use
### 前提

プロジェクトルートで以下のコマンドを走らせる

`export PATH=bin:$PATH`

もしくは.envrcに以下のコマンドを記述

`PATH_add bin`
### Command
```
// テストをすべて走らせる
hdb test

// selfhostを走らせる
hdb selfhost

// a.cに対してアセンブリの出力
hdb src a.c

// a.cに対してアセンブリを出力し実行する
hdb exec a.c

// セルフホストする
hdb selfhost
// + 第二世代コンパイラでテストを走らせる
hdb runtest2

```

### コンパイラの世代
```
gcc => 第0世代コンパイラ
$PROJECT_ROOT/bin/hcc1 => 第1世代コンパイラ
$PROJECT_ROOT/bin/hcc2 => 第2世代コンパイラ
```
## 参考図書(refディレクトリ)
[C11の最終ドラフト](ref/N1570.pdf)

[x86-64のABI](ref/x86-64-psABI-1.0.pdf)

## 参考リンク
[低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook)

[アセンブリ命令](https://www.mztn.org/lxasm64/amd00.html)

[文字列操作](https://ja.wikibooks.org/wiki/C言語/標準ライブラリ/文字列操作)

[Linux x86アセンブラ](https://qiita.com/MoriokaReimen?page=2)

[代表的な命令](https://wiki.onakasuita.org/pukiwiki/?よく使うASM命令ベスト100位に説明つけてみた)

[x86_64 プログラミング入門](https://tanakamura.github.io/pllp/docs/asm_language.html)

[Intel Instruction Set pages](https://web.itu.edu.tr/kesgin/mul06/intel/index.html)

[さくっとアセンブリ入門 hello,world編](https://rabbitfoot141.hatenablog.com/entry/2016/05/01/124410)
