```text
   ___                                   __  ___           _     
  / _ | __ _  ___ ____ ___  ___         /  |/  /__ __ ___ (_)____
 / __ |/  ' \/ _ `/_ // _ \/ _ \       / /|_/ // // /(_-</ // __/
/_/ |_/_/_/_/\_,_//__/\___/_//_/      /_/  /_/ \_,_//___/_/ \__/ 
   ____ __ __ _  __ ___   __  ___ ____ ____   ____ ____ __   ___   _  __ ___ 
  / _  \\ \/ /| |/ // _ | /  |/  //  _// __/  /  _// __// /  / _ | / |/ // _ \
 / // / \  / /    // __ |/ /|_/ /_/ / / /__  _/ / _\ \ / /__/ __ |/    // // /
/____/  /_/ /_/|_//_/ |_/_/  /_//___/ \___/ /___//___//____/_/ |_/_/|_/____/ 
```

# Amazon Music Dynamic Island (Linux / KDE Wayland)

Una **Dynamic Island** minimalista estilo Apple para **Amazon Music (Unlimited / Free)** en Linux (Wayland / KDE Plasma), escrita de forma nativa en **C++17** con **Qt6**, **`LayerShellQt`** y **`KGlobalAccel`**.

---

## Vista Previa (ASCII UI)

### 1. Modo Píldora Compacta (`216 x 26 px` — Consumo ~20 MB RAM)

```text
      .---------------------------------------------------.
     (  [::]  Cancion Actual · Artista              ||||   )
      '---------------------------------------------------'
       ^Portada                                     ^Ecualizador 14 FPS
```

### 2. Modo Isla Expandida (`584 x 480 px` — Reproductor + SonicRadar Mix)

```text
  .-------------------------------------------------------------------------.
  | [::]  Titulo de la Cancion     [x] [|<] [||] [>|] [@] [~] [*] [L] [K] [^] [X] |
  |       Artista — Album           1   2    3    4    5   6   7   8   9  10  11  |
  |-------------------------------------------------------------------------|
  |  SonicRadar Mix · Basado en Artista (1/20)     [Nueva mezcla] [Ocultar] |
  |  > 01. Pista Recomendada Uno — Artista Afin A                           |
  |    02. Pista Recomendada Dos — Artista Afin B                           |
  |    03. Pista Recomendada Tres — Artista Afin C                          |
  |-------------------------------------------------------------------------|
  |                                                                         |
  |                     AMAZON MUSIC WEB PLAYER (DRM)                       |
  |                       [ QtWebEngine + Widevine ]                        |
  |                                                                         |
  |   [Buscar...]                                                           |
  |   +---------+  +---------+  +---------+  +---------+                    |
  |   | Album 1 |  | Album 2 |  | Album 3 |  | Album 4 |                    |
  |   +---------+  +---------+  +---------+  +---------+                    |
  |                                                                         |
  '-------------------------------------------------------------------------'

  Leyenda de controles vectoriales (100% QPainter, cero imagenes/emojis):
  [1] Shuffle (Aleatorio)       [5] Repeat (Repeticion)    [9]  Grabador de atajo
  [2] Pista anterior            [6] FreshFlow (Filtro)     [10] Minimizar a pildora
  [3] Play / Pause              [7] SonicRadar (Mix 20)    [11] Cerrar aplicacion
  [4] Siguiente pista           [8] Pin (Fijar/Mover)
