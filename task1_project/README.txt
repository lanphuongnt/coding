1. Add include/cryptopp and lib (compile g++) into folder .
2. Change tasks.json follow:

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