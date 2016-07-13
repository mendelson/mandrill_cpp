# Output error codes:
* -1: Não foi possível abrir o arquivo de configuração dos codecs.

* -2: Não foi possível carregar o arquivo de configuração dos codecs no buffer de memória.

* -3: O arquivo JSON de configurações de codecs possui erros.

* -4: Nome de codec inválido.

* -5: O arquivo JSON de configurações de codecs não foi carregado em memória ainda.

* -6: Lost connection with the source camera.

* -7: Error writing to socket.

* -8: Error while opening socket.

* -9: Error, no such host.

* -10: Error connectiong.

# Pre-commit practices
* Before commiting your code, make sure to run the following script
```
./scripts/formatCodingStyle.sh
```

# Third party softwares currently in use
* g++ version 4.8.4
* OpenCV version 3.1.0
* GStreamer
* JsonCpp version 1.7.2
* OpenSSL 1_0_2-stable
* ClangFormat version 3.8
* Doxygen Release_1_8_11

# OpenCV
## Environment setup
```
sudo apt-get install libopencv-dev build-essential cmake git libgtk2.0-dev pkg-config python-dev python-numpy libdc1394-22 libdc1394-22-dev libjpeg-dev libpng12-dev libtiff4-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev libtbb-dev libqt4-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip
```
## OpenCV setup
```
mkdir <third_party_repos>/opencv
cd <third_party_repos>/opencv
git clone http://github.com/Itseez/opencv.git <version>
cd <version>
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..
make -j $(nproc)
sudo make install
sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig
```

* To make sure you have the proper version installed, run:
```
pkg-config --modversion opencv
```

# Gstreamer
```
sudo apt-get install gstreamer1.0-alsa gstreamer1.0-fluendo-mp3 gstreamer1.0-plugins-bad gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-ugly gstreamer1.0-tool h264enc graphviz libgstreamer-plugins-*
```

# JsonCpp
```
git clone --branch 1.7.2 https://github.com/open-source-parsers/jsoncpp.git
cd jsoncpp
python amalgamate.py
```

# OpenSSL
```
git clone https://github.com/openssl/openssl.git
cd openssl
git checkout OpenSSL_1_0_2-stable
./config
make depend
make
make test
sudo make install
```

# Clang
## Installation
```
sudo add-apt-repository "deb http://llvm-apt.ecranbleu.org/apt/trusty/ llvm-toolchain-trusty-3.8 main"
wget -O - http://llvm-apt.ecranbleu.org/apt/llvm-snapshot.gpg.key|sudo apt-key add -
sudo apt-get install clang-3.8 lldb-3.8
sudo apt-get install clang-format-3.8
```
* If you intend to edit the clang-format file, make sure to check the documentation [here](http://llvm.org/releases/3.8.0/tools/clang/docs/ClangFormatStyleOptions.html).

# Doxygen
## Install
```
sudo apt-get install flex bison libiconv* make texlive-full
cd <third_party_repos>
git clone https://github.com/doxygen/doxygen.git
cd doxygen
git checkout tags/Release_1_8_11
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
sudo make install
cd ..
cmake -Dbuild_wizard=YES
cmake -Dbuild_doc=YES
make docs
sudo apt-get install doxygen-gui
```

## Generate documentation
```
make doc
```

# Compiling Mandrill-Low
* To compile this module, simply run the following from the project's root directory:
```
make
```

* If you want to speed up the compilation process, you may use mullti-core compilation:
```
make -j $(nproc)
```

# Running Mandrill-Low
```
./scripts/runCore.sh
```

# Versioning
* To indicate a new version, commit, add a new tag to the desired commit, make the branch "latest" point to that tag, as follows:
```
git commit
git tag v<interface>.<version>.<revision>
git push origin v<interface>.<version>.<revision>
git checkout latest
git reset --hard v<interface>.<version>.<revision>
git push -f origin latest
```
