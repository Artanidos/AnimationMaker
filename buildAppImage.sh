mkdir AppImage/usr
mkdir AppImage/usr/bin
mkdir AppImage/usr/lib
mkdir AppImage/usr/plugins
mkdir AppImage/usr/plugins/audio
mkdir AppImage/usr/plugins/bearer
mkdir AppImage/usr/plugins/iconengines
mkdir AppImage/usr/plugins/imageformats
mkdir AppImage/usr/plugins/mediaservice
mkdir AppImage/usr/plugins/platforms
mkdir AppImage/usr/plugins/printsupport
mkdir AppImage/usr/plugins/xcbglintegration

cp bin/ffmpeg AppImage/usr/bin
cp ../build-AnimationMaker-Desktop-Release/AnimationMaker AppImage/usr/bin
cp lib/* AppImage/usr/lib
cp -r plugins/* AppImage/usr/plugins
/home/olaf/appimagetool/appimagetool-x86_64.AppImage AppImage
mv Application-x86_64.AppImage AnimationMaker-Linux-x86_64-1.3.AppImage
