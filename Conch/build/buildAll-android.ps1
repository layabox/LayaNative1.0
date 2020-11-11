cd common\proj.android\jni\
E:\Android\android-sdk\ndk-bundle\ndk-build.cmd
cd ..\..\..\
cd render\proj.android\jni\
E:\Android\android-sdk\ndk-bundle\ndk-build.cmd
cd ..\..\..\
copy common\proj.android\obj\local\armeabi-v7a\libcommon.a ..\libs\android32\
copy render\proj.android\obj\local\armeabi-v7a\librender.a ..\libs\android32\
copy common\proj.android\obj\local\arm64-v8a\libcommon.a ..\libs\android64\
copy render\proj.android\obj\local\arm64-v8a\librender.a ..\libs\android64\
$tmpf="..\source\conch\JCConch.cpp"
(Get-Item $tmpf).LastWriteTime = Get-Date
cd conch\proj.android\jni\
E:\Android\android-sdk\ndk-bundle\ndk-build.cmd
cd ..\..\..\
copy conch\proj.android\libs\  conch\proj.android_studio\conch5\ -Force -Recurse