```

---

## Arquitectura del Motor de Recomendacion (`SonicRadar` + `FreshFlow`)

```text
  +-----------------------+       MPRIS2 / JS DOM       +-----------------------+
  |  Amazon Music Player  | --------------------------> |   DynamicNotch Core   |
  |    (QtWebEngine)      | <-------------------------- |    (C++17 / Qt6)      |
  +-----------------------+    Control SPA sin Reload   +-----------+-----------+
                                                                    |
                         +------------------------------------------+
                         |                                          |
                         v                                          v
          +------------------------------+           +------------------------------+
          |          FreshFlow           |           |          SonicRadar          |
          |------------------------------|           |------------------------------|
          | - Tabla Hash (< 16 KB RAM)   |           | - Toma el artista actual     |
          | - Registra pistas escuchadas |           | - Consulta afinidad real     |
          |   durante mas de 20 segundos |           |   en API publica de Deezer   |
          | - Salta automaticamente      |           | - Filtra contra FreshFlow    |
          |   canciones repetidas en las |           | - Arma Playlist de 20 pistas |
          |   ultimas 18 horas           |           |   y avanza automaticamente   |
          +------------------------------+           +------------------------------+
```

---

## Caracteristicas Principales

- **Diseno estilo Apple Dynamic Island:** Pildora compacta negra mate (`216 x 26 px`) con iconos 100% vectoriales renderizados con `QPainter`, portada del album en tiempo real, titulo/artista y ecualizador animado.
- **Controles completos de reproduccion:** Botones vectoriales nativos para **Shuffle** (Aleatorio), **Anterior**, **Play/Pause**, **Siguiente** y **Repeat** (Repeticion) sincronizados con el reproductor web y MPRIS2.
- **FreshFlow (Filtro de frescura inteligente):** Evita que el algoritmo de Amazon Music repita las mismas canciones una y otra vez. Registra pistas escuchadas en una tabla hash ultraligera (`< 16 KB` de RAM, maximo 300 entradas en `~/.local/share/amazon-music-notch/freshflow.json`) y salta automaticamente cualquier pista que ya haya sonado en las ultimas 18 horas.
- **SonicRadar (Generador de Playlist de Descubrimiento):** Con un clic, toma el artista actual o tus artistas escuchados recientemente, consulta afinidad real de oyentes mediante la API publica de Deezer, descarta canciones ya escuchadas en **FreshFlow** y genera una **playlist variada de 20 canciones (`SonicRadar Mix`)** que se reproduce de forma continua controlando el buscador interno (SPA) de Amazon Music sin recargar la pagina.
- **Reproductor Amazon Music integrado:** Incluye el reproductor oficial de Amazon Music dentro del propio notch con soporte para **Widevine DRM** y modo escritorio forzado (sin bloqueos de *"Escuchar en la app"*).
- **Ultra optimizado (Bajo consumo de RAM y CPU):**
  - **Lazy-loading:** El motor web solo se inicializa cuando expandes el notch por primera vez; en modo pildora inicial consume ~20 MB de RAM y lee el estado directamente por **MPRIS2 D-Bus**.
  - **Sin re-alocacion de texturas en Wayland:** Transicion instantanea sin redimensionar el buffer de `QtWebEngine` fotograma a fotograma.
  - **Limite de memoria JS y procesos:** Configurado con `--renderer-process-limit=1` para mantener el consumo de recursos al minimo.
- **Posicion libre o fija:** Boton de candado integrado para fijar la isla o arrastrarla libremente a cualquier punto de la pantalla.
- **Soporte para tecla Copilot (`F23` / `Meta+Shift+F23`) y grabador de atajos integrado.**

---

## Requisitos (Arch Linux)

```bash
sudo pacman -S qt6-base qt6-declarative qt6-webengine layer-shell-qt kglobalaccel gcc pkgconf
```

*(Para reproduccion DRM de Amazon Music Unlimited dentro del notch, detecta automaticamente `WidevineCdm` de Brave, Chrome o Chromium).*

---

## Compilacion e Instalacion

```bash
chmod +x build.sh
./build.sh --install
```

Para ejecutarlo o alternar su despliegue desde terminal o atajo de teclado:

```bash
~/.cache/amazon-music-notch/amazon-music-notch          # Inicia el notch
~/.cache/amazon-music-notch/amazon-music-notch --toggle # Abre/cierra la isla
```

La sesion de Amazon (cookies), los ajustes y el historial de FreshFlow viven en `~/.local/share/amazon-music-notch/` (se migran solos desde `~/.cache` la primera vez que arranca).
