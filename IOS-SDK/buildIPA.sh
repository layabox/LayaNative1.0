#!/bin/sh





PROJECT_PATH=$(pwd)/LayaBox/LayaBox.xcodeproj
SCHEME=LayaBox
CONFIGURATION=Release
TARGET=LayaBox
ARCHIVE_APTH=$(pwd)/build/layabox.xcarchive
EXPORT_PATH=$(pwd)/build
EXPORT_PLIST=$(pwd)/EnterpriseExportOptionsPlist.plist
OLD_IPA_PATH=$(pwd)/build/LayaBox.ipa
IPA_PATH=$(pwd)/build/layabox.ipa
echo “********************clean****************************”
xcodebuild clean -quiet -project ${PROJECT_PATH}  -configuration ${CONFIGURATION} -alltargets #-target ${TARGET} -scheme ${SCHEME}


echo “********************archive****************************”
rm -rf ./build
mkdir ./build
xcodebuild archive -quiet -project ${PROJECT_PATH}  -configuration ${CONFIGURATION}   -archivePath ${ARCHIVE_APTH} -scheme ${SCHEME} #-target ${TARGET}


echo “********************exportArchive****************************”
xcodebuild -quiet -exportArchive -archivePath ${ARCHIVE_APTH} -exportPath ${EXPORT_PATH} -exportOptionsPlist ${EXPORT_PLIST}

mv ${OLD_IPA_PATH} ${IPA_PATH}

