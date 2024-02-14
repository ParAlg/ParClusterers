# pip3 install gdown
gdown --id 1FPqD8Wns7NXTSYDAcK4ZsqUUGABLyZMn # AmazonTitles-670K
gdown --id 16FIzX3TnlsqbrwSJJ2gDih69laezfZWR # Amazon-670K
gdown --id 1sxPHzlnotUKjbtVRe0GuXGfSI7YhBSdc # WikiSeeAlsoTItles-350K
gdown --folder --id  12HiiGWmbLfTEEObs2Y2jiTETZfXDowrn # WikiTitles-500K	


# unzip files and remove zip.
find . -name "*.zip" -exec unzip {} \;
rm *.zip