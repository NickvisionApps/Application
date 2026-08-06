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
echo -e "${BOLD}${BLUE} Nickvision Linux Install Script${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"

if [[ $# -lt 1 ]]; then
    error "Usage: $0 <prefix> [debug|release]"
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

APP_ID="org.nickvision.application"
APP_EXECUTABLE="app"
PREFIX="$1"
PROFILE="${2:-release}"
if [[ "${PROFILE}" != "debug" && "${PROFILE}" != "release" ]]; then
    error "Usage: $0 <prefix> [debug|release]"
fi
BIN_DIR="${PREFIX}/bin"
LIB_DIR="${PREFIX}/lib/${APP_ID}"
DATA_DIR="${PREFIX}/share"
info "Prefix: ${PREFIX}"
info "Bin directory: ${BIN_DIR}"
info "Lib directory: ${LIB_DIR}"
info "Data directory: ${DATA_DIR}"

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
APP_BINARY="${TARGET_DIR}/${APP_EXECUTABLE}"
if [[ ! -f "${APP_BINARY}" ]]; then
    error "Expected executable not found: ${APP_BINARY}"
fi

echo -e "${BOLD}${BLUE}==============================================================${RESET}"
echo -e "${BOLD}${BLUE} Installing ${APP_ID} to ${PREFIX}${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"

info "Creating directories..."
mkdir -p "${BIN_DIR}" "${LIB_DIR}" "${DATA_DIR}/applications" "${DATA_DIR}/metainfo"
mkdir -p "${DATA_DIR}/icons/hicolor/scalable/apps" "${DATA_DIR}/icons/hicolor/symbolic/apps"
success "Created directories."

info "Installing application binary..."
cp "${APP_BINARY}" "${LIB_DIR}/${APP_EXECUTABLE}"
chmod +x "${LIB_DIR}/${APP_EXECUTABLE}"
success "Installed application binary to ${LIB_DIR}."

info "Installing translations..."
while IFS= read -r -d '' lc_messages_dir; do
    LANGUAGE="$(basename "$(dirname "${lc_messages_dir}")")"
    mkdir -p "${LIB_DIR}/${LANGUAGE}/LC_MESSAGES"
    cp "${lc_messages_dir}"/*.mo "${LIB_DIR}/${LANGUAGE}/LC_MESSAGES/"
done < <(find "${TARGET_DIR}" -mindepth 2 -maxdepth 2 -type d -name "LC_MESSAGES" -print0)
success "Installed translations."

info "Installing launcher script..."
sed -e "s|@LIB_DIR@|${LIB_DIR}|g" -e "s|@OUTPUT_NAME@|${APP_EXECUTABLE}|g" \
    "${SCRIPT_DIR}/${APP_ID}.in" > "${BIN_DIR}/${APP_ID}"
chmod +x "${BIN_DIR}/${APP_ID}"
success "Installed launcher script to ${BIN_DIR}/${APP_ID}."

info "Installing desktop file..."
sed -e "s|@LIB_DIR@|${LIB_DIR}|g" -e "s|@OUTPUT_NAME@|${APP_EXECUTABLE}|g" -e "s|@APP_ID@|${APP_ID}|g" \
    "${SCRIPT_DIR}/${APP_ID}.desktop.in" > "${DATA_DIR}/applications/${APP_ID}.desktop"
success "Installed desktop file."

info "Installing metadata file..."
cp "${SCRIPT_DIR}/${APP_ID}.metainfo.xml" "${DATA_DIR}/metainfo/${APP_ID}.metainfo.xml"
success "Installed metadata file."

info "Installing icons..."
cp "${REPO_ROOT}/resources/${APP_ID}.svg" "${DATA_DIR}/icons/hicolor/scalable/apps/${APP_ID}.svg"
cp "${REPO_ROOT}/resources/${APP_ID}-devel.svg" "${DATA_DIR}/icons/hicolor/scalable/apps/${APP_ID}-devel.svg"
cp "${REPO_ROOT}/resources/${APP_ID}-symbolic.svg" "${DATA_DIR}/icons/hicolor/symbolic/apps/${APP_ID}-symbolic.svg"
success "Installed icons."

info "Updating GTK icon cache..."
gtk-update-icon-cache "${DATA_DIR}/icons/hicolor" || warn "Failed to update GTK icon cache."
success "Updated GTK icon cache."

info "Updating desktop database..."
update-desktop-database "${DATA_DIR}/applications" || warn "Failed to update desktop database."
success "Updated desktop database."

echo -e "${BOLD}${BLUE}==============================================================${RESET}"
echo -e "${BOLD}${GREEN}✔ Installed ${APP_ID} Successfully!${RESET}"
echo -e "${BOLD}${BLUE}==============================================================${RESET}"
