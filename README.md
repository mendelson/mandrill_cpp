# Pre-commit practices
* Before commiting your code, make sure to run the following script
```
./scripts/formatCodingStyle.sh
```
* In case you are commiting any changes from a shared machine, you may use the following:
```
git -c user.name='someone' -c user.email='some@one.org' commit -m '...'
```

# Third party softwares currently in use
* g++ version 4.8.4
* OpenCV version 3.1.0
* GStreamer
* JsonCpp version 1.7.2
* OpenSSL 1_0_2-stable
* ClangFormat version 3.8
* Doxygen Release_1_8_11
* Ubuntu 16.04 LTS (no updates)

# Dependencies
Please, run the following prior to any other installation.
```
sudo apt-get install libopencv-dev build-essential cmake git libgtk2.0-dev pkg-config python-dev python-numpy libdc1394-22 libdc1394-22-dev libjpeg-dev libpng12-dev libtiff5-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libxine2-dev libv4l-dev libtbb-dev libqt4-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip gstreamer1.0-alsa gstreamer1.0-fluendo-mp3 gstreamer1.0-plugins-bad gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-ugly gstreamer1.0-tool graphviz libgstreamer-plugins-* gstreamer-tools libgstreamer1.0-0 libgstreamer1.0-dev
```

or just run:
```
./script/mandrillSetup.sh
```

# Gstreamer
If, for some reason, you haven't run the dependencies installation as previously described, run:
```
sudo apt-get install gstreamer1.0-alsa gstreamer1.0-fluendo-mp3 gstreamer1.0-plugins-bad gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-ugly gstreamer1.0-tool h264enc graphviz libgstreamer-plugins-* gstreamer-tools libgstreamer1.0-0 libgstreamer1.0-dev
```

# JsonCpp
```
git clone --branch 1.7.2 https://github.com/open-source-parsers/jsoncpp.git
cd jsoncpp
python amalgamate.py
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

If you are not willing to use the script above, the executable receives the following parameters:
```
./bin/MandrillCore <url-high-resolution> <url-low-resolution> <camera-uuid> <socket-number>
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
