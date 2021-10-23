# hooligan

hooligan is yet another tiny self-hosted C compiler by @tychy and @yokonao.

## Get Started
Shell script for developers(hcli) and shared object file(libhooligan.so) are placed in `bin` directory.
You have to add `bin` directory to PATH and LD_LIBRARY_PATH.

We recommend you use [direnv](https://github.com/direnv/direnv).
You can use `.envrc.sample`.
Plese run `cp .envrc.sample .envrc`.


### install shellspec
It is require to install shellspec for testing
```
curl -fsSL https://git.io/shellspec | sh
```
Or, refer to [the link](https://github.com/shellspec/shellspec#installation)

Run simply `shellspec` command for testing
```
shellspec
```


## Command
```
// display help
hcli -h

// compile specified files and output assembly code
hcli src a.c

// compile and assembly specified files and execute
hcli exec a.c

// generate 2nd gen compiler
make selfhost

// run tests using 2nd gen compiler
hcli test -g 2

```

## References
[低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook)

[アセンブリ命令](https://www.mztn.org/lxasm64/amd00.html)

[文字列操作](https://ja.wikibooks.org/wiki/C言語/標準ライブラリ/文字列操作)

[Linux x86アセンブラ](https://qiita.com/MoriokaReimen?page=2)

[代表的な命令](https://wiki.onakasuita.org/pukiwiki/?よく使うASM命令ベスト100位に説明つけてみた)

[x86_64 プログラミング入門](https://tanakamura.github.io/pllp/docs/asm_language.html)

[Intel Instruction Set pages](https://web.itu.edu.tr/kesgin/mul06/intel/index.html)

[さくっとアセンブリ入門 hello,world編](https://rabbitfoot141.hatenablog.com/entry/2016/05/01/124410)

[C11の最終ドラフト](https://drive.google.com/drive/u/0/folders/1_JbdYg11sAQIpDWm1wYloAoMofjHx53y)

[x86-64のABI](https://drive.google.com/drive/u/0/folders/1_JbdYg11sAQIpDWm1wYloAoMofjHx53y)
