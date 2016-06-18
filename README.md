## Output error codes:
* -1: Não foi possível abrir o arquivo de configuração dos codecs.

* -2: Não foi possível carregar o arquivo de configuração dos codecs no buffer de memória.

* -3: O arquivo JSON de configurações de codecs possui erros.

* -4: Nome de codec inválido.

* -5: O arquivo JSON de configurações de codecs não foi carregado em memória ainda.

* -6: Lost connection with the source camera.

## Third party softwares currently in use
* g++ version 4.8.4
* OpenCV version 3.1.0
* JsonCpp version 1.7.2

## OpenCV
# Environment setup
```
$ sudo apt-get -y install libopencv-dev build-essential cmake git libgtk2.0-dev pkg-config python-dev python-numpy libdc1394-22 libdc1394-22-dev libjpeg-dev libpng12-dev libtiff4-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev libtbb-dev libqt4-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip
```
# OpenCV setup
```
$ mkdir <third_party_repos>/opencv
$ cd <third_party_repos>/opencv
$ git clone http://github.com/Itseez/opencv.git <version>
$ cd <version>
$ mkdir build
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..
$ make -j $(nproc)
$ sudo make install
$ sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
$ sudo ldconfig
```

## JsonCpp
```
$ git clone --branch 1.7.2 https://github.com/open-source-parsers/jsoncpp.git
$ cd jsoncpp
$ python amalgamate.py
```

## Compiling Mandrill-Low
..* To compile this module, simply run the following from the project's root directory:
```
$ make
```

..* If you want to speed up the compilation process, you may make use of mullti-core compilation:
```
$ make -j $(nproc)
```
