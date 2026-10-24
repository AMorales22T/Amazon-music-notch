#!/usr/bin/env bash
set -euo pipefail

echo "[1/2] Generando código MOC de Qt6..."
/usr/lib/qt6/moc main.cpp -o main.moc

echo "[2/2] Compilando amazon-music-notch con optimización -O3..."
g++ -O3 -std=c++17 -fPIC main.cpp -o amazon-music-notch \
    $(pkg-config --cflags --libs Qt6Widgets Qt6Gui Qt6Core Qt6DBus Qt6Network Qt6WebEngineWidgets Qt6WebEngineCore) \
    -I/usr/include/KF6/KGlobalAccel -lLayerShellQtInterface -lKF6GlobalAccel

echo "Compilación completada: ./amazon-music-notch"

if [[ "${1:-}" == "--install" ]]; then
    INSTALL_DIR="${HOME}/.cache/amazon-music-notch"
    mkdir -p "${INSTALL_DIR}"
    cp -f amazon-music-notch main.cpp icon.svg "${INSTALL_DIR}/"

    # Instalar iconos en tema hicolor de KDE/Freedesktop
    mkdir -p "${HOME}/.local/share/icons/hicolor/scalable/apps" "${HOME}/.local/share/icons/hicolor/256x256/apps"
    cp -f icon.svg "${HOME}/.local/share/icons/hicolor/scalable/apps/amazon-music-notch.svg"
    if command -v rsvg-convert >/dev/null 2>&1; then
        rsvg-convert -w 256 -h 256 icon.svg -o "${HOME}/.local/share/icons/hicolor/256x256/apps/amazon-music-notch.png"
    fi
    kbuildsycoca6 --noincremental >/dev/null 2>&1 || true

    # Configurar Widevine symlinks si existe en Brave
    BRAVE_WIDEVINE="${HOME}/.config/BraveSoftware/Brave-Browser/WidevineCdm"
    if [[ -d "${BRAVE_WIDEVINE}" ]]; then
        mkdir -p "${HOME}/.config/google-chrome" "${HOME}/.config/chromium"
        ln -sfn "${BRAVE_WIDEVINE}" "${HOME}/.config/google-chrome/WidevineCdm"
        ln -sfn "${BRAVE_WIDEVINE}" "${HOME}/.config/chromium/WidevineCdm"
    fi

    echo "Instalado en ${INSTALL_DIR}/amazon-music-notch"
fi
