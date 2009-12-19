#! /bin/sh

dir="temp"
dmg="dmg/rolisteam.dmg"

rm -rf "$dir"
mkdir "$dir"
cp -R bin/rolisteam.app "$dir"
cp COPYRIGHT "$dir"
cp AUTHORS "$dir"
cp COPING.txt "$dir"
cp -R resources/doc "$dir"/documentation
hdiutil create -srcfolder "$dir" -volname "rolisteam" "$dmg"
hdiutil internet-enable -yes "$dmg"



