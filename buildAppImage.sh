mkdir -p AppImage/usr/lib
mkdir -p AppImage/usr/bin
mkdir -p AppImage/usr/share/applications
mkdir -p AppImage/usr/share/icons/hicolor

# App
cp ../build-AnimationMaker-Desktop_Qt_5_9_1_GCC_64bit-Release/AnimationMaker AppImage/usr/bin
cp binaries/* AppImage/usr/bin
# Desktop, #Icon
cp default.desktop AppImage/usr/share/applications
cp default.svg AppImage/usr/share/icons/hicolor

export LD_LIBRARY_PATH=~/Qt/5.9.1/gcc_64/lib
/home/olaf/linuxdeployqt/linuxdeployqt-continuous-x86_64.AppImage AppImage/usr/share/applications/*.desktop
rm AppImage/AppRun
cp AppRun AppImage
/home/olaf/appimagetool/appimagetool-x86_64.AppImage AppImage
mv AnimationMaker-x86_64.AppImage AnimationMaker-Linux-x86_64-1.4.1.AppImage

# Clean up
rm -r AppImage/*
rmdir AppImage
