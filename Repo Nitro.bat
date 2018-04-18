@echo off
color 1f
echo "Fixing shit"
git lfs install
git lfs track *.psd
git lfs track *.tga
git lfs track *.mb
git lfs track *.tif
git lfs track *.mov
git lfs track *.mwv
git lfs track *.zip
git lfs track *.rar
git config pack.windowMemory 100m
git config pack.packSizeLimit 200m
echo '*.png -delta' >> .gitattributes
echo '*.jpg -delta' >> .gitattributes
echo '*.mp3 -delta' >> .gitattributes
echo -
echo -
echo -
echo -
color 20
echo jobs a good'n
pause
