#!/usr/bin/env bash
set -euo pipefail

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
echo -e "${BOLD}${BLUE} Nickvision macOS App Package Script${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

APP_ID="org.nickvision.application"
APP_NAME="Application"
APP_EXECUTABLE="app"
MINIMUM_MACOS_VERSION="14.0"
COPYRIGHT="Copyright © 2021-2026 Nickvision. All rights reserved."
ICON_PNG="${REPO_ROOT}/resources/${APP_ID}.png"

PROFILE="${1:-release}"
if [[ "${PROFILE}" != "debug" && "${PROFILE}" != "release" ]]; then
    error "Usage: $0 [debug|release]"
fi

echo -e "${BOLD}${BLUE}==============================================================${RESET}"
echo -e "${BOLD}${BLUE} Building ${APP_ID}${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"
cd "${REPO_ROOT}"
info "Building application..."
if [[ "${PROFILE}" == "release" ]]; then
    cargo build --release
else
    cargo build
fi
success "Built application."

TARGET_DIR="${REPO_ROOT}/target/${PROFILE}"
APP_BUNDLE="${TARGET_DIR}/${APP_NAME}.app"
CONTENTS_DIR="${APP_BUNDLE}/Contents"
MACOS_DIR="${CONTENTS_DIR}/MacOS"
RESOURCES_DIR="${CONTENTS_DIR}/Resources"
ICONSET_DIR="${TARGET_DIR}/AppIcon.iconset"
ICNS_FILE="${TARGET_DIR}/${APP_ID}.icns"
APP_BINARY="${TARGET_DIR}/${APP_EXECUTABLE}"

if [[ ! -f "${APP_BINARY}" ]]; then
    error "Expected executable not found: ${APP_BINARY}"
fi

echo -e "${BOLD}${BLUE}==============================================================${RESET}"
echo -e "${BOLD}${BLUE} Packaging ${APP_NAME}.app${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"

info "Creating app bundle directories..."
rm -rf "${APP_BUNDLE}" "${ICONSET_DIR}"
mkdir -p "${MACOS_DIR}" "${RESOURCES_DIR}" "${ICONSET_DIR}"
success "Created app bundle directories."

info "Generating app icon..."
if [[ -f "${ICON_PNG}" ]]; then
    sips -z 16 16 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_16x16.png" >/dev/null
    sips -z 32 32 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_16x16@2x.png" >/dev/null
    sips -z 32 32 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_32x32.png" >/dev/null
    sips -z 64 64 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_32x32@2x.png" >/dev/null
    sips -z 128 128 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_128x128.png" >/dev/null
    sips -z 256 256 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_128x128@2x.png" >/dev/null
    sips -z 256 256 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_256x256.png" >/dev/null
    sips -z 512 512 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_256x256@2x.png" >/dev/null
    sips -z 512 512 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_512x512.png" >/dev/null
    sips -z 1024 1024 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_512x512@2x.png" >/dev/null
    iconutil -c icns "${ICONSET_DIR}" -o "${ICNS_FILE}"
    cp "${ICNS_FILE}" "${RESOURCES_DIR}/${APP_ID}.icns"
    ICON_ENTRY=$'    <key>CFBundleIconFile</key>\n    <string>'"${APP_ID}"$'.icns</string>'
    success "Generated app icon."
else
    warn "Icon not found at ${ICON_PNG}, skipping icon generation."
    ICON_ENTRY=""
fi

info "Copying application binary..."
cp "${APP_BINARY}" "${MACOS_DIR}/${APP_EXECUTABLE}"
chmod +x "${MACOS_DIR}/${APP_EXECUTABLE}"
success "Copied application binary."

info "Copying dynamic libraries..."
while IFS= read -r dep; do
    cp "${dep}" "${MACOS_DIR}/$(basename "${dep}")"
done < <(find "${TARGET_DIR}" -maxdepth 1 -type f \( -name "*.dylib" -o -name "*.so" \))
success "Copied dynamic libraries."

info "Copying translations..."
while IFS= read -r -d '' lc_messages_dir; do
    LANGUAGE="$(basename "$(dirname "${lc_messages_dir}")")"
    mkdir -p "${MACOS_DIR}/${LANGUAGE}/LC_MESSAGES"
    cp "${lc_messages_dir}"/*.mo "${MACOS_DIR}/${LANGUAGE}/LC_MESSAGES/"
done < <(find "${TARGET_DIR}" -mindepth 2 -maxdepth 2 -type d -name "LC_MESSAGES" -print0)
success "Copied translations."

info "Writing Info.plist..."
cat > "${CONTENTS_DIR}/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>${APP_EXECUTABLE}</string>
    <key>CFBundleIdentifier</key>
    <string>${APP_ID}</string>
    <key>CFBundleName</key>
    <string>${APP_NAME}</string>
    <key>CFBundleVersion</key>
    <string>$(grep '^version = ' app/Cargo.toml | cut -d'"' -f2)</string>
    <key>CFBundleShortVersionString</key>
    <string>$(grep '^version = ' app/Cargo.toml | cut -d'"' -f2)</string>
${ICON_ENTRY}
    <key>NSHighResolutionCapable</key>
    <true/>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>LSMinimumSystemVersion</key>
    <string>${MINIMUM_MACOS_VERSION}</string>
    <key>NSHumanReadableCopyright</key>
    <string>${COPYRIGHT}</string>
</dict>
</plist>
EOF
success "Wrote Info.plist."

rm -rf "${ICONSET_DIR}" "${ICNS_FILE}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"
echo -e "${BOLD}${GREEN}✔ Created ${APP_BUNDLE} Successfully!${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"
