# hooligan

hooligan is yet another tiny pre-self-hosted C compiler.

### todo
- [ ] self-host


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

```

## 参考リンク
[低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook)

[アセンブリ命令](https://www.mztn.org/lxasm64/amd00.html)

[文字列操作](https://ja.wikibooks.org/wiki/C言語/標準ライブラリ/文字列操作)

[Linux x86アセンブラ](https://qiita.com/MoriokaReimen?page=2)

[代表的な命令](https://wiki.onakasuita.org/pukiwiki/?よく使うASM命令ベスト100位に説明つけてみた)

[x86_64 プログラミング入門](https://tanakamura.github.io/pllp/docs/asm_language.html)

[Intel Instruction Set pages](https://web.itu.edu.tr/kesgin/mul06/intel/index.html)

[さくっとアセンブリ入門 hello,world編](https://rabbitfoot141.hatenablog.com/entry/2016/05/01/124410)

[System V ABI](https://software.intel.com/content/dam/develop/external/us/en/documents/mpx-linux64-abi.pdf)
