#!/bin/sh
./buildSDK.sh
./build-ios-sdk2.sh
cp -rf ./LayaRuntime-iOS/resource ./LayaBox/resource
./buildIPA.sh

