#!/bin/bash

# Initialize enviroment
BASE_PATH=`pwd`


# SVN parameters

SVN_CMD=/usr/bin/svn
SVN_URL=http://10.1.5.8/Hotpot/SAF/trunk/PluginFramework/
SVN_USER=xiaowei.oms
SVN_PSWD=omstest

# Workspace directories
DIR_SRC=/home/cm64990/jenkins/workspace/SAF/PluginFramework/
DIR_UnitTest=/home/cm64990/jenkins/workspace/SAF/PluginFramework/tests/
DIR_UnitTestJar=/home/cm64990/jenkins/unitTest/libs/

# adb directory
DIR_ADB=/home/cm64990/jenkins/android-sdk-linux-r16/platform-tools

# Function InitWorkspace()

InitTest()
{
 echo "** Initializing tests ..."


cd $DIR_UnitTest

chmod 777 -R *

rm -f AndroidManifest.xml 

for dirdel in $DIR_UnitTest/* ; do
if [ -d $dirdel ] ; then
  rm -rf $dirdel
fi
done

echo "-> Done: Initalize tests successfully!"

}
InitTest

# Functions DownloadSource()
DownloadSource()
{
  #retval=0
  #SVN Source
  {
    	#svn co $SVN_URL --force --username $SVN_USER --password $SVN_PSWD $DIR_SRC
 	svn update $DIR_SRC --username $SVN_USER --password $SVN_PSWD 
  } 

ret=$?
echo $ret
if [ $ret -eq 1 ]; then
   echo " Connnect to svn server failed!"
   echo
   exit 0
fi
   echo " Connect to svn server successed!"
   echo "--> Done:Source code checkout successfully!"
}
DownloadSource

sed '/instr/,+1 s/\(android:name="\).*"/\1com.zutubi.android.junitreport.JUnitReportTestRunner"/' AndroidManifest.xml >> test.xml

if [ -f test.xml ]; then
 mv test.xml AndroidManifest.xml
fi

cp -a $DIR_UnitTestJar $DIR_UnitTest


ant clean && ant uninstall 
ant emma debug install test


