#!/usr/bin/env bash

RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
BLUE="\033[0;34m"
CYAN="\033[0;36m"
BOLD="\033[1m"
RESET="\033[0m"

info()    { echo -e "${CYAN}==>${RESET} $1"; }
success() { echo -e "${GREEN}✔${RESET} $1"; }
warn()    { echo -e "${YELLOW}⚠${RESET} $1"; }
error()   { echo -e "${RED}✘${RESET} $1"; exit 1; }

echo -e "${BOLD}${BLUE}==============================================================${RESET}"
echo -e "${BOLD}${BLUE} Nickvision Linux publish-and-install Script${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"

CURRENT_PWD=$(pwd)
set -euo pipefail
if [[ $# -lt 2 ]]; then
    echo "Usage: $0 prefix runtime"
    exit 1
fi

info "Changing to script directory..."
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR"
success "Changed to script directory: $SCRIPT_DIR"

info "Loading variables..."
APP_ID="org.nickvision.application"
OUTPUT_NAME="application"
PREFIX="$1"
RUNTIME="$2"
BIN_DIR="$PREFIX/bin"
LIB_DIR="$PREFIX/lib/$APP_ID"
DATA_DIR="$PREFIX/share"
ROOT_DIR="$SCRIPT_DIR/../.."
info "Bin directory: $BIN_DIR"
info "Lib directory: $LIB_DIR"
info "Data directory: $DATA_DIR"
success "Loaded variables."

echo -e "${BOLD}${BLUE}==============================================================${RESET}"
echo -e "${BOLD}${BLUE} Publishing and Installing $APP_ID ($RUNTIME)${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"

info "Creating directories..."
mkdir -p "$BIN_DIR" "$LIB_DIR" "$DATA_DIR"
success "Created directories."

info "Installing npm dependencies..."
cd "$ROOT_DIR"
bash flatpak-node/setup_sdk_node_headers.sh
npm install --offline
success "Installed npm dependencies."

info "Building application..."
npm run tauri build -- --no-bundle
success "Built application."

info "Installing binary..."
cp "$ROOT_DIR/src-tauri/target/release/$OUTPUT_NAME" "$LIB_DIR/$OUTPUT_NAME"
success "Installed binary to $LIB_DIR/$OUTPUT_NAME."

cd "$SCRIPT_DIR"

info "Creating desktop file..."
DESKTOP_FILE="$DATA_DIR/applications/$APP_ID.desktop"
mkdir -p "$(dirname "$DESKTOP_FILE")"
cp "$APP_ID.desktop.in" "$DESKTOP_FILE"
sed -i "s|@APP_ID@|$APP_ID|g" "$DESKTOP_FILE"
sed -i "s|@LIB_DIR@|$LIB_DIR|g" "$DESKTOP_FILE"
sed -i "s|@OUTPUT_NAME@|$OUTPUT_NAME|g" "$DESKTOP_FILE"
success "Created desktop file at $DESKTOP_FILE."

info "Creating launcher script..."
LAUNCHER_FILE="$BIN_DIR/$APP_ID"
cp "$APP_ID.in" "$LAUNCHER_FILE"
sed -i "s|@LIB_DIR@|$LIB_DIR|g" "$LAUNCHER_FILE"
sed -i "s|@OUTPUT_NAME@|$OUTPUT_NAME|g" "$LAUNCHER_FILE"
chmod +x "$LAUNCHER_FILE"
success "Created launcher script at $LAUNCHER_FILE."

info "Copying metadata file..."
METADATA_FILE="$DATA_DIR/metainfo/$APP_ID.metainfo.xml"
mkdir -p "$(dirname "$METADATA_FILE")"
cp "$ROOT_DIR/build-aux/$APP_ID.metainfo.xml" "$METADATA_FILE"
success "Copied metadata file to $METADATA_FILE."

info "Copying icons..."
SCALABLE_ICON_DIR="$DATA_DIR/icons/hicolor/scalable/apps"
SYMBOLIC_ICON_DIR="$DATA_DIR/icons/hicolor/symbolic/apps"
mkdir -p "$SCALABLE_ICON_DIR" "$SYMBOLIC_ICON_DIR"
cp "$ROOT_DIR/resources/$APP_ID.svg" "$SCALABLE_ICON_DIR/$APP_ID.svg"
cp "$ROOT_DIR/resources/$APP_ID-devel.svg" "$SCALABLE_ICON_DIR/$APP_ID-devel.svg"
cp "$ROOT_DIR/resources/$APP_ID-symbolic.svg" "$SYMBOLIC_ICON_DIR/$APP_ID-symbolic.svg"
success "Copied icons."

info "Installing locale files..."
LOCALE_SRC="$ROOT_DIR/src-tauri/target/locale"
if [[ -d "$LOCALE_SRC" ]]; then
    APPDIR_LOCALE_DIR="$PREFIX/usr/lib/$OUTPUT_NAME/locale"
    mkdir -p "$APPDIR_LOCALE_DIR" "$DATA_DIR/locale"
    cp -a "$LOCALE_SRC/." "$APPDIR_LOCALE_DIR/"
    for lang_dir in "$LOCALE_SRC"/*/; do
        lang="$(basename "$lang_dir")"
        mkdir -p "$DATA_DIR/locale/$lang/LC_MESSAGES"
        cp "$lang_dir/LC_MESSAGES/$OUTPUT_NAME.mo" "$DATA_DIR/locale/$lang/LC_MESSAGES/$OUTPUT_NAME.mo"
    done
    success "Installed locale files to $APPDIR_LOCALE_DIR and $DATA_DIR/locale."
else
    warn "No locale directory found at $LOCALE_SRC, skipping."
fi

info "Updating GTK icon cache..."
gtk-update-icon-cache || true
success "Updated GTK icon cache."

info "Updating desktop database..."
update-desktop-database || true
success "Updated desktop database."

info "Restoring previous working directory..."
cd "$CURRENT_PWD"
success "Restored working directory to $CURRENT_PWD."

echo -e "${BOLD}${BLUE}==============================================================${RESET}"
echo -e "${BOLD}${GREEN}✔ Published and Installed $APP_ID Successfully!${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"
