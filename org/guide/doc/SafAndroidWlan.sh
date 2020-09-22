#!/bin/bash

# Initialize enviroment
BASE_PATH=`pwd`

# SVN parameters
SVN_CMD=/usr/bin/svn
SVN_PluginsWlan=http://10.1.5.8/Hotpot/SAF/trunk/Plugins-WLAN/
SVN_WlanService=http://10.1.5.8/Hotpot/SAF/trunk/Plugins-WLAN/WlanService/
SVN_WlanServiceSDK=http://10.1.5.8/Hotpot/SAF/trunk/Plugins-WLAN/WlanServiceSDK/
SVN_WlanDemo=http://10.1.5.8/Hotpot/SAF/trunk/Plugins-WLAN/WlanDemo/
SVN_PluginFrameworkSDK=http://10.1.5.8/Hotpot/SAF/trunk/PluginFrameworkSDK/
SVN_EwalkLib=http://10.1.5.8/Wlan/eWalk_Android/trunk/EwalkLib/
SVN_USER=xiaowei.oms
SVN_PSWD=omstest

# Workspace directories
DIR_SAF=/home/cm64990/jenkins/workspace/SAF/
DIR_Plugins_WLAN=/home/cm64990/jenkins/workspace/SAF/Plugins-WLAN/
DIR_WlanService=/home/cm64990/jenkins/workspace/SAF/Plugins-WLAN/WlanService/
DIR_WlanService_apk=/home/cm64990/jenkins/workspace/SAF/Plugins-WLAN/WlanService/bin/
DIR_WlanServiceSDK=/home/cm64990/jenkins/workspace/SAF/Plugins-WLAN/WlanServiceSDK/
DIR_WlanServiceSDK_jar=/home/cm64990/jenkins/workspace/SAF/Plugins-WLAN/WlanServiceSDK/bin/
DIR_WlanDemo=/home/cm64990/jenkins/workspace/SAF/Plugins-WLAN/WlanDemo/
DIR_WlanDemo_apk=/home/cm64990/jenkins/workspace/SAF/Plugins-WLAN/WlanDemo/bin/
DIR_PluginFrameworkSDK=/home/cm64990/jenkins/workspace/SAF/PluginFrameworkSDK/
DIR_EwalkLib=/home/cm64990/jenkins/workspace/Wlan/EwalkLib/

# Function InitWorkspace()

InitEwalkLib()
{
 
echo "** Initializing EwalkLib ..."

cd $DIR_EwalkLib

chmod 777 -R *

for dirdel in $DIR_EwalkLib/* ; do
if [ -d $dirdel ] ; then
  rm -rf $dirdel
fi
done

echo "-> Done: Initalize EwalkLib successfully!"

}
InitEwalkLib

InitWlanService()
{
echo "** Initializing WlanService ..."

cd $DIR_WlanService

chmod 777 -R *

rm -f AndroidManifest.xml

for dir1del in $DIR_WlanService/* ; do
if [ -d $dir1del ] ; then
  rm -rf $dir1del
fi
done
	echo "-> Done: Initalize WlanService successfully!"
#	return 0
}
InitWlanService

InitWlanServiceSDK()
{
echo "** Initializing WlanServiceSDK ..."

cd $DIR_WlanServiceSDK

chmod 777 -R *

for dir2del in $DIR_WlanServiceSDK/* ; do
if [ -d $dir2del ] ; then
  rm -rf $dir2del
fi
done
	echo "-> Done: Initalize WlanServiceSDK successfully!"
#	return 0
}
InitWlanServiceSDK

InitPluginFrameworkSDK()
{
echo "** Initalizing FrameworkSDK ..."

cd $DIR_PluginFrameworkSDK
chmod 777 -R *
	
rm -f AndroidManifest.xml

for dir2del in $DIR_PluginFrameworkSDK/* ; do
if [ -d $dir2del ];then
  rm -rf $dir2del

fi
done
	echo "-> Done: Initalize PluginFrameworkSDK successfully!"

}

InitPluginFrameworkSDK


InitWlanDemo()
{
echo "** Initializing WlanDemo ..."

cd $DIR_WlanDemo

chmod 777 -R *

rm -f AndroidManifest.xml

for dir3del in $DIR_WlanDemo/* ; do
if [ -d $dir3del ] ; then
  rm -rf $dir3del
fi
done
	echo "-> Done: Initalize WlanDemo successfully!"
	echo "-> Done: Initalize WorkSpace successfully!"
#	return 0
}
InitWlanDemo


# Functions DownloadSource()
DownloadSource()
{
  #retval=0
  #SVN Source
  {
    	#svn co $SVN_WlanService --force --username $SVN_USER --password $SVN_PSWD $DIR_WlanService
	svn update $DIR_Plugins_WLAN --username $SVN_USER --password $SVN_PSWD
	#svn co $SVN_WlanServiceSDK --force --username $SVN_USER --password $SVN_PSWD $DIR_WlanServiceSDK
	#svn update $DIR_WlanServiceSDK --username $SVN_USER --password $SVN_PSWD
    	#svn co $SVN_WlanDemo --force --username $SVN_USER --password $SVN_PSWD $DIR_WlanDemo
	#svn update $DIR_WlanDemo --username $SVN_USER --password $SVN_PSWD
	svn update $DIR_PluginFrameworkSDK --username $SVN_USER --password $SVN_PSWD
	svn update $DIR_EwalkLib --username $SVN_USER --password $SVN_PSWD

  } 

ret1=$?
echo $ret1
if [ $ret1 -eq 1 ]; then
   echo " Connnect to svn server failed!"
   echo
   exit 0
fi
   echo " Connect to svn server successed!"
   echo "-> Done: Source code checkout successfully!"
}

DownloadSource

#Modify versionName in the AndroidManifest.xml

cd $DIR_WlanService

chmod 777 -R *.*
verCode=`awk -F \" '/android:versionCode=/{print $2}' VC.txt`

	echo verCode=$verCode

#sed 's/\(android:versionCode="\)[^"]*"/\1'`svn info | grep Revision: | head -1 |  awk '{print $2}'`'"/;s/\(android:versionName="\)[^"]*"/\1'3.0.1'-'`date +%Y%m%d-%H%M`'"/' AndroidManifest.xml >> temp.xml
sed 's/\(android:versionCode="\)[^"]*"/\1'$verCode'"/;s/\(android:versionName="\)[^"]*"/\1'1.0'-'`date +%Y%m%d-%H%M`'"/' AndroidManifest.xml >> temp.xml

  	mv -f temp.xml AndroidManifest.xml

sed 's/\(android:versionCode="\)[^"]*"/\1'$((verCode+1))'"/' VC.txt >> temp.txt

 	mv -f temp.txt VC.txt

#Update AnroidManifest.xml to svn server

svn ci -m "The AndroidManifest.xml has been modified" AndroidManifest.xml

# Compile WlanService

ant clean && ant release findbugs

#Modify versionName in the AndroidManifest.xml

cd $DIR_WlanDemo
chmod 777 -R *
verCode=`awk -F \" '/android:versionCode=/{print $2}' VC.txt`

	echo verCode=$verCode

#sed 's/\(android:versionCode="\)[^"]*"/\1'`svn info | grep Revision: | head -1 |  awk '{print $2}'`'"/;s/\(android:versionName="\)[^"]*"/\1'3.0.1'-'`date +%Y%m%d-%H%M`'"/' AndroidManifest.xml >> temp.xml
sed 's/\(android:versionCode="\)[^"]*"/\1'$verCode'"/;s/\(android:versionName="\)[^"]*"/\1'1.0'-'`date +%Y%m%d-%H%M`'"/' AndroidManifest.xml >> temp.xml

  	mv -f temp.xml AndroidManifest.xml

sed 's/\(android:versionCode="\)[^"]*"/\1'$((verCode+1))'"/' VC.txt >> temp.txt

 	mv -f temp.txt VC.txt


#Update AnroidManifest.xml to svn server

#svn ci -m "The AndroidManifest.xml has been modified" AndroidManifest.xml

# Compile WlanDemo

ant clean && ant release findbugs

#check whether apk file existed or not

files1=$(ls $DIR_WlanService_apk/*.apk 2>/dev/null | wc -l)
files2=$(ls $DIR_WlanDemo_apk/*.apk 2>/dev/null | wc -l)
echo a=$files1
echo b=$files2
 
	if [[ $files1 != "0" ]] || [[ $files2 != "0" ]] 

then
  echo "**Build apk file successed!"
  echo "->Start to put apk from local to ftp server!"
  
else 
  echo "**Build apk file failed!"
  exit 0

	fi	

#Put apk from local to ftp server
if ping -c 3 10.2.48.148;then

echo "Ftp server works normally!"

else

echo "Ftp server is down again!"
exit 0

fi

ftp -nv <<EOC
	
open 10.2.48.148

user omsftpadmin cmcc1234

prompt

cd ./SAF-framework

if [ ! -x "`date +%Y%m%d`" ]; then

mkdir `date +%Y%m%d`

echo "Create the folder of today successfully!"

fi

echo "The folder of today has exsited!"

cd "`date +%Y%m%d`"

lcd $DIR_WlanService_apk

mput *.apk

lcd $DIR_WlanDemo_apk

mput *.apk

lcd $DIR_WlanServiceSDK_jar

mput classes.jar

rename classes.jar WlanServiceSDK-`date +%Y%m%d%H%M`.jar

close

bye

EOC

echo "-> Done: Put all files successfully!"

