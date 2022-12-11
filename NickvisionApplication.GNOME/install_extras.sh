#!/bin/bash

INSTALL_PREFIX="/usr"
if [ ! -z $1 ]
then
	INSTALL_PREFIX=$1
fi
echo Install prefix: $INSTALL_PREFIX

SOURCE_PREFIX="."
if [ ${PWD##*/} == "NickvisionApplication.GNOME" ]
then
	SOURCE_PREFIX=".."
fi

echo Installing icons...
mkdir -p $INSTALL_PREFIX/share/icons/hicolor/scalable/apps
cp $SOURCE_PREFIX/NickvisionApplication.Shared/Resources/org.nickvision.application.svg $INSTALL_PREFIX/share/icons/hicolor/scalable/apps/
cp $SOURCE_PREFIX/NickvisionApplication.Shared/Resources/org.nickvision.application-devel.svg $INSTALL_PREFIX/share/icons/hicolor/scalable/apps/
mkdir -p $INSTALL_PREFIX/share/icons/hicolor/symbolic/apps
cp $SOURCE_PREFIX/NickvisionApplication.Shared/Resources/org.nickvision.application-symbolic.svg $INSTALL_PREFIX/share/icons/hicolor/symbolic/apps/

#echo Installing GResource...
#mkdir -p $INSTALL_PREFIX/share/org.nickvision.application
#glib-compile-resources $SOURCE_PREFIX/NickvisionApplication.GNOME/Resources/org.nickvision.application.gresource.xml
#mv $SOURCE_PREFIX/NickvisionApplication.GNOME/Resources/org.nickvision.application.gresource $INSTALL_PREFIX/share/org.nickvision.application/

echo Installing desktop file...
mkdir -p $INSTALL_PREFIX/share/applications
cp $SOURCE_PREFIX/NickvisionApplication.GNOME/org.nickvision.application.desktop $INSTALL_PREFIX/share/applications/

echo Installing metainfo...
mkdir -p $INSTALL_PREFIX/share/metainfo
cp $SOURCE_PREFIX/NickvisionApplication.GNOME/org.nickvision.application.metainfo.xml $INSTALL_PREFIX/share/metainfo/

echo Done!