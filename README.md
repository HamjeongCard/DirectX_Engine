# DirectX 11 3D Box Game Framework

DirectX 11 기반의 3D 박스(큐브) 렌더링 게임 프레임워크 예제 프로젝트입니다.

## 기능 (Features)
- **Win32 Window Management**: `Window` 클래스를 통한 윈도우 생성 및 메세지 루프 관리
- **DirectX 11 Core Engine**: Device, Device Context, SwapChain, RenderTargetView, DepthStencilView, RasterizerState 설정
- **3D Cube Geometry**: 버텍스 버퍼(Vertex Buffer) 및 인덱스 버퍼(Index Buffer) 기반 3D 박스
- **HLSL Shaders**: 런타임 D3DCompile을 이용한 정점(Vertex) 및 픽셀(Pixel) 셰이더 컴파일
- **3D Transformations**: `DirectXMath`를 이용한 월드(World), 뷰(View), 프로젝션(Projection) 행렬 연산 및 회전 애니메이션

## 디렉토리 구조 (Directory Structure)
```
├── CMakeLists.txt        # CMake 빌드 설정 파일
├── README.md             # 프로젝트 설명서
└── src/
    ├── Main.cpp          # WinMain 엔트리 포인트 및 메인 게임 루프
    ├── Window.h / .cpp   # Win32 윈도우 래퍼 클래스
    ├── Graphics.h / .cpp # DirectX 11 그래픽스 렌더러
    └── Cube.h / .cpp     # 3D 박스 객체 및 HLSL 셰이더
```

## 빌드 방법 (How to Build)

### 1. Windows (MSVC / Visual Studio)
1. Visual Studio 2019 이상 및 C++ CMake 도구를 설치합니다.
2. 프로젝트 폴더에서 명령 프롬프트(CMD) 또는 PowerShell을 엽니다.
3. 아래 명령어를 실행합니다:
```sh
cmake -B build
cmake --build build --config Release
```
4. `build/Release/DirectX11_Engine.exe`를 실행합니다.

### 2. Linux (Cross Compilation with MinGW-w64)
1. MinGW-w64 및 CMake를 설치합니다:
```sh
sudo apt-get update
sudo apt-get install -y cmake ninja-build g++-mingw-w64-x86-64
```
2. 크로스 컴파일 빌드를 실행합니다:
```sh
cmake -B build -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++
cmake --build build
```
3. 생성된 `build/DirectX11_Engine.exe` 파일은 Windows 환경 또는 Wine을 통해 실행할 수 있습니다.
