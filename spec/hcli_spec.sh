Describe 'bin/hcli'
  Describe '-h option'
    It 'outputs help text' priority:required
      When run ./bin/hcli -h
      The status should eq 1
      The output should include "ヘルプだよ"
      The error should be blank
    End

    It 'works in --help' priority:preferably
      When run ./bin/hcli -h
      The status should eq 1
      The output should include "ヘルプだよ"
      The error should be blank
    End
  End

  Describe 'clear command'
    add_assembly_file() { touch a.s; }
    BeforeEach 'add_assembly_file'
    
    It 'deletes all .s files' priority:required
      When run ./bin/hcli clean && [ ! -e "./*.s" ]
      The status should eq 0
      The output should eq "不要な一時ファイルを消去します...done"
      The error should be blank
    End

    It 'works in short hand' priority:preferably
      When run ./bin/hcli cl && [ ! -e "./*.s" ]
      The status should eq 0
      The output should eq "不要な一時ファイルを消去します...done"
      The error should be blank
    End
  End

  Describe 'compile command' 
    clean() { ./bin/hcli clean > /dev/null; }
    BeforeEach 'clean'

    It 'performs compilation' priority:required
      When run ./bin/hcli compile tests/spec/main.c
      The status should eq 0
      The path ./main.s should be exist
      The output should include "" # Warningを抑制するために標準出力をテストする必要がある
      The error should be blank
    End

    It 'works in short hand' priority:preferably
      When run ./bin/hcli compile tests/spec/main.c
      The status should eq 0
      The path ./main.s should be exist
      The output should include "" # Warningを抑制するために標準出力をテストする必要がある
      The error should be blank
    End
  End

  Describe 'src command'
    clean() { ./bin/hcli clean > /dev/null; }
    BeforeEach 'clean'

    It 'output the content of assembly file' priority:required
      When run ./bin/hcli src tests/spec/main.c
      The status should eq 0
      The output should include ".intel_syntax noprefix"
      The error should be blank
    End

    It 'works in short hand' priority:preferably
      When run ./bin/hcli s tests/spec/main.c
      The status should eq 0
      The output should include ".intel_syntax noprefix"
      The error should be blank
    End
  End

  Describe 'exec command'
    clean() { ./bin/hcli clean > /dev/null; }
    BeforeEach 'clean'

    It 'compiles and executes .c file' priority:required
      When run ./bin/hcli exec tests/spec/main.c
      The status should eq 0
      The output should include "結果: 0"
      The output should include "Hello, World"
      The error should be blank
    End

    It 'works in short hand' priority:preferably
      When run ./bin/hcli exec tests/spec/main.c
      The status should eq 0
      The output should include "結果: 0"
      The output should include "Hello, World"
      The error should be blank
    End
  End

  Describe 'debug command'
    clean() { ./bin/hcli clean > /dev/null; }
    BeforeEach 'clean'
    debug_and_run() { echo "run\n quit" | ./bin/hcli debug tests/spec/main.c; }

    It 'run gdb and normally exit' priority:required
      When call debug_and_run
      The status should eq 0
      The output should include gdb
      The error should be blank
    End
  End

  Describe 'test command'
    It 'run tests successfully in 1st generation compiler' priority:required
      When run ./bin/hcli test
      The status should eq 0
      The output should include passed
      The error should be blank
    End

    It 'works in shorthand' priority:preferably
      When run ./bin/hcli t
      The status should eq 0
      The output should include passed
      The error should be blank
    End

    It 'run tests successfully using gcc' priority:preferably
      When run ./bin/hcli test -g 0 2>/dev/null
      The status should eq 0
      The output should include passed
      The error should include "" # Warningを抑制するために標準エラーをテストする必要がある
    End

    It 'run tests successfully in 100th generation compiler' priority:preferably
      When run ./bin/hcli test -g 100
      The status should eq 0
      The output should include passed
      The error should be blank
    End
  End
End
