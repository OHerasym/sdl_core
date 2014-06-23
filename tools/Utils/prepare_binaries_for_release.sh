# /usr/bin/sh
# This scropt is preparing binaries files to release in svn
# You should setup build path for your system 
# and release name
#
RELEASE_NAME=SDL_RB_B3.5

DEST=/media/Media/development/CM/release/$RELEASE_NAME
PROJ_DIR=/media/Media/development/applink

WEB_BUILD_DIR=/media/Media/development/CM/applink-build
QT_BUILD_DIR=/media/Media/development/CM/qtcreator-build
WEB_BUILD_DIR_QNX=/media/Media/development/CM/applink-build-qnx

# mkdir, cd into it
mkcd () {
mkdir -p "$*"
cd "$*"
}

function chapter() {
echo "-----------------------------------------------------------------"
echo $1
echo "-----------------------------------------------------------------"

}

chapter "Building under linux webHMI"
mkcd $WEB_BUILD_DIR
cmake -DCMAKE_BUILD_TYPE=Release $PROJ_DIR
make 
make install

#chapter "Building under linux qtHMI"
#mkcd $QT_BUILD_DIR
#cmake -DCMAKE_BUILD_TYPE=Release -DHMI2=ON $PROJ_DIR 
#make 
#make install

chapter "Building under qnx webHMI"
mkcd $WEB_BUILD_DIR_QNX
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$PROJ_DIR/qnx_6.5.0_linux_x86.cmake $PROJ_DIR 
make 
make install

chapter "creating dir tree"
mkdir -pv $DEST/binaries/{ubuntu1204/{webHMI,qtHMI},qnx650/webHMI}
 
chapter "archive HMI and copy to dest"
mkcd $PROJ_DIR/src/components
tar -pczf $DEST/HMI.tar.gz HMI
cd $DEST
cp ./HMI.tar.gz $DEST/binaries/ubuntu1204/webHMI/
cp ./HMI.tar.gz $DEST/binaries/qnx650/webHMI/
rm $DEST/HMI.tar.gz HMI

chapter "copy binaries LinuxWEB"
cp -v $WEB_BUILD_DIR/bin/* $DEST/binaries/ubuntu1204/webHMI/
echo "HMI/index.html" >  $DEST/binaries/ubuntu1204/webHMI/hmi_link 

chapter "copy binaries QnxWeb"
cp -v $WEB_BUILD_DIR_QNX/bin/* $DEST/binaries/qnx650/webHMI/
echo "HMI/index.html" > $DEST/binaries/qnx650/webHMI/hmi_link 

#chapter "copy binaries LinuxQt"
#cp -v $QT_BUILD_DIR/bin/smartDeviceLinkCore $DEST/binaries/ubuntu1204/qtHMI/smartDeviceLinkCore
#cp -v $QT_BUILD_DIR/bin/smartDeviceLink.ini  $DEST/binaries/ubuntu1204/qtHMI/smartDeviceLink.ini 
#cp -v $QT_BUILD_DIR/bin/policy_table.json $DEST/binaries/ubuntu1204/qtHMI/policy_table.json
#cp -v $QT_BUILD_DIR/bin/start_hmi.sh $DEST/binaries/ubuntu1204/qtHMI/start_hmi.sh
#cp -r -v $QT_BUILD_DIR/bin/hmi $DEST/binaries/ubuntu1204/qtHMI/hmi



