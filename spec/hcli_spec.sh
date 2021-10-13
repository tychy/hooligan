Describe 'bin/hcli'
  Describe 'clear command'
    add_assembly_file() { touch a.s; }
    BeforeEach 'add_assembly_file'
    
    It 'deletes all .s files'
      When run ./bin/hcli clean && [ ! -e ./*.s ]
      The status should eq 0
      The output should eq "不要な一時ファイルを消去します...done"
    End

    It 'works in short hand'
      When run ./bin/hcli cl && [ ! -e ./*.s ]
      The status should eq 0
      The output should eq "不要な一時ファイルを消去します...done"
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
