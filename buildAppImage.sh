mkdir -p AppImage/usr/lib
mkdir -p AppImage/usr/bin
mkdir -p AppImage/plugins/gsap
mkdir -p AppImage/usr/share/applications
mkdir -p AppImage/usr/share/icons/hicolor

# App
cp ~/Sourcecode/build-AnimationMaker-Desktop_Qt_5_9_1_GCC_64bit-Release/AnimationMaker AppImage/usr/bin
cp binaries/* AppImage/usr/bin
# Lib
cp ~/Sourcecode/build-AnimationMaker-Desktop_Qt_5_9_1_GCC_64bit-Release/Widgets/libWidgets.so* AppImage/usr/lib
cp ~/Sourcecode/build-AnimationMaker-Desktop_Qt_5_9_1_GCC_64bit-Release/License/libLicense.so* AppImage/usr/lib
# Plugins
cp ~/Sourcecode/build-AnimationMaker-Desktop_Qt_5_9_1_GCC_64bit-Release/plugins/Pie/libPie.so AppImage/plugins
cp ~/Sourcecode/build-AnimationMaker-Desktop_Qt_5_9_1_GCC_64bit-Release/plugins/Html/libHtml.so AppImage/plugins
cp plugins/Html/gsap/* AppImage/plugins/gsap

# Desktop, #Icon
cp default.desktop AppImage/usr/share/applications
cp default.svg AppImage/usr/share/icons/hicolor

export PATH=$PATH:~/Qt/5.9.1/gcc_64/bin
export LD_LIBRARY_PATH=~/Sourcecode/AnimationMaker/AppImage/usr/lib:~/Qt/5.9.1/gcc_64/lib
/home/art/Apps/linuxdeployqt/linuxdeployqt-continuous-x86_64.AppImage AppImage/usr/share/applications/*.desktop
rm AppImage/AppRun
cp AppRun AppImage
/home/art/Apps/appimagetool/appimagetool-x86_64.AppImage AppImage
mv AnimationMaker-x86_64.AppImage AnimationMaker-Linux-x86_64-1.8.1.AppImage

# Clean up
# rm -r AppImage/*
