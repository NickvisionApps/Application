#!/bin/sh

if [ -z $1 || -z $2 ]
then
    echo "Usage: $0 prefix runtime"
    exit 1
fi

# Initialize script
CURRENT_PWD=$(pwd)
set -eu

# Change pwd to script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd $SCRIPT_DIR

# Load variables
APP_ID="org.nickvision.application"
PROJECT="Nickvision.Application.GNOME"
PREFIX=$1
RUNTIME=$2
BIN_DIR="$PREFIX/bin"
LIB_DIR="$PREFIX/lib/$APP_ID"
DATA_DIR="$PREFIX/share"

echo "=============================================================="
echo " Publishing and Installing $APP_ID"
echo "=============================================================="

# Create main directories
mkdir -p $BIN_DIR
mkdir -p $LIB_DIR
mkdir -p $DATA_DIR

# Publish application
echo "---------------------------"
echo " Publishing application... "
echo "---------------------------"
export DOTNET_CLI_TELEMETRY_OPTOUT=1
if [ -n "$container" ]
then
    dotnet publish -c Release --source "$CURRENT_PWD/nuget-sources" --source "/usr/lib/sdk/dotnet10/nuget/packages" "../../$PROJECT/$PROJECT.csproj" --runtime $RUNTIME --self-contained true
else
    dotnet publish -c Release "../../$PROJECT/$PROJECT.csproj" --runtime $RUNTIME --self-contained true
fi
cp -a ../../$PROJECT/bin/Release/net10.0/$RUNTIME/publish/. $LIB_DIR

# Create desktop file
echo "---------------------------"
echo " Creating desktop file... "
echo "---------------------------"
DESKTOP_FILE="$DATA_DIR/applications/$APP_ID.desktop"
mkdir -p "$(dirname $DESKTOP_FILE)"
cp "$APP_ID.desktop.in" $DESKTOP_FILE
sed -i "s|@LIB_DIR@|$LIB_DIR|g" $DESKTOP_FILE
sed -i "s|@OUTPUT_NAME@|$PROJECT|g" $DESKTOP_FILE

# Create executable launcher script
echo "---------------------------"
echo " Creating launcher... "
echo "---------------------------"
LAUNCHER_FILE="$BIN_DIR/$APP_ID"
cp "$APP_ID.in" $LAUNCHER_FILE
sed -i "s|@LIB_DIR@|$LIB_DIR|g" $LAUNCHER_FILE
sed -i "s|@OUTPUT_NAME@|$PROJECT|g" $LAUNCHER_FILE
chmod +x $LAUNCHER_FILE

# Copy metadata file
echo "---------------------------"
echo " Copying metadata... "
echo "---------------------------"
METADATA_FILE="$DATA_DIR/metainfo/$APP_ID.metainfo.xml"
mkdir -p "$(dirname $METADATA_FILE)"
cp "$APP_ID.metainfo.xml" $METADATA_FILE

# Copy icons
echo "---------------------------"
echo " Copying icons... "
echo "---------------------------"
SCALABLE_ICON_DIR="$DATA_DIR/icons/hicolor/scalable/apps"
SYMBOLIC_ICON_DIR="$DATA_DIR/icons/hicolor/symbolic/apps"
mkdir -p $SCALABLE_ICON_DIR
mkdir -p $SYMBOLIC_ICON_DIR
cp "../$APP_ID.svg" "$SCALABLE_ICON_DIR/$APP_ID.svg"
cp "../$APP_ID-devel.svg" "$SCALABLE_ICON_DIR/$APP_ID-devel.svg"
cp "../$APP_ID-symbolic.svg" "$SYMBOLIC_ICON_DIR/$APP_ID-symbolic.svg"

# Update gtk icon cache
echo "---------------------------"
echo " Updating icon cache... "
echo "---------------------------"
gtk-update-icon-cache

# Update desktop database
echo "---------------------------"
echo " Updating desktop files... "
echo "---------------------------"
update-desktop-database

# Restore pwd
cd $CURRENT_PWD

echo "---------------------------"
echo " Completed successfully! "
echo "---------------------------"