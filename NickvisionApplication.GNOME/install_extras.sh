#!/bin/sh

INSTALL_PREFIX="/usr"
if [ -n "${1}" ]
then
    INSTALL_PREFIX="${1}"
fi
echo Install prefix: "${INSTALL_PREFIX}"

if [ "$(basename "$(pwd)")" = "NickvisionApplication.GNOME" ]
then
    cd ..
fi

echo "Installing icons..."
mkdir -p "${INSTALL_PREFIX}"/share/icons/hicolor/scalable/apps
for icon in org.nickvision.application.svg org.nickvision.application-devel.svg
do
    cp ./NickvisionApplication.Shared/Resources/${icon}               \
       "${INSTALL_PREFIX}"/share/icons/hicolor/scalable/apps/
done
mkdir -p "${INSTALL_PREFIX}"/share/icons/hicolor/symbolic/apps
for icon in org.nickvision.application-symbolic.svg  \
                moon-outline-symbolic.svg            \
                sun-outline-symbolic.svg
do
    cp ./NickvisionApplication.Shared/Resources/${icon}               \
       "${INSTALL_PREFIX}"/share/icons/hicolor/symbolic/apps/
done

# echo "Installing GResource..."
# mkdir -p "${INSTALL_PREFIX}"/share/org.nickvision.application
# glib-compile-resources ./NickvisionApplication.GNOME/Resources/org.nickvision.application.gresource.xml
# mv ./NickvisionApplication.GNOME/Resources/org.nickvision.application.gresource \
#    "${INSTALL_PREFIX}"/share/org.nickvision.application/

echo "Installing desktop file..."
mkdir -p "${INSTALL_PREFIX}"/share/applications
cp ./NickvisionApplication.GNOME/org.nickvision.application.desktop \
   "${INSTALL_PREFIX}"/share/applications/

echo "Installing metainfo..."
mkdir -p "${INSTALL_PREFIX}"/share/metainfo
cp ./NickvisionApplication.GNOME/org.nickvision.application.metainfo.xml    \
   "${INSTALL_PREFIX}"/share/metainfo/

echo "Translating desktop file and metainfo..."
python3 ./NickvisionApplication.GNOME/translate_meta.py ${INSTALL_PREFIX}

# echo "Installing mime types..."
# mkdir -p "${INSTALL_PREFIX}"/share/mime/packages
# cp ./NickvisionApplication.GNOME/org.nickvision.application.extension.xml   \
#    "${INSTALL_PREFIX}"/share/mime/packages/
# update-mime-database "${INSTALL_PREFIX}"/share/mime/

echo "Done!"
