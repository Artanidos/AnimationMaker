LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/olaf/Qt/5.8/gcc_64/lib
export LD_LIBRARY_PATH
PATH=$PATH:/home/olaf/Qt/5.8/gcc_64/bin
export PATH
/home/olaf/linuxdeployqt/linuxdeployqt-continuous-x86_64.AppImage AnimationMaker -appimage
mv Application-x86_64.AppImage AnimationMaker-Linux-x86_64-1.1.AppImage

