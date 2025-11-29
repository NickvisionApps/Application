#!/bin/sh

if [ -z $1 || -z $2 || -x $3 || -x $4 ];
then
    echo "Usage: $0 id project prefix runtime"
    exit 1
fi

set -eu

# Load variables
$APP_ID=$1
$PROJECT=$2
$PREFIX=$3
$RUNTIME=$4
BIN_DIR="/$PREFIX/bin"
LIB_DIR="/$PREFIX/lib/$APP_ID"
DATA_DIR="/$PREFIX/share"

# Create main directories
mkdir -p $BIN_DIR
mkdir -p $LIB_DIR
mkdir -p $DATA_DIR

# Publish application
dotnet publish -c Release --source "./nuget-sources" --source "/usr/lib/sdk/dotnet10/nuget/packages" "../$PROJECT/$PROJECT.csproj" --runtime $RUNTIME --self-contained true
cp -r --remove-destination "/run/build/$PROJECT/bin/Release/net10.0/$RUNTIME/publish/*" $LIB_DIR

# Create desktop file
DESKTOP_FILE="$DATA_DIR/applications/$APP_ID.desktop"
mkdir -p "$(dirname $DESKTOP_FILE)"
cp "/run/build/resources/linux/$APP_ID.desktop.in" $DESKTOP_FILE
sed -i "s|@LIB_DIR@|$LIB_DIR|g" $DESKTOP_FILE
sed -i "s|@OUTPUT_NAME@|$PROJECT|g" $DESKTOP_FILE

# Create executable launcher script
LAUNCHER_FILE="$BIN_DIR/$APP_ID"
cp "/run/build/resources/linux/$APP_ID.in" $LAUNCHER_FILE
sed -i "s|@LIB_DIR@|$LIB_DIR|g" $LAUNCHER_FILE
sed -i "s|@OUTPUT_NAME@|$PROJECT|g" $LAUNCHER_FILE
chmod +x $LAUNCHER_FILE

# Copy metadata file
METADATA_FILE="$DATA_DIR/metainfo/$APP_ID.metainfo.xml"
mkdir -p "$(dirname $METADATA_FILE)"
cp "/run/build/resources/linux/$APP_ID.metainfo.xml" $METADATA_FILE

# Copy icons
SCALABLE_ICON_DIR="$DATA_DIR/icons/hicolor/scalable/apps"
SYMBOLIC_ICON_DIR="$DATA_DIR/icons/hicolor/symbolic/apps"
mkdir -p $SCALABLE_ICON_DIR
mkdir -p $SYMBOLIC_ICON_DIR
cp "/run/build/resources/$APP_ID.svg" "$SCALABLE_ICON_DIR/$APP_ID.svg"
cp "/run/build/resources/$APP_ID-devel.svg" "$SCALABLE_ICON_DIR/$APP_ID-devel.svg"
cp "/run/build/resources/$APP_ID-symbolic.svg" "$SYMBOLIC_ICON_DIR/$APP_ID-symbolic.svg"

# Update gtk icon cache
gtk-update-icon-cache

# Update desktop database
update-desktop-database

echo "$APP_ID has been published and installed successfully."
