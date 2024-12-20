This is just a simple repo I use to practice the basic skill of modern C++

Notice:
1. How to set launch.json configuration for vs code:
{
    "version": "0.2.0",
    "configurations": [
      {
        "name": "C++ Launch",
        "type": "cppdbg",
        "request": "launch",
        "program": "${workspaceFolder}/build/mordern_cpp/reactor/EchoServer",
        "stopAtEntry": false,
        "cwd": "${workspaceFolder}",
        //"customLaunchSetupCommands": [
        //  { "text": "target-run", "description": "run target", "ignoreFailures": false }
        //],
        "targetArchitecture": "x86_64",
        "launchCompleteCommand": "exec-run",
        "linux": {
          "MIMode": "gdb",
          "miDebuggerPath": "/usr/bin/gdb"
        },
        "osx": {
          "MIMode": "lldb"
        },
        "windows": {
          "MIMode": "gdb",
          "miDebuggerPath": "C:\\MinGw\\bin\\gdb.exe"
        }
      }
    ]
  }
  