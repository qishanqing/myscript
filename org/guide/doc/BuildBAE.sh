#!/bin/bash

SVN_CMD=/usr/bin/svn
SVN_URL_1=http://10.1.5.8/Hotpot/BAE/trunk/meeting
SVN_URL_2=http://10.1.5.8/Hotpot/BAE/trunk/baogong
SVN_URL_3=http://10.1.5.8/Hotpot/BAE/trunk/food
SVN_URL_4=http://10.1.5.8/Hotpot/BAE/trunk/feedback
SVN_USER=hemaoyue.oms
SVN_PSWD=hemaoyue

# shell目录设定：HotPotLib  
#               _MACOSX    HotPotLib

# Workspace directories

DIR_src=/var/lib/jenkins/workspace/BAE/resources
DIR_install=/var/lib/jenkins/workspace/BAE/resources/MobileInstall
DIR_widget=/var/lib/jenkins/workspace/BAE/resources/MobileInstall/Applications
DIR_Build_APP=/var/lib/jenkins/workspace/BAE/build/Distribution-iphoneos
DIR_Build_IPA=/var/lib/jenkins/workspace/BAE/build
DIR_Report=/var/lib/jenkins/workspace/BAE/clangScanBuildReports


if [ -d $DIR_install ] ; then
  rm -rf $DIR_install
  echo "-> Done: Delete MobileInstall successfully!"
fi

rm -rf $DIR_src/MobileInstall.zip
echo "-> Done: Delete MobileInstall.zip successfully!"


cp -rf $DIR_src/../MobileInstall.zip $DIR_src



# 1.解压如下路径下的压缩包HotPotLib/resources/MobileInstall.zip


unzip -o $DIR_src/MobileInstall.zip -d $DIR_src


echo "-> Done: Unzip MobileInstall successfully!"

# 2.删除压缩包
 
rm $DIR_src/MobileInstall.zip


echo "-> Done: Delete Zip_before successfully!"


# 3.替换如下路径下的widget，HotPotLib/resources/MobileInstall

#删除原widget文件夹
rm -r -f $DIR_widget/00
rm -r -f $DIR_widget/01
rm -r -f $DIR_widget/02
rm -r -f $DIR_widget/03

echo "-> Done: Delete Widget_before successfully!"

#下载新widget文件夹
#svn co $SVN_URL_1 $SVN_URL_2 $SVN_URL_3 $SVN_URL_4 --force --username $SVN_USER --password $SVN_PSWD $DIR_widget
svn export $SVN_URL_1 --force --username $SVN_USER --password $SVN_PSWD $DIR_widget/00
svn export $SVN_URL_2 --force --username $SVN_USER --password $SVN_PSWD $DIR_widget/01
svn export $SVN_URL_3 --force --username $SVN_USER --password $SVN_PSWD $DIR_widget/02
svn export $SVN_URL_4 --force --username $SVN_USER --password $SVN_PSWD $DIR_widget/03

#重命名新的widget为1、2、3、4
#mv $DIR_widget/meeting $DIR_widget/00
#mv $DIR_widget/baogong $DIR_widget/01
#mv $DIR_widget/food $DIR_widget/02
#mv $DIR_widget/feedback $DIR_widget/03

echo "-> Done: Update and Rename Widget_new successfully!"

# 4.压缩MobileInstall文件夹，生成新的MobileInstall.zip


cd $DIR_src

zip -r MobileInstall.zip MobileInstall __MACOSX



echo "-> Done: Release new zip successfully!"

rm -r -f MobileInstall __MACOSX

# 5.打开工程

cd /var/lib/jenkins/workspace/BAE



# 6.选择product－>clean

/usr/bin/xcodebuild clean -sdk iphoneos5.1 -configuration Distribution



/usr/bin/security unlock-keychain -p cmcc234



# 7. 编译工程，生成新的app，构建静态代码检查目录
rm -r -f $DIR_Report/*

scan-build -o /var/lib/jenkins/workspace/BAE/clangScanBuildReports /usr/bin/xcodebuild -project HotPot.xcodeproj -target HotPot -configuration Distribution -sdk iphoneos5.1

cd $DIR_Report/*


if [ -f index.html ] ; then
	cp -r -f $DIR_Report/*/* $DIR_Report
	rm -r -f $DIR_Report/`date +%Y-%m-%d`*
else
	cp -r -f $DIR_src/../../index.html $DIR_Report
fi


# 8. 发布成为ipa

/usr/bin/xcrun -sdk iphoneos PackageApplication -v "$DIR_Build_APP/HotPot.app" -o "$DIR_Build_IPA/HotPot`date +%Y%m%d-%H%M`.ipa" --sign "iPhone Distribution: China Mobile Communication Co. Ltd., Research Institute"



# 9.上传编译后的app文件至指定服务器

#Put app from local to ftp server

if ping -c 3 10.2.48.148;then
echo "Ftp server works normally!"

else
echo "Ftp server is down again!"
exit 0

fi

ftp -nv <<-EOC
	
open 10.2.48.148

user omsftpadmin cmcc1234

prompt

cd ./HotPot-BAE

mkdir `date +%Y%m%d`

cd "`date +%Y%m%d`"

lcd $DIR_Build_IPA

mput *.ipa

close

bye

EOC

echo "-> Done: Put ipa file successfully!"

cd $DIR_Build_IPA

rm *.ipa

