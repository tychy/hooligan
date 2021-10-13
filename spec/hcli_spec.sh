Describe 'bin/hcli'
  Describe '-h option'
    It 'outputs help text'
      When run ./bin/hcli -h
      The status should eq 1
      The output should include "ヘルプだよ"
    End

    It 'works in --help'
      When run ./bin/hcli -h
      The status should eq 1
      The output should include "ヘルプだよ"
    End
  End

  Describe 'clear command'
    add_assembly_file() { touch a.s; }
    BeforeEach 'add_assembly_file'
    
    It 'deletes all .s files'
      When run ./bin/hcli clean && [ ! -e "./*.s" ]
      The status should eq 0
      The output should eq "不要な一時ファイルを消去します...done"
    End

    It 'works in short hand'
      When run ./bin/hcli cl && [ ! -e "./*.s" ]
      The status should eq 0
      The output should eq "不要な一時ファイルを消去します...done"
    End
  End

  Describe 'compile command'
    clean() { ./bin/hcli clean > /dev/null; }
    BeforeEach 'clean'

    It 'performs compilation'
      When run ./bin/hcli compile tests/spec/main.c
      The status should eq 0
      The path ./main.s should be exist
    End

    It 'works in short hand'
      When run ./bin/hcli compile tests/spec/main.c
      The status should eq 0
      The path ./main.s should be exist
    End
  End

  Describe 'src command'
    clean() { ./bin/hcli clean > /dev/null; }
    BeforeEach 'clean'

    It 'output the content of assembly file'
      When run ./bin/hcli src tests/spec/main.c
      The status should eq 0
      The output should include ".intel_syntax noprefix"
    End

    It 'works in short hand'
      When run ./bin/hcli s tests/spec/main.c
      The status should eq 0
      The output should include ".intel_syntax noprefix"
    End
  End

  Describe 'exec command'
    clean() { ./bin/hcli clean > /dev/null; }
    BeforeEach 'clean'

    It 'compiles and executes .c file'
      When run ./bin/hcli exec tests/spec/main.c
      The status should eq 0
      The output should include "結果: 0"
      The output should include "Hello, World"
    End

    It 'works in short hand'
      When run ./bin/hcli exec tests/spec/main.c
      The status should eq 0
      The output should include "結果: 0"
      The output should include "Hello, World"
    End
  End

  Describe 'test command'
    It 'run tests successfully in 1st generation compiler'
      When run ./bin/hcli test
      The status should eq 0
      The output should include passed
    End

    It 'works in shorthand'
      When run ./bin/hcli t
      The status should eq 0
      The output should include passed
    End
  End
End
