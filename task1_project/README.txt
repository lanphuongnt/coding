1. Add include/cryptopp and lib (compile g++) into folder .
2. Change tasks.json follow:
WINDOWS
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "shell",
			"label": "C/C++: g++.exe build active file",
			"command": "C:\\msys64\\mingw64\\bin\\g++.exe",
			"args": [
				"-g2",
				"-O3",
				"-DNDEBUG",
				"${workspaceFolder}\\mypackages\\*.cpp",
				"${file}",
				"-o",
				"${fileDirname}\\${fileBasenameNoExtension}.exe",
				"-D_WIN32_WINNT=0x0501",
				"-pthread",
				"-L${workspaceFolder}\\lib",
				"-l:libcryptopp.a",
				"-I${workspaceFolder}\\include",
				"-Wall",
			],
			"options": {
				"cwd": "C:\\msys64\\mingw64\\bin"
			},
			"problemMatcher": [
				"$gcc"
			],
			"group": {
				"kind": "build",
				"isDefault": true
			},
			"detail": "compiler: C:\\msys64\\mingw64\\bin\\g++.exe"
		}
	]
}

UBUNTU:
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "shell",
			"label": "C/C++: g++ build active file",
			"command": "/usr/bin/g++",
			"args": [
				"-g2",
				"-O3",
				"-DNDEBUG",
				"${workspaceFolder}/*.cpp",
				"-o",
				"${fileDirname}/${fileBasenameNoExtension}",
				"-pthread",
				"-I${workspaceFolder}/include",
				"-L${workspaceFolder}/lib",
				"-l:libcryptopp.a",
				"-Wall",
			],
			"options": {
				"cwd": "${workspaceFolder}"
			},
			"problemMatcher": [
				"$gcc"
			],
			"group": "build",
			"detail": "compiler: /usr/bin/g++"
		}
	]
}
