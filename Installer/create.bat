copy C:\SourceCode\build-AnimationMaker-Desktop-Release\release\AnimationMaker.exe packages\com.vendor.product\data
copy C:\SourceCode\build-AnimationMaker-Desktop-Release\Widgets\release\Widgets.dll packages\com.vendor.product\data
xcopy /S /Y C:\SourceCode\AnimationMaker\winlib\*.* packages\com.vendor.product\data
mkdir 
copy C:\SourceCode\build-AnimationMaker-Desktop-Release\plugins\Pie\release\Pie.dll packages\com.vendor.product\data\plugins 
copy C:\SourceCode\build-AnimationMaker-Desktop-Release\plugins\Html\release\Html.dll packages\com.vendor.product\data\plugins 

C:\Qt\Tools\QtInstallerFramework\3.0\bin\binarycreator.exe -f -c config\config.xml -p packages AnimationMakerSetup.exe