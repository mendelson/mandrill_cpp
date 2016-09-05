git clone https://github.com/gpac/gpac.git
cd gpac
git checkout v0.6.1
./configure --static-mp4box --use-zlib=no
make -j $(nproc)
sudo make install
