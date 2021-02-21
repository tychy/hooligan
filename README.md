# hooligan

低レイヤを知りたい人のためのCコンパイラ作成入門

https://www.sigbus.info/compilerbook

## 前提

プロジェクトルートで以下のコマンドを走らせる

`export PATH=bin:$PATH`

もしくは.envrcに以下のコマンドを記述

`PATH_add bin`
## テストコマンド
`hcc test`

もしくは

`docker-compose run hooligan make test`

## 参考リンク
[アセンブリ命令](https://www.mztn.org/lxasm64/amd00.html)

[文字列操作](https://ja.wikibooks.org/wiki/C言語/標準ライブラリ/文字列操作)

[Linux x86アセンブラ](https://qiita.com/MoriokaReimen?page=2)

[代表的な命令](https://wiki.onakasuita.org/pukiwiki/?よく使うASM命令ベスト100位に説明つけてみた)

[x86_64 プログラミング入門](https://tanakamura.github.io/pllp/docs/asm_language.html)

[Intel Instruction Set pages](https://web.itu.edu.tr/kesgin/mul06/intel/index.html)

[さくっとアセンブリ入門 hello,world編](https://rabbitfoot141.hatenablog.com/entry/2016/05/01/124410)

[System V ABI](https://software.intel.com/sites/default/files/article/402129/mpx-linux64-abi.pdf)
