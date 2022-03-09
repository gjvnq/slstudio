# Compiling on macOS

## Steps

1. Install [Homebrew](https://brew.sh).
2. Install dependencies: `brew install qt@5 vtk pcl opencv@2 glew flann eigen boost libdc1394 hidapi ffmpeg@4`
3. Clone the project and enter it: `git clone github.com/jakobwilm/slstudio && cd slstudio`
4. Run: `export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:$(pwd)"`
5. Run: `export PATH="/usr/local/opt/qt@5/bin:$PATH"`
6. Run: `qmake src/SLStudio.pro`
7. Run: `make`
8. Start: `open bin/SLStudio.app`

Warning: to run the binary file directly, run:
```bash
export DYLD_FALLBACK_LIBRARY_PATH=/usr/local/opt/ffmpeg@4/lib
bin/SLStudio.app/Contents/MacOS/SLStudio
```

## Tested on

* macOS 11.6.4
* Qt 5.15.2 (at `/usr/local/opt/qt@5`)
* VTK 9.1.0 (at `/usr/local/Cellar/vtk/9.1.0_3`)
* PCL 1.12.1
* OpenCV 2.4.13.7 (at `/usr/local/Cellar/opencv@2/2.4.13.7_12`)
* GLEW 2.2.0
* Eigen 3.4.0
* Boost 1.78.0 (at `/usr/local/Cellar/boost/1.78.0_1`)
* Libdc1394 2.2.6
* Hidapi 0.11.2
* FFMPEG 4.4.1 (at `/usr/local/opt/ffmpeg@4/`)
libresample--0.1.3
* With `PKG_CONFIG_PATH` environment variable with value `/usr/local/opt/opencv@2/lib/pkgconfig:/usr/local/opt/coreutils/lib/pkgconfig:/usr/local/opt/qt@5/lib/pkgconfig:/usr/local/custom/lib/pkgconfig:/usr/local/lib/pkgconfig:/usr/lib/pkgconfig:` (I had placed the `boost.pc` file at `/usr/local/custom/lib/pkgconfig`)

## Notes

  * You will probably need to adjust `boost.pc` to match your system.
  * You will get numerous linker warning like `was built for newer macOS version (11.0) than being linked (10.13)`.