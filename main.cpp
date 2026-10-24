#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusArgument>
#include <QLocalServer>
#include <QLocalSocket>
#include <QAction>
#include <QKeySequence>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QUrl>
#include <QPixmap>
#include <QIcon>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QDateTime>
#include <QRandomGenerator>
#include <QHash>
#include <QSet>
#include <QScreen>
#include <QWindow>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QVersionNumber>
#include <QFile>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QtMath>

#include <LayerShellQt/Window>
#include <KGlobalAccel>

class VectorIconButton : public QPushButton {
    Q_OBJECT
public:
    enum IconType {
        Shuffle,
        Prev,
        Play,
        Pause,
        Next,
        Repeat,
        FreshFlow,
        SonicRadar,
        Pin,
        RecordKey,
        Minimize,
        CloseApp
    };

    explicit VectorIconButton(IconType type, int size = 26, QWidget *parent = nullptr)
        : QPushButton(parent), m_type(type) {
        setFixedSize(size, size);
        setCursor(Qt::PointingHandCursor);
        setFocusPolicy(Qt::NoFocus);
        setAttribute(Qt::WA_Hover, true);
    }

    void setIconType(IconType t) {
        if (m_type != t) {
            m_type = t;
            update();
        }
    }

    void setActiveState(bool active) {
        if (m_active != active) {
            m_active = active;
            update();
        }
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);

        bool hovered = underMouse();
        bool pressed = isDown();

        if (m_type == CloseApp) {
            QColor bg = pressed ? QColor(255, 59, 48, 220)
                                : (hovered ? QColor(255, 69, 58, 190) : QColor(255, 69, 58, 0));
            p.setPen(Qt::NoPen);
            p.setBrush(bg);
            p.drawEllipse(rect().adjusted(1, 1, -1, -1));
        } else if (m_active) {
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(255, 255, 255, pressed ? 60 : 42));
            p.drawEllipse(rect().adjusted(1, 1, -1, -1));
        } else if (hovered || pressed) {
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(255, 255, 255, pressed ? 45 : 26));
            p.drawEllipse(rect().adjusted(1, 1, -1, -1));
        }

        QColor fg = m_active ? QColor(255, 255, 255, 255)
                             : (hovered ? QColor(255, 255, 255, 250) : QColor(255, 255, 255, 195));
        p.setPen(QPen(fg, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.setBrush(fg);

        qreal cx = width() / 2.0;
        qreal cy = height() / 2.0;

        switch (m_type) {
        case Shuffle: {
            p.setBrush(Qt::NoBrush);
            QColor col = m_active ? QColor(0x00, 0xD2, 0xD3) : fg;
            p.setPen(QPen(col, 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            QPainterPath p1;
            p1.moveTo(cx - 4.4, cy - 2.8);
            p1.cubicTo(cx - 1.0, cy - 2.8, cx - 0.5, cy + 2.8, cx + 4.0, cy + 2.8);
            p.drawPath(p1);
            QPainterPath p2;
            p2.moveTo(cx - 4.4, cy + 2.8);
            p2.cubicTo(cx - 1.0, cy + 2.8, cx - 0.5, cy - 2.8, cx + 4.0, cy - 2.8);
            p.drawPath(p2);
            // Arrowheads on right
            p.drawLine(QPointF(cx + 2.3, cy - 4.3), QPointF(cx + 4.3, cy - 2.8));
            p.drawLine(QPointF(cx + 2.3, cy - 1.3), QPointF(cx + 4.3, cy - 2.8));
            p.drawLine(QPointF(cx + 2.3, cy + 1.3), QPointF(cx + 4.3, cy + 2.8));
            p.drawLine(QPointF(cx + 2.3, cy + 4.3), QPointF(cx + 4.3, cy + 2.8));
            break;
        }
        case Repeat: {
            p.setBrush(Qt::NoBrush);
            QColor col = m_active ? QColor(0x00, 0xD2, 0xD3) : fg;
            p.setPen(QPen(col, 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            QPainterPath topArc;
            topArc.moveTo(cx - 4.2, cy + 0.6);
            topArc.lineTo(cx - 4.2, cy - 1.4);
            topArc.quadTo(cx - 4.2, cy - 3.2, cx - 2.4, cy - 3.2);
            topArc.lineTo(cx + 3.8, cy - 3.2);
            p.drawPath(topArc);
            p.drawLine(QPointF(cx + 2.1, cy - 4.7), QPointF(cx + 4.1, cy - 3.2));
            p.drawLine(QPointF(cx + 2.1, cy - 1.7), QPointF(cx + 4.1, cy - 3.2));

            QPainterPath botArc;
            botArc.moveTo(cx + 4.2, cy - 0.6);
            botArc.lineTo(cx + 4.2, cy + 1.4);
            botArc.quadTo(cx + 4.2, cy + 3.2, cx + 2.4, cy + 3.2);
            botArc.lineTo(cx - 3.8, cy + 3.2);
            p.drawPath(botArc);
            p.drawLine(QPointF(cx - 2.1, cy + 1.7), QPointF(cx - 4.1, cy + 3.2));
            p.drawLine(QPointF(cx - 2.1, cy + 4.7), QPointF(cx - 4.1, cy + 3.2));
            break;
        }
        case FreshFlow: {
            // Sleek wave/pulse filter icon: green when FreshFlow protection is active
            p.setBrush(Qt::NoBrush);
            QColor col = m_active ? QColor(0x30, 0xD1, 0x58) : fg;
            p.setPen(QPen(col, 1.45, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            QPainterPath pulse;
            pulse.moveTo(cx - 4.8, cy);
            pulse.lineTo(cx - 2.4, cy);
            pulse.lineTo(cx - 1.1, cy - 3.8);
            pulse.lineTo(cx + 1.1, cy + 3.8);
            pulse.lineTo(cx + 2.4, cy);
            pulse.lineTo(cx + 4.8, cy);
            p.drawPath(pulse);
            break;
        }
        case SonicRadar: {
            // Sleek compass/radar icon for external track discovery
            p.setBrush(Qt::NoBrush);
            QColor col = m_active ? QColor(0x0A, 0x84, 0xFF) : fg;
            p.setPen(QPen(col, 1.35, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawEllipse(QPointF(cx, cy), 4.6, 4.6);
            QPainterPath needle;
            needle.moveTo(cx + 2.4, cy - 2.4);
            needle.lineTo(cx + 0.7, cy + 0.7);
            needle.lineTo(cx - 2.4, cy + 2.4);
            needle.lineTo(cx - 0.7, cy - 0.7);
            needle.closeSubpath();
            p.setPen(Qt::NoPen);
            p.setBrush(col);
            p.drawPath(needle);
            break;
        }
        case Play: {
            QPainterPath path;
            path.moveTo(cx - 3.2, cy - 5.0);
            path.lineTo(cx + 5.0, cy);
            path.lineTo(cx - 3.2, cy + 5.0);
            path.closeSubpath();
            p.setPen(Qt::NoPen);
            p.drawPath(path);
            break;
        }
        case Pause: {
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(QRectF(cx - 4.2, cy - 4.8, 2.7, 9.6), 1.1, 1.1);
            p.drawRoundedRect(QRectF(cx + 1.5, cy - 4.8, 2.7, 9.6), 1.1, 1.1);
            break;
        }
        case Prev: {
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(QRectF(cx - 5.0, cy - 4.3, 1.7, 8.6), 0.8, 0.8);
            QPainterPath path;
            path.moveTo(cx + 4.3, cy - 4.4);
            path.lineTo(cx - 2.4, cy);
            path.lineTo(cx + 4.3, cy + 4.4);
            path.closeSubpath();
            p.drawPath(path);
            break;
        }
        case Next: {
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(QRectF(cx + 3.3, cy - 4.3, 1.7, 8.6), 0.8, 0.8);
            QPainterPath path;
            path.moveTo(cx - 4.3, cy - 4.4);
            path.lineTo(cx + 2.4, cy);
            path.lineTo(cx - 4.3, cy + 4.4);
            path.closeSubpath();
            p.drawPath(path);
            break;
        }
        case Pin: {
            p.setBrush(Qt::NoBrush);
            QColor pinCol = m_active ? QColor(255, 255, 255, 240) : QColor(10, 132, 255, 240);
            p.setPen(QPen(pinCol, 1.45, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawRoundedRect(QRectF(cx - 4.0, cy - 0.5, 8.0, 5.2), 1.5, 1.5);
            QPainterPath shackle;
            shackle.moveTo(cx - 2.4, cy - 0.5);
            shackle.lineTo(cx - 2.4, cy - 2.6);
            shackle.arcTo(QRectF(cx - 2.4, cy - 5.0, 4.8, 4.6), 180, -180);
            if (m_active) {
                shackle.lineTo(cx + 2.4, cy - 0.5);
            }
            p.drawPath(shackle);
            break;
        }
        case RecordKey: {
            p.setBrush(Qt::NoBrush);
            QColor keyCol = m_active ? QColor(0x30, 0xD1, 0x58) : fg;
            p.setPen(QPen(keyCol, 1.35, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawRoundedRect(QRectF(cx - 5.2, cy - 3.6, 10.4, 7.2), 1.6, 1.6);
            p.setPen(Qt::NoPen);
            p.setBrush(keyCol);
            p.drawEllipse(QPointF(cx - 2.3, cy - 0.9), 0.8, 0.8);
            p.drawEllipse(QPointF(cx, cy - 0.9), 0.8, 0.8);
            p.drawEllipse(QPointF(cx + 2.3, cy - 0.9), 0.8, 0.8);
            p.drawRoundedRect(QRectF(cx - 2.6, cy + 1.1, 5.2, 1.1), 0.5, 0.5);
            break;
        }
        case Minimize: {
            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(fg, 1.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawLine(QPointF(cx - 3.8, cy + 1.5), QPointF(cx, cy - 2.2));
            p.drawLine(QPointF(cx, cy - 2.2), QPointF(cx + 3.8, cy + 1.5));
            break;
        }
        case CloseApp: {
            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(fg, 1.7, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(QPointF(cx - 3.3, cy - 3.3), QPointF(cx + 3.3, cy + 3.3));
            p.drawLine(QPointF(cx + 3.3, cy - 3.3), QPointF(cx - 3.3, cy + 3.3));
            break;
        }
        }
    }

private:
    IconType m_type;
    bool m_active = false;
};

class AlbumArtWidget : public QWidget {
    Q_OBJECT
public:
    explicit AlbumArtWidget(int sz = 18, QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(sz, sz);
    }

    void setPixmap(const QPixmap &px) {
        m_scaled = QPixmap();
        if (!px.isNull()) {
            // Escala y redondea una sola vez. setClipPath no suaviza bordes; un pincel de textura si.
            const qreal dpr = devicePixelRatioF();
            const QSize target = size() * dpr;
            const QPixmap scaled = px.scaled(target, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            QPixmap baked(target);
            baked.fill(Qt::transparent);
            {
                QPainter bp(&baked);
                bp.setRenderHint(QPainter::Antialiasing, true);
                bp.setPen(Qt::NoPen);
                bp.setBrush(QBrush(scaled));
                bp.setBrushOrigin(QPointF(target.width() - scaled.width(), target.height() - scaled.height()) / 2.0);
                const qreal r = qMin(target.width(), target.height()) * 0.24;
                bp.drawRoundedRect(QRectF(QPointF(0, 0), QSizeF(target)), r, r);
            }
            baked.setDevicePixelRatio(dpr);
            m_scaled = baked;
        }
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);

        if (!m_scaled.isNull()) {
            p.drawPixmap(rect(), m_scaled);
        } else {
            QPainterPath path;
            qreal radius = qMin(width(), height()) * 0.24;
            path.addRoundedRect(rect(), radius, radius);
            p.setClipPath(path);

            // Sleek pitch-black & silver geometric Antigravity/Apple fallback mark
            QLinearGradient grad(0, 0, width(), height());
            grad.setColorAt(0.0, QColor(28, 30, 36));
            grad.setColorAt(1.0, QColor(10, 11, 14));
            p.fillRect(rect(), grad);

            qreal cx = width() / 2.0;
            qreal cy = height() / 2.0;
            qreal s = qMin(width(), height()) * 0.28;

            p.setPen(Qt::NoPen);
            p.setBrush(QColor(255, 255, 255, 220));
            QPainterPath delta;
            delta.moveTo(cx, cy - s);
            delta.lineTo(cx + s * 0.85, cy + s * 0.75);
            delta.lineTo(cx, cy + s * 0.30);
            delta.lineTo(cx - s * 0.85, cy + s * 0.75);
            delta.closeSubpath();
            p.drawPath(delta);
        }
    }

private:
    QPixmap m_scaled;
};

class EqualizerWidget : public QWidget {
    Q_OBJECT
public:
    explicit EqualizerWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(15, 13);
        m_timer = new QTimer(this);
        // 14 FPS (70ms); el timer solo corre mientras suena y el widget esta visible
        connect(m_timer, &QTimer::timeout, this, &EqualizerWidget::step);
        m_heights = {0.30f, 0.70f, 0.45f, 0.60f};
    }

    void setPlaying(bool playing) {
        if (m_playing == playing) return;
        m_playing = playing;
        if (!m_playing) {
            m_heights = {0.22f, 0.22f, 0.22f, 0.22f};
            update();
        }
        syncTimer();
    }

private slots:
    void step() {
        m_tick++;
        m_heights[0] = 0.22f + 0.65f * qAbs(qSin(m_tick * 0.25f));
        m_heights[1] = 0.25f + 0.72f * qAbs(qCos(m_tick * 0.34f));
        m_heights[2] = 0.22f + 0.65f * qAbs(qSin(m_tick * 0.41f + 1.1f));
        m_heights[3] = 0.22f + 0.52f * qAbs(qCos(m_tick * 0.29f + 2.0f));
        update();
    }

protected:
    void showEvent(QShowEvent *) override { m_shown = true; syncTimer(); }
    void hideEvent(QHideEvent *) override { m_shown = false; syncTimer(); }

    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        QColor barColor = m_playing ? QColor(255, 255, 255, 220) : QColor(255, 255, 255, 70);
        p.setPen(Qt::NoPen);
        p.setBrush(barColor);

        qreal barW = 2.2;
        qreal gap = 1.6;
        qreal totalW = 4 * barW + 3 * gap;
        qreal startX = (width() - totalW) / 2.0;

        for (int i = 0; i < 4; ++i) {
            qreal h = qMax(2.5, height() * qreal(m_heights[i]));
            qreal y = (height() - h) / 2.0;
            p.drawRoundedRect(QRectF(startX + i * (barW + gap), y, barW, h), 1.1, 1.1);
        }
    }

private:
    void syncTimer() {
        if (m_playing && m_shown) {
            if (!m_timer->isActive()) m_timer->start(70);
        } else {
            m_timer->stop();
        }
    }

    QTimer *m_timer;
    QList<float> m_heights;
    bool m_playing = false;
    bool m_shown = false;
    int m_tick = 0;
};

static QString appCacheDir() {
    QString base = QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
    if (base.isEmpty()) base = QDir::homePath() + QStringLiteral("/.cache");
    QString dir = base + QStringLiteral("/amazon-music-notch");
    QDir().mkpath(dir);
    return dir;
}

// Sesion de Amazon (cookies), ajustes e historial: un limpiador de ~/.cache no debe borrarlos.
static QString appDataDir() {
    QString base = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    if (base.isEmpty()) base = QDir::homePath() + QStringLiteral("/.local/share");
    QString dir = base + QStringLiteral("/amazon-music-notch");
    QDir().mkpath(dir);
    return dir;
}

// Mueve una sola vez el estado de versiones anteriores (que vivia en ~/.cache).
static void migrateLegacyState() {
    for (const char *name : {"config.ini", "freshflow.json", "webprofile"}) {
        const QString from = appCacheDir() + QLatin1Char('/') + QLatin1String(name);
        const QString to = appDataDir() + QLatin1Char('/') + QLatin1String(name);
        if (!QFileInfo::exists(to) && QFileInfo::exists(from)) QDir().rename(from, to);
    }
}

// Widevine mas reciente de Brave/Chrome/Chromium; la carpeta de version cambia con cada actualizacion.
static QString findWidevineLib() {
    const QString rel = QStringLiteral("/_platform_specific/linux_x64/libwidevinecdm.so");
    const QString cfg = QDir::homePath() + QStringLiteral("/.config/");
    const QStringList roots = {
        cfg + QStringLiteral("BraveSoftware/Brave-Browser/WidevineCdm"),
        cfg + QStringLiteral("google-chrome/WidevineCdm"),
        cfg + QStringLiteral("chromium/WidevineCdm")
    };
    QString best;
    QVersionNumber bestVer;
    for (const QString &root : roots) {
        const QStringList dirs = QDir(root).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &v : dirs) {
            const QVersionNumber ver = QVersionNumber::fromString(v);
            const QString lib = root + QLatin1Char('/') + v + rel;
            if (ver > bestVer && QFile::exists(lib)) { bestVer = ver; best = lib; }
        }
    }
    const QString sys = QStringLiteral("/opt/google/chrome/WidevineCdm") + rel;
    return (best.isEmpty() && QFile::exists(sys)) ? sys : best;
}

class DynamicNotch : public QWidget {
    Q_OBJECT

public:
    DynamicNotch() : m_settings(appDataDir() + QStringLiteral("/config.ini"), QSettings::IniFormat) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setMouseTracking(true);

        m_pinned = m_settings.value(QStringLiteral("pinned"), true).toBool();
        m_freshFlowEnabled = m_settings.value(QStringLiteral("freshFlow"), true).toBool();
        m_posX = m_settings.value(QStringLiteral("posX"), -1).toInt();
        m_posY = m_settings.value(QStringLiteral("posY"), 6).toInt();

        loadTrackHistory();

        m_currentPillSize = m_compactSize;
        setFixedSize(m_currentPillSize);

        m_autoCollapseTimer = new QTimer(this);
        m_autoCollapseTimer->setSingleShot(true);
        connect(m_autoCollapseTimer, &QTimer::timeout, this, [this]() {
            if (m_expanded && !underMouse() && !m_recordingKey) {
                collapse();
            }
        });

        setupUi();
        setupMprisPolling();
        setupLocalServer();
        setupGlobalShortcuts();
    }

    // Direct resize without per-frame Wayland surface reallocation!
    // In Wayland + QtWebEngine, resizing a Chromium view 30 times over 180ms causes severe GPU texture thrashing.
    // Instant transition + frozen WebView during state change eliminates 100% of lag!
    void applyPillSize(const QSize &s) {
        if (m_currentPillSize == s) return;
        int oldW = m_currentPillSize.width();
        m_currentPillSize = s;
        setFixedSize(s);
        m_posX -= (s.width() - oldW) / 2;
        applyLayerMargins();
        update();
    }

    void setupLayerShell() {
        winId();
        QScreen *scr = QGuiApplication::primaryScreen();
        int screenW = scr ? scr->geometry().width() : 1920;
        if (m_posX < 0 || m_posX > screenW - 100) {
            m_posX = (screenW - m_currentPillSize.width()) / 2;
        }
        m_posY = qBound(0, m_posY, 900);

        if (QWindow *w = windowHandle()) {
            m_layerWindow = LayerShellQt::Window::get(w);
            if (m_layerWindow) {
                m_layerWindow->setLayer(LayerShellQt::Window::LayerOverlay);
                m_layerWindow->setAnchors(LayerShellQt::Window::Anchors(
                    LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft));
                m_layerWindow->setExclusiveZone(-1);
                m_layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
                m_layerWindow->setScope(QStringLiteral("amazon-music-notch"));
                applyLayerMargins();
            }
        }
    }

    void applyLayerMargins() {
        if (!m_layerWindow) return;
        QScreen *scr = QGuiApplication::primaryScreen();
        int screenW = scr ? scr->geometry().width() : 1920;
        int screenH = scr ? scr->geometry().height() : 1080;
        int clampedX = qBound(0, m_posX, qMax(0, screenW - width()));
        int clampedY = qBound(0, m_posY, qMax(0, screenH - height()));
        m_posX = clampedX;
        m_posY = clampedY;
        m_layerWindow->setMargins(QMargins(clampedX, clampedY, 0, 0));
        if (QWindow *w = windowHandle()) {
            w->requestUpdate();
        }
        update();
    }

public slots:
    void toggleExpanded() {
        if (!isVisible()) {
            show();
            expand();
            return;
        }
        if (m_expanded) {
            collapse();
        } else {
            expand();
        }
    }

    void expand() {
        if (!isVisible()) show();
        m_autoCollapseTimer->stop();

        if (m_expanded) {
            if (QWindow *w = windowHandle()) w->requestActivate();
            activateWindow();
            if (m_webView) m_webView->setFocus();
            return;
        }
        m_expanded = true;

        m_compactContainer->hide();
        applyPillSize(m_expandedSize);
        m_expandedContainer->show();

        // Initialize WebEngine after container is visible so Wayland surface handle is valid
        ensureWebViewInitialized();
        if (m_webView) m_webView->show();

        if (m_layerWindow) {
            m_layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);
            if (QWindow *w = windowHandle()) {
                w->requestActivate();
            }
        }
        activateWindow();
        raise();

        QTimer::singleShot(50, this, [this]() {
            if (QWindow *w = windowHandle()) w->requestActivate();
            activateWindow();
            if (m_webView) m_webView->setFocus();
        });
    }

    void collapse() {
        if (!m_expanded) return;
        m_expanded = false;
        m_recordingKey = false;
        m_autoCollapseTimer->stop();
        m_keyBtn->setActiveState(false);

        if (m_layerWindow) {
            m_layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
        }

        m_expandedContainer->hide();
        applyPillSize(m_compactSize);
        m_compactContainer->show();
    }

protected:
    void enterEvent(QEnterEvent *event) override {
        m_autoCollapseTimer->stop();
        QWidget::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override {
        if (m_expanded && !m_dragging && !m_recordingKey) {
            m_autoCollapseTimer->start(450);
        }
        QWidget::leaveEvent(event);
    }

    void changeEvent(QEvent *event) override {
        if (event->type() == QEvent::ActivationChange) {
            if (!isActiveWindow() && m_expanded && !underMouse() && !m_recordingKey) {
                m_autoCollapseTimer->start(250);
            }
        }
        QWidget::changeEvent(event);
    }

    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);

        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(rect(), Qt::transparent);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);

        QRectF pillRect = QRectF(rect()).adjusted(1.0, 1.0, -1.0, -1.0);
        qreal radius = m_expanded ? 22.0 : (pillRect.height() / 2.0);

        QPainterPath path;
        path.addRoundedRect(pillRect, radius, radius);

        p.fillPath(path, QColor(0, 0, 0, 250));

        QColor rimColor = !m_pinned ? QColor(255, 255, 255, 70) : QColor(255, 255, 255, 30);
        p.setPen(QPen(rimColor, 1.0));
        p.drawPath(path);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            // In expanded mode, only start drag from the top header bar (y <= 48)
            if (m_expanded && event->position().y() > 48) {
                QWidget::mousePressEvent(event);
                return;
            }
            m_pressLocalPos = event->position().toPoint();
            m_dragging = false;
            m_mouseDown = true;
            m_lastDragStepMs = 0;
            m_autoCollapseTimer->stop();
            event->accept();
            return;
        }
        QWidget::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (m_mouseDown && !(event->buttons() & Qt::LeftButton)) {
            m_mouseDown = false;
            m_dragging = false;
            releaseMouse();
            unsetCursor();
            return;
        }
        if (m_mouseDown && (event->buttons() & Qt::LeftButton)) {
            QPoint delta = event->position().toPoint() - m_pressLocalPos;
            if (!m_pinned) {
                if (!m_dragging && delta.manhattanLength() > 4) {
                    m_dragging = true;
                    m_autoCollapseTimer->stop();
                    setCursor(Qt::ClosedHandCursor);
                    grabMouse();
                }
                if (m_dragging) {
                    // Wayland wlr-layer-shell margins are double-buffered and applied by KWin on vblank (~16ms).
                    // Waiting 22ms + forcing repaint() (wl_surface.commit) ensures KWin has moved the surface
                    // and updated wl_pointer local coordinates before we sample the next delta!
                    qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
                    if (nowMs - m_lastDragStepMs >= 22 && (qAbs(delta.x()) > 1 || qAbs(delta.y()) > 1)) {
                        m_lastDragStepMs = nowMs;
                        QScreen *scr = QGuiApplication::primaryScreen();
                        int screenW = scr ? scr->geometry().width() : 1920;
                        int screenH = scr ? scr->geometry().height() : 1080;
                        m_posX = qBound(0, m_posX + delta.x(), qMax(0, screenW - width()));
                        m_posY = qBound(0, m_posY + delta.y(), qMax(0, screenH - height()));
                        applyLayerMargins();
                        repaint();
                    }
                }
            }
            event->accept();
            return;
        }
        QWidget::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && m_mouseDown) {
            m_mouseDown = false;
            releaseMouse();
            unsetCursor();
            if (m_dragging) {
                m_dragging = false;
                m_settings.setValue(QStringLiteral("posX"), m_posX);
                m_settings.setValue(QStringLiteral("posY"), m_posY);
                m_settings.sync();
            } else {
                if (!m_expanded) {
                    expand();
                }
            }
            event->accept();
            return;
        }
        QWidget::mouseReleaseEvent(event);
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (m_recordingKey) {
            int key = event->key();
            if (key == Qt::Key_Control || key == Qt::Key_Shift || key == Qt::Key_Alt || key == Qt::Key_Meta) {
                return;
            }
            QKeySequence seq;
            if (key == Qt::Key_unknown || key == 0) {
                seq = QKeySequence(Qt::META | Qt::SHIFT | Qt::Key_F23);
            } else {
                seq = QKeySequence(event->modifiers() | key);
            }
            if (!seq.isEmpty()) {
                bindCustomShortcut(seq);
                m_recordingKey = false;
                m_keyBtn->setActiveState(false);
                m_expArtist->setText(QStringLiteral("Tecla guardada: ") + seq.toString());
            }
            event->accept();
            return;
        }

        if (event->key() == Qt::Key_Escape && m_expanded) {
            collapse();
            event->accept();
            return;
        }
        QWidget::keyPressEvent(event);
    }

private:
    void setupUi() {
        m_netManager = new QNetworkAccessManager(this);

        QVBoxLayout *holderLayout = new QVBoxLayout(this);
        holderLayout->setContentsMargins(0, 0, 0, 0);
        holderLayout->setSpacing(0);

        // --- COMPACT MODE (216 x 26 px) ---
        m_compactContainer = new QWidget(this);
        m_compactContainer->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        QHBoxLayout *compactLayout = new QHBoxLayout(m_compactContainer);
        compactLayout->setContentsMargins(8, 3, 10, 3);
        compactLayout->setSpacing(8);

        m_artCompact = new AlbumArtWidget(18, m_compactContainer);
        m_artCompact->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        m_compactTitle = new QLabel(QStringLiteral("Amazon Music"), m_compactContainer);
        m_compactTitle->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        m_compactTitle->setStyleSheet(
            "color: rgba(255, 255, 255, 0.92); font-family: 'Inter', 'Noto Sans', sans-serif; "
            "font-size: 11px; font-weight: 600; background: transparent;"
        );
        m_compactTitle->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        m_eqCompact = new EqualizerWidget(m_compactContainer);
        m_eqCompact->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        compactLayout->addWidget(m_artCompact);
        compactLayout->addWidget(m_compactTitle, 1);
        compactLayout->addWidget(m_eqCompact);

        // --- EXPANDED MODE ---
        m_expandedContainer = new QWidget(this);
        m_expandedContainer->hide();
        m_expLayout = new QVBoxLayout(m_expandedContainer);
        m_expLayout->setContentsMargins(12, 10, 12, 12);
        m_expLayout->setSpacing(8);

        // Top Bar: Track Info + Controls + Pin + Keybind + Minimize + Close (Tache)
        QHBoxLayout *topRow = new QHBoxLayout();
        topRow->setSpacing(8);

        m_artExpanded = new AlbumArtWidget(30, m_expandedContainer);
        m_artExpanded->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        QVBoxLayout *metaLayout = new QVBoxLayout();
        metaLayout->setSpacing(0);
        metaLayout->setContentsMargins(0, 0, 0, 0);

        m_expTitle = new QLabel(QStringLiteral("Amazon Music Unlimited"), m_expandedContainer);
        m_expTitle->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        m_expTitle->setStyleSheet(
            "color: #FFFFFF; font-family: 'Inter', 'Noto Sans', sans-serif; "
            "font-size: 11.5px; font-weight: 600; background: transparent;"
        );

        m_expArtist = new QLabel(QStringLiteral("España"), m_expandedContainer);
        m_expArtist->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        m_expArtist->setStyleSheet(
            "color: rgba(255, 255, 255, 0.55); font-family: 'Inter', 'Noto Sans', sans-serif; "
            "font-size: 10px; font-weight: 500; background: transparent;"
        );

        metaLayout->addWidget(m_expTitle);
        metaLayout->addWidget(m_expArtist);

        QHBoxLayout *ctrlLayout = new QHBoxLayout();
        ctrlLayout->setSpacing(2);

        m_shuffleBtn = new VectorIconButton(VectorIconButton::Shuffle, 22, m_expandedContainer);
        m_shuffleBtn->setToolTip(QStringLiteral("Modo aleatorio (Shuffle)"));

        m_prevBtn = new VectorIconButton(VectorIconButton::Prev, 23, m_expandedContainer);
        m_prevBtn->setToolTip(QStringLiteral("Pista anterior"));

        m_playBtn = new VectorIconButton(VectorIconButton::Play, 25, m_expandedContainer);
        m_playBtn->setToolTip(QStringLiteral("Reproducir / Pausar"));

        m_nextBtn = new VectorIconButton(VectorIconButton::Next, 23, m_expandedContainer);
        m_nextBtn->setToolTip(QStringLiteral("Siguiente pista"));

        m_repeatBtn = new VectorIconButton(VectorIconButton::Repeat, 22, m_expandedContainer);
        m_repeatBtn->setToolTip(QStringLiteral("Modo repetición (Repeat)"));

        m_freshFlowBtn = new VectorIconButton(VectorIconButton::FreshFlow, 23, m_expandedContainer);
        m_freshFlowBtn->setActiveState(m_freshFlowEnabled);
        m_freshFlowBtn->setToolTip(QStringLiteral("FreshFlow: salta automáticamente canciones repetidas en las últimas 18h"));

        m_sonicRadarBtn = new VectorIconButton(VectorIconButton::SonicRadar, 23, m_expandedContainer);
        m_sonicRadarBtn->setToolTip(QStringLiteral("SonicRadar: genera una playlist nueva basada en lo que estás escuchando"));

        m_pinBtn = new VectorIconButton(VectorIconButton::Pin, 22, m_expandedContainer);
        m_pinBtn->setActiveState(m_pinned);
        m_pinBtn->setToolTip(m_pinned ? QStringLiteral("Posición fija (clic para mover)")
                                      : QStringLiteral("Modo mover activo (arrastra desde esta barra superior)"));

        m_keyBtn = new VectorIconButton(VectorIconButton::RecordKey, 22, m_expandedContainer);
        m_keyBtn->setToolTip(QStringLiteral("Asignar tecla rápida"));

        m_minBtn = new VectorIconButton(VectorIconButton::Minimize, 22, m_expandedContainer);
        m_minBtn->setToolTip(QStringLiteral("Minimizar a píldora (Esc)"));

        m_closeBtn = new VectorIconButton(VectorIconButton::CloseApp, 23, m_expandedContainer);
        m_closeBtn->setToolTip(QStringLiteral("Cerrar el Notch por completo"));

        connect(m_shuffleBtn, &QPushButton::clicked, this, [this]() { triggerMediaControl(QStringLiteral("Shuffle")); });
        connect(m_prevBtn, &QPushButton::clicked, this, [this]() { triggerMediaControl(QStringLiteral("Previous")); });
        connect(m_playBtn, &QPushButton::clicked, this, [this]() { triggerMediaControl(QStringLiteral("PlayPause")); });
        connect(m_nextBtn, &QPushButton::clicked, this, [this]() { triggerMediaControl(QStringLiteral("Next")); });
        connect(m_repeatBtn, &QPushButton::clicked, this, [this]() { triggerMediaControl(QStringLiteral("Repeat")); });
        connect(m_freshFlowBtn, &QPushButton::clicked, this, [this]() {
            m_freshFlowEnabled = !m_freshFlowEnabled;
            m_freshFlowBtn->setActiveState(m_freshFlowEnabled);
            m_settings.setValue(QStringLiteral("freshFlow"), m_freshFlowEnabled);
            m_settings.sync();
            m_expArtist->setText(m_freshFlowEnabled
                ? QStringLiteral("FreshFlow activo: sin repeticiones recientes")
                : QStringLiteral("FreshFlow desactivado"));
        });
        connect(m_sonicRadarBtn, &QPushButton::clicked, this, &DynamicNotch::triggerSonicRadar);
        connect(m_pinBtn, &QPushButton::clicked, this, [this]() {
            m_pinned = !m_pinned;
            m_pinBtn->setActiveState(m_pinned);
            m_settings.setValue(QStringLiteral("pinned"), m_pinned);
            m_settings.sync();
            update();
        });
        connect(m_keyBtn, &QPushButton::clicked, this, [this]() {
            m_recordingKey = !m_recordingKey;
            m_keyBtn->setActiveState(m_recordingKey);
            if (m_recordingKey) {
                setFocus(Qt::OtherFocusReason);
                m_expArtist->setText(QStringLiteral("Pulsa la tecla que quieras asignar..."));
            } else {
                pollMpris();
            }
        });
        connect(m_minBtn, &QPushButton::clicked, this, &DynamicNotch::collapse);
        connect(m_closeBtn, &QPushButton::clicked, this, []() {
            qApp->quit();
        });

        ctrlLayout->addWidget(m_shuffleBtn);
        ctrlLayout->addWidget(m_prevBtn);
        ctrlLayout->addWidget(m_playBtn);
        ctrlLayout->addWidget(m_nextBtn);
        ctrlLayout->addWidget(m_repeatBtn);
        ctrlLayout->addSpacing(3);
        ctrlLayout->addWidget(m_freshFlowBtn);
        ctrlLayout->addWidget(m_sonicRadarBtn);
        ctrlLayout->addSpacing(3);
        ctrlLayout->addWidget(m_pinBtn);
        ctrlLayout->addWidget(m_keyBtn);
        ctrlLayout->addWidget(m_minBtn);
        ctrlLayout->addSpacing(2);
        ctrlLayout->addWidget(m_closeBtn);

        topRow->addWidget(m_artExpanded);
        topRow->addLayout(metaLayout, 1);
        topRow->addLayout(ctrlLayout);

        m_expLayout->addLayout(topRow);

        // --- SONIC RADAR PLAYLIST PANEL (Lightweight Native Drawer) ---
        m_playlistPanel = new QWidget(m_expandedContainer);
        m_playlistPanel->setFixedHeight(114);
        m_playlistPanel->setStyleSheet(
            "background: rgba(20, 22, 28, 0.94); border: 1px solid rgba(255, 255, 255, 0.09); border-radius: 10px;"
        );
        m_playlistPanel->hide();

        QVBoxLayout *plLayout = new QVBoxLayout(m_playlistPanel);
        plLayout->setContentsMargins(8, 5, 8, 5);
        plLayout->setSpacing(4);

        QHBoxLayout *plHeader = new QHBoxLayout();
        plHeader->setSpacing(6);

        m_playlistHeaderLabel = new QLabel(QStringLiteral("SonicRadar Mix"), m_playlistPanel);
        m_playlistHeaderLabel->setStyleSheet(
            "color: #0A84FF; font-family: 'Inter', 'Noto Sans', sans-serif; "
            "font-size: 10px; font-weight: 700; border: none; background: transparent;"
        );

        QString miniBtnStyle = QStringLiteral(
            "QPushButton { color: rgba(255, 255, 255, 0.82); background: rgba(255, 255, 255, 0.08); "
            "border: 1px solid rgba(255, 255, 255, 0.12); border-radius: 5px; padding: 1px 7px; "
            "font-size: 9.5px; font-weight: 600; }"
            "QPushButton:hover { background: rgba(10, 132, 255, 0.35); color: #FFFFFF; }"
        );

        QPushButton *regenBtn = new QPushButton(QStringLiteral("Nueva mezcla"), m_playlistPanel);
        regenBtn->setCursor(Qt::PointingHandCursor);
        regenBtn->setFocusPolicy(Qt::NoFocus);
        regenBtn->setStyleSheet(miniBtnStyle);
        connect(regenBtn, &QPushButton::clicked, this, [this]() { buildSonicRadarPlaylist(true); });

        QPushButton *hidePlBtn = new QPushButton(QStringLiteral("Ocultar"), m_playlistPanel);
        hidePlBtn->setCursor(Qt::PointingHandCursor);
        hidePlBtn->setFocusPolicy(Qt::NoFocus);
        hidePlBtn->setStyleSheet(miniBtnStyle);
        connect(hidePlBtn, &QPushButton::clicked, this, [this]() { m_playlistPanel->hide(); });

        plHeader->addWidget(m_playlistHeaderLabel, 1);
        plHeader->addWidget(regenBtn);
        plHeader->addWidget(hidePlBtn);

        m_playlistWidget = new QListWidget(m_playlistPanel);
        m_playlistWidget->setFocusPolicy(Qt::NoFocus);
        m_playlistWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_playlistWidget->setStyleSheet(
            "QListWidget { background: transparent; border: none; color: rgba(255, 255, 255, 0.85); "
            "font-family: 'Inter', 'Noto Sans', sans-serif; font-size: 10.5px; outline: none; }"
            "QListWidget::item { padding: 2px 4px; border-radius: 4px; }"
            "QListWidget::item:hover { background: rgba(255, 255, 255, 0.08); }"
            "QListWidget::item:selected { background: rgba(10, 132, 255, 0.28); color: #FFFFFF; font-weight: 600; }"
        );
        connect(m_playlistWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
            int idx = m_playlistWidget->row(item);
            playRadarPlaylistAtIndex(idx);
        });

        plLayout->addLayout(plHeader);
        plLayout->addWidget(m_playlistWidget, 1);

        m_expLayout->addWidget(m_playlistPanel);

        holderLayout->addWidget(m_compactContainer);
        holderLayout->addWidget(m_expandedContainer);
    }

    void ensureWebViewInitialized() {
        if (m_webView) return;

        QWebEngineProfile *profile = new QWebEngineProfile(QStringLiteral("AmazonMusicNotch"), this);
        profile->setPersistentStoragePath(appDataDir() + QStringLiteral("/webprofile"));
        profile->setCachePath(appCacheDir() + QStringLiteral("/webcache"));
        profile->setHttpCacheType(QWebEngineProfile::DiskHttpCache);
        profile->setHttpCacheMaximumSize(64 * 1024 * 1024); // Cap disk cache at 64 MB
        profile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
        profile->setHttpUserAgent(QStringLiteral("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36"));

        // Inject lightweight script:
        // 1) Force desktop viewport (1280px) so Amazon Music never shows mobile blocker
        // 2) Kill heavy CSS animations/backdrop-filters inside Amazon Music Web Player to save CPU/GPU!
        QWebEngineScript desktopFixScript;
        desktopFixScript.setName(QStringLiteral("AmazonMusicDesktopAndPerfFix"));
        desktopFixScript.setInjectionPoint(QWebEngineScript::DocumentCreation);
        desktopFixScript.setWorldId(QWebEngineScript::MainWorld);
        desktopFixScript.setRunsOnSubFrames(false);
        desktopFixScript.setSourceCode(QStringLiteral(R"(
            (function() {
                try {
                    Object.defineProperty(window, 'innerWidth', { get: function() { return 1280; }, configurable: true });
                    Object.defineProperty(window, 'outerWidth', { get: function() { return 1280; }, configurable: true });
                    Object.defineProperty(document.documentElement, 'clientWidth', { get: function() { return 1280; }, configurable: true });
                    Object.defineProperty(window.screen, 'width', { get: function() { return 1920; }, configurable: true });
                    Object.defineProperty(window.screen, 'availWidth', { get: function() { return 1920; }, configurable: true });
                    window.matchMedia = (function(orig) {
                        return function(q) {
                            if (q && (q.includes('max-width') || q.includes('pointer: coarse'))) {
                                return { matches: false, media: q, onchange: null, addListener: function(){}, removeListener: function(){}, addEventListener: function(){}, removeEventListener: function(){}, dispatchEvent: function(){ return false; } };
                            }
                            return orig ? orig.call(window, q) : { matches: true, media: q };
                        };
                    })(window.matchMedia);

                    document.addEventListener('DOMContentLoaded', function() {
                        const style = document.createElement('style');
                        style.textContent = '*, *::before, *::after { backdrop-filter: none !important; -webkit-backdrop-filter: none !important; }';
                        document.head.appendChild(style);
                    });
                } catch(e) {}
            })();
        )"));
        profile->scripts()->insert(desktopFixScript);

        m_webView = new QWebEngineView(m_expandedContainer);
        QWebEnginePage *page = new QWebEnginePage(profile, m_webView);
        m_webView->setPage(page);
        m_webView->page()->setBackgroundColor(QColor(0, 0, 0));
        m_webView->settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, false);
        m_webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
        m_webView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        m_webView->settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, false);
        m_webView->settings()->setAttribute(QWebEngineSettings::WebGLEnabled, false);
        m_webView->settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, false);
        m_webView->setUrl(QUrl(QStringLiteral("https://music.amazon.es")));
        m_webView->setZoomFactor(0.75);

        m_expLayout->addWidget(m_webView, 1);
    }

    void setupMprisPolling() {
        m_pollTimer = new QTimer(this);
        connect(m_pollTimer, &QTimer::timeout, this, &DynamicNotch::pollMpris);
        // Relaxed polling interval (1500ms) for ultra-low background CPU overhead
        m_pollTimer->start(1500);
        QTimer::singleShot(100, this, &DynamicNotch::pollMpris);
    }

    QString findActiveMprisService() {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusReply<QStringList> reply = bus.interface()->registeredServiceNames();
        if (!reply.isValid()) return QString();

        const QStringList services = reply.value();
        if (services.contains(QStringLiteral("org.mpris.MediaPlayer2.plasma-browser-integration"))) {
            return QStringLiteral("org.mpris.MediaPlayer2.plasma-browser-integration");
        }
        for (const QString &s : services) {
            if (s.startsWith(QStringLiteral("org.mpris.MediaPlayer2.")) && !s.contains(QStringLiteral("chromium"))) {
                return s;
            }
        }
        return QString();
    }

    void pollMpris() {
        if (m_webView && m_webView->page()) {
            QString js = QStringLiteral(R"(
                (function() {
                    let title = '';
                    let artist = '';
                    let album = '';
                    let art = '';
                    let playing = false;
                    let shuffle = false;
                    let repeat = false;
                    let curTime = 0;
                    let durTime = 0;

                    if (navigator.mediaSession && navigator.mediaSession.metadata) {
                        title = navigator.mediaSession.metadata.title || '';
                        artist = navigator.mediaSession.metadata.artist || '';
                        album = navigator.mediaSession.metadata.album || '';
                        if (navigator.mediaSession.metadata.artwork && navigator.mediaSession.metadata.artwork.length > 0) {
                            art = navigator.mediaSession.metadata.artwork[0].src || '';
                        }
                    }

                    const a = document.querySelector('audio, video');
                    if (a) {
                        curTime = a.currentTime || 0;
                        durTime = a.duration || 0;
                        if (!a.paused && curTime > 0) playing = true;
                    }
                    if (navigator.mediaSession && navigator.mediaSession.playbackState === 'playing') {
                        playing = true;
                    }

                    if (!title) {
                        const tEl = document.querySelector('music-horizontal-item[slot="trackInfo"], [class*="trackTitle"], [data-testid*="track-title"]');
                        if (tEl) {
                            title = tEl.getAttribute('primary-text') || tEl.textContent || '';
                            artist = tEl.getAttribute('secondary-text') || '';
                            art = tEl.getAttribute('image-src') || '';
                        }
                    }

                    const shufBtn = document.querySelector('music-button[icon-name*="shuffle"], [aria-label*="Aleatori"], [aria-label*="Shuffle"], [aria-label*="aleatori"], [aria-label*="shuffle"]');
                    if (shufBtn) {
                        const ariaP = shufBtn.getAttribute('aria-pressed');
                        const ariaL = (shufBtn.getAttribute('aria-label') || '').toLowerCase();
                        const variant = (shufBtn.getAttribute('variant') || '').toLowerCase();
                        shuffle = (ariaP === 'true' || variant === 'accent' || shufBtn.hasAttribute('active') || ariaL.includes('desactivar') || ariaL.includes('turn off'));
                    }

                    const repBtn = document.querySelector('music-button[icon-name*="repeat"], [aria-label*="Repetir"], [aria-label*="Repeat"], [aria-label*="repetir"], [aria-label*="repeat"]');
                    if (repBtn) {
                        const ariaP = repBtn.getAttribute('aria-pressed');
                        const iconN = (repBtn.getAttribute('icon-name') || '').toLowerCase();
                        const variant = (repBtn.getAttribute('variant') || '').toLowerCase();
                        repeat = (ariaP === 'true' || variant === 'accent' || repBtn.hasAttribute('active') || iconN.includes('one') || iconN.includes('all'));
                    }

                    return JSON.stringify({
                        title: title.trim(),
                        artist: artist.trim(),
                        album: album.trim(),
                        art: art.trim(),
                        playing: playing,
                        shuffle: shuffle,
                        repeat: repeat,
                        curTime: curTime,
                        durTime: durTime
                    });
                })();
            )");

            m_webView->page()->runJavaScript(js, [this](const QVariant &res) {
                QJsonDocument doc = QJsonDocument::fromJson(res.toString().toUtf8());
                QJsonObject obj = doc.object();
                QString wTitle = obj.value(QStringLiteral("title")).toString().trimmed();
                QString wArtist = obj.value(QStringLiteral("artist")).toString().trimmed();
                QString wAlbum = obj.value(QStringLiteral("album")).toString().trimmed();
                QString wArt = obj.value(QStringLiteral("art")).toString().trimmed();
                bool wPlaying = obj.value(QStringLiteral("playing")).toBool();
                bool wShuffle = obj.value(QStringLiteral("shuffle")).toBool();
                bool wRepeat = obj.value(QStringLiteral("repeat")).toBool();
                double curTime = obj.value(QStringLiteral("curTime")).toDouble();
                double durTime = obj.value(QStringLiteral("durTime")).toDouble();

                if (!wTitle.isEmpty()) {
                    m_webHasTrack = true;
                    m_webPlaying = wPlaying;
                    m_shuffleState = wShuffle;
                    m_repeatState = wRepeat;
                    if (m_shuffleBtn) m_shuffleBtn->setActiveState(m_shuffleState);
                    if (m_repeatBtn) m_repeatBtn->setActiveState(m_repeatState);

                    // Auto-advance SonicRadar Playlist when current track is within 2.5s of ending
                    qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
                    if (wPlaying && !m_radarPlaylist.isEmpty() && m_radarIndex >= 0
                        && durTime > 25.0 && (durTime - curTime) <= 2.6 && (durTime - curTime) > 0.1
                        && (nowMs - m_lastRadarLaunchMs > 8000)) {
                        int nextIdx = (m_radarIndex + 1) % m_radarPlaylist.size();
                        playRadarPlaylistAtIndex(nextIdx);
                    }

                    QString sub = (!wAlbum.isEmpty() && wAlbum != wTitle && !wArtist.isEmpty())
                        ? QStringLiteral("%1 — %2").arg(wArtist, wAlbum)
                        : (wArtist.isEmpty() ? wAlbum : wArtist);
                    updateTrackDisplay(wTitle, sub, wArtist, wArt, wPlaying);
                    return;
                }
                m_webHasTrack = false;
                pollDbusMprisFallback();
            });
            return;
        }
        pollDbusMprisFallback();
    }

    void pollDbusMprisFallback() {
        QString service = findActiveMprisService();
        if (service.isEmpty()) {
            updateTrackDisplay(QStringLiteral("Amazon Music"), QStringLiteral("España"), QString(), QString(), false);
            return;
        }
        m_activeService = service;

        QDBusMessage msg = QDBusMessage::createMethodCall(
            service,
            QStringLiteral("/org/mpris/MediaPlayer2"),
            QStringLiteral("org.freedesktop.DBus.Properties"),
            QStringLiteral("GetAll")
        );
        msg << QStringLiteral("org.mpris.MediaPlayer2.Player");

        QDBusMessage reply = QDBusConnection::sessionBus().call(msg, QDBus::Block, 200);
        if (reply.type() != QDBusMessage::ReplyMessage || reply.arguments().isEmpty()) {
            return;
        }

        QVariantMap props = qdbus_cast<QVariantMap>(reply.arguments().at(0));
        QString status = props.value(QStringLiteral("PlaybackStatus")).toString();
        bool playing = (status == QStringLiteral("Playing"));

        if (props.contains(QStringLiteral("Shuffle"))) {
            m_shuffleState = props.value(QStringLiteral("Shuffle")).toBool();
            if (m_shuffleBtn) m_shuffleBtn->setActiveState(m_shuffleState);
        }
        if (props.contains(QStringLiteral("LoopStatus"))) {
            QString loop = props.value(QStringLiteral("LoopStatus")).toString();
            m_repeatState = (!loop.isEmpty() && loop != QStringLiteral("None"));
            if (m_repeatBtn) m_repeatBtn->setActiveState(m_repeatState);
        }

        QVariantMap meta;
        const QVariant metaVar = props.value(QStringLiteral("Metadata"));
        if (metaVar.canConvert<QDBusArgument>()) {
            const QDBusArgument arg = metaVar.value<QDBusArgument>();
            arg >> meta;
        } else {
            meta = metaVar.toMap();
        }

        QString title = meta.value(QStringLiteral("xesam:title")).toString().trimmed();
        QStringList artists = meta.value(QStringLiteral("xesam:artist")).toStringList();
        QString album = meta.value(QStringLiteral("xesam:album")).toString().trimmed();
        QString rawArtist = artists.isEmpty() ? QString() : artists.first().trimmed();
        QString artist = artists.isEmpty() ? album : artists.join(QStringLiteral(", "));
        QString artUrl = meta.value(QStringLiteral("mpris:artUrl")).toString();

        if (title.isEmpty()) {
            title = QStringLiteral("Amazon Music");
            artist = QStringLiteral("España");
        }

        QString displaySubtitle = (!album.isEmpty() && album != title)
            ? QStringLiteral("%1 — %2").arg(artist, album)
            : artist;

        updateTrackDisplay(title, displaySubtitle, rawArtist, artUrl, playing);
    }

    static QString normalizeTrackKey(const QString &artist, const QString &title) {
        QString cleanTitle = title.section(QLatin1Char('('), 0, 0).section(QLatin1Char('['), 0, 0).trimmed().toLower();
        QString cleanArtist = artist.section(QLatin1Char(','), 0, 0).section(QStringLiteral(" — "), 0, 0).trimmed().toLower();
        return cleanArtist + QStringLiteral("|") + cleanTitle;
    }

    void loadTrackHistory() {
        QFile f(appDataDir() + QStringLiteral("/freshflow.json"));
        if (!f.open(QIODevice::ReadOnly)) return;
        QJsonObject root = QJsonDocument::fromJson(f.readAll()).object();
        QJsonObject hist = root.value(QStringLiteral("history")).toObject();
        qint64 now = QDateTime::currentSecsSinceEpoch();
        for (auto it = hist.begin(); it != hist.end(); ++it) {
            qint64 ts = it.value().toVariant().toLongLong();
            // Keep only tracks from last 7 days so memory stays < 16 KB
            if (now - ts < 7 * 86400 && m_trackHistory.size() < 300) {
                m_trackHistory.insert(it.key(), ts);
            }
        }
        QJsonArray seeds = root.value(QStringLiteral("seeds")).toArray();
        for (const QJsonValue &v : seeds) {
            QString s = v.toString().trimmed();
            if (!s.isEmpty() && !m_seedArtists.contains(s) && m_seedArtists.size() < 25) {
                m_seedArtists.append(s);
            }
        }
    }

    void saveTrackHistory() {
        QJsonObject hist;
        qint64 now = QDateTime::currentSecsSinceEpoch();
        for (auto it = m_trackHistory.begin(); it != m_trackHistory.end(); ++it) {
            if (now - it.value() < 7 * 86400) {
                hist.insert(it.key(), it.value());
            }
        }
        QJsonArray seeds;
        for (const QString &s : m_seedArtists) {
            seeds.append(s);
        }
        QJsonObject root;
        root.insert(QStringLiteral("history"), hist);
        root.insert(QStringLiteral("seeds"), seeds);

        QFile f(appDataDir() + QStringLiteral("/freshflow.json"));
        if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            f.write(QJsonDocument(root).toJson(QJsonDocument::Compact));
        }
    }

    void recordFinishedTrackIfValid() {
        if (m_lastNormKey.isEmpty() || m_trackStartEpoch <= 0) return;
        qint64 now = QDateTime::currentSecsSinceEpoch();
        // Record track if user listened to it for at least 20 seconds without skipping
        if (now - m_trackStartEpoch >= 20) {
            if (m_trackHistory.size() >= 300) {
                // Evict oldest entry to cap RAM usage strictly
                auto oldestIt = m_trackHistory.begin();
                for (auto it = m_trackHistory.begin(); it != m_trackHistory.end(); ++it) {
                    if (it.value() < oldestIt.value()) oldestIt = it;
                }
                if (oldestIt != m_trackHistory.end()) m_trackHistory.erase(oldestIt);
            }
            m_trackHistory.insert(m_lastNormKey, now);

            if (!m_lastRawArtist.isEmpty() && m_lastRawArtist != QStringLiteral("España")) {
                m_seedArtists.removeAll(m_lastRawArtist);
                m_seedArtists.prepend(m_lastRawArtist);
                while (m_seedArtists.size() > 25) {
                    m_seedArtists.removeLast();
                }
            }
            saveTrackHistory();
        }
    }

    void updateTrackDisplay(const QString &title, const QString &artist, const QString &rawArtist, const QString &artUrl, bool playing) {
        m_eqCompact->setPlaying(playing);
        m_playBtn->setIconType(playing ? VectorIconButton::Pause : VectorIconButton::Play);

        if (title != m_lastTitle || artist != m_lastArtist) {
            // Save previous track into FreshFlow history if it was listened to for >= 20s
            recordFinishedTrackIfValid();

            m_lastTitle = title;
            m_lastArtist = artist;
            m_lastRawArtist = rawArtist.isEmpty() ? artist.section(QStringLiteral(" — "), 0, 0).trimmed() : rawArtist;

            bool isPlaceholder = (title == QStringLiteral("Amazon Music") || title == QStringLiteral("Amazon Music Unlimited"));
            QString normKey = isPlaceholder ? QString() : normalizeTrackKey(m_lastRawArtist, title);
            m_lastNormKey = normKey;
            m_trackStartEpoch = (!normKey.isEmpty() && playing) ? QDateTime::currentSecsSinceEpoch() : 0;

            QString compactLine = (artist.isEmpty() || artist == QStringLiteral("España"))
                ? title
                : QStringLiteral("%1 · %2").arg(title, artist);

            QFontMetrics fmCompact(m_compactTitle->font());
            m_compactTitle->setText(fmCompact.elidedText(compactLine, Qt::ElideRight, 146));

            QFontMetrics fmTitle(m_expTitle->font());
            m_expTitle->setText(fmTitle.elidedText(title, Qt::ElideRight, 195));

            // FreshFlow check: if this track was already listened to in the last 18 hours, auto-skip it
            qint64 now = QDateTime::currentSecsSinceEpoch();
            if (m_freshFlowEnabled && playing && !normKey.isEmpty() && m_trackHistory.contains(normKey)) {
                qint64 lastPlayed = m_trackHistory.value(normKey);
                if (now - lastPlayed < 18 * 3600 && m_consecutiveSkips < 4) {
                    m_consecutiveSkips++;
                    m_trackStartEpoch = 0; // Do not re-record skipped track
                    if (!m_recordingKey) {
                        m_expArtist->setText(QStringLiteral("FreshFlow: saltando pista repetida..."));
                    }
                    QTimer::singleShot(350, this, [this]() {
                        triggerMediaControl(QStringLiteral("Next"));
                    });
                    return;
                }
            }
            m_consecutiveSkips = 0;

            if (!m_recordingKey && !m_radarBusy) {
                QFontMetrics fmArtist(m_expArtist->font());
                m_expArtist->setText(fmArtist.elidedText(artist, Qt::ElideRight, 195));
            }
        } else if (playing && m_trackStartEpoch == 0 && !m_lastNormKey.isEmpty()) {
            m_trackStartEpoch = QDateTime::currentSecsSinceEpoch();
        }

        if (artUrl != m_lastArtUrl) {
            m_lastArtUrl = artUrl;
            if (artUrl.startsWith(QStringLiteral("file://"))) {
                QPixmap px(QUrl(artUrl).toLocalFile());
                m_artCompact->setPixmap(px);
                m_artExpanded->setPixmap(px);
            } else if (artUrl.startsWith(QStringLiteral("http://")) || artUrl.startsWith(QStringLiteral("https://"))) {
                QNetworkReply *rep = m_netManager->get(QNetworkRequest(QUrl(artUrl)));
                connect(rep, &QNetworkReply::finished, this, [this, rep]() {
                    rep->deleteLater();
                    if (rep->error() == QNetworkReply::NoError) {
                        QPixmap px;
                        px.loadFromData(rep->readAll());
                        m_artCompact->setPixmap(px);
                        m_artExpanded->setPixmap(px);
                    }
                });
            } else {
                m_artCompact->setPixmap(QPixmap());
                m_artExpanded->setPixmap(QPixmap());
            }
        }
    }

    void triggerSonicRadar() {
        if (m_radarBusy) return;

        // If playlist already exists and panel is hidden, show panel first; otherwise generate fresh playlist
        if (!m_radarPlaylist.isEmpty() && m_playlistPanel && m_playlistPanel->isHidden()) {
            m_playlistPanel->show();
            return;
        }
        buildSonicRadarPlaylist(true);
    }

    void buildSonicRadarPlaylist(bool autoPlayFirst) {
        if (m_radarBusy) return;

        QString seed = m_lastRawArtist;
        if (seed.isEmpty() || seed == QStringLiteral("España") || seed.startsWith(QStringLiteral("Amazon"))) {
            if (!m_seedArtists.isEmpty()) {
                seed = m_seedArtists.first();
            }
        }

        if (seed.isEmpty() || seed == QStringLiteral("España")) {
            m_expArtist->setText(QStringLiteral("SonicRadar: reproduce una canción base primero"));
            return;
        }

        m_radarBusy = true;
        m_sonicRadarBtn->setActiveState(true);
        m_radarSeedName = seed;
        m_expArtist->setText(QStringLiteral("SonicRadar: armando playlist basada en ") + seed + QStringLiteral("..."));

        QUrl searchUrl(QStringLiteral("https://api.deezer.com/search/artist"));
        QUrlQuery q;
        q.addQueryItem(QStringLiteral("q"), seed);
        q.addQueryItem(QStringLiteral("limit"), QStringLiteral("1"));
        searchUrl.setQuery(q);

        QNetworkReply *rep = m_netManager->get(QNetworkRequest(searchUrl));
        connect(rep, &QNetworkReply::finished, this, [this, rep, seed, autoPlayFirst]() {
            rep->deleteLater();
            if (rep->error() != QNetworkReply::NoError) {
                m_radarBusy = false;
                m_sonicRadarBtn->setActiveState(false);
                m_expArtist->setText(QStringLiteral("SonicRadar: error de red"));
                return;
            }
            QJsonArray data = QJsonDocument::fromJson(rep->readAll()).object().value(QStringLiteral("data")).toArray();
            if (data.isEmpty()) {
                m_radarBusy = false;
                m_sonicRadarBtn->setActiveState(false);
                m_expArtist->setText(QStringLiteral("SonicRadar: sin datos para ") + seed);
                return;
            }
            int artistId = data.first().toObject().value(QStringLiteral("id")).toInt();
            fetchRelatedArtistsForPlaylist(artistId, seed, autoPlayFirst);
        });
    }

    void fetchRelatedArtistsForPlaylist(int artistId, const QString &seedName, bool autoPlayFirst) {
        QUrl relUrl(QStringLiteral("https://api.deezer.com/artist/%1/related?limit=20").arg(artistId));
        QNetworkReply *rep = m_netManager->get(QNetworkRequest(relUrl));
        connect(rep, &QNetworkReply::finished, this, [this, rep, artistId, seedName, autoPlayFirst]() {
            rep->deleteLater();
            QJsonArray related = QJsonDocument::fromJson(rep->readAll()).object().value(QStringLiteral("data")).toArray();

            // Select up to 5 related artists + the seed artist itself (for deep cuts)
            QList<QPair<int, QString>> chosenArtists;
            QList<int> indices;
            for (int i = 0; i < related.size(); ++i) indices.append(i);
            for (int i = indices.size() - 1; i > 0; --i) {
                int j = QRandomGenerator::global()->bounded(i + 1);
                indices.swapItemsAt(i, j);
            }
            for (int i = 0; i < qMin(5, int(indices.size())); ++i) {
                QJsonObject relObj = related.at(indices.at(i)).toObject();
                int rId = relObj.value(QStringLiteral("id")).toInt();
                QString rName = relObj.value(QStringLiteral("name")).toString();
                if (rId > 0 && !rName.isEmpty()) {
                    chosenArtists.append(qMakePair(rId, rName));
                }
            }
            chosenArtists.append(qMakePair(artistId, seedName));

            m_pendingBuckets.clear();
            m_pendingArtistRequests = chosenArtists.size();

            for (const auto &artPair : chosenArtists) {
                int aId = artPair.first;
                QString aName = artPair.second;
                QUrl topUrl(QStringLiteral("https://api.deezer.com/artist/%1/top?limit=12").arg(aId));
                QNetworkReply *topRep = m_netManager->get(QNetworkRequest(topUrl));
                connect(topRep, &QNetworkReply::finished, this, [this, topRep, aName, autoPlayFirst]() {
                    topRep->deleteLater();
                    QJsonArray tracks = QJsonDocument::fromJson(topRep->readAll()).object().value(QStringLiteral("data")).toArray();
                    QList<QPair<QString, QString>> bucket;
                    for (const QJsonValue &v : tracks) {
                        QJsonObject tObj = v.toObject();
                        QString tTitle = tObj.value(QStringLiteral("title_short")).toString().trimmed();
                        if (tTitle.isEmpty()) tTitle = tObj.value(QStringLiteral("title")).toString().trimmed();
                        QString tArtist = tObj.value(QStringLiteral("artist")).toObject().value(QStringLiteral("name")).toString(aName).trimmed();
                        QString key = normalizeTrackKey(tArtist, tTitle);
                        if (!tTitle.isEmpty() && !m_trackHistory.contains(key) && key != m_lastNormKey) {
                            bucket.append(qMakePair(tArtist, tTitle));
                        }
                    }
                    // Shuffle this artist's bucket
                    for (int i = bucket.size() - 1; i > 0; --i) {
                        int j = QRandomGenerator::global()->bounded(i + 1);
                        bucket.swapItemsAt(i, j);
                    }
                    if (!bucket.isEmpty()) {
                        m_pendingBuckets.append(bucket);
                    }

                    m_pendingArtistRequests--;
                    if (m_pendingArtistRequests <= 0) {
                        finalizeSonicRadarPlaylist(autoPlayFirst);
                    }
                });
            }
        });
    }

    void finalizeSonicRadarPlaylist(bool autoPlayFirst) {
        m_radarBusy = false;
        m_sonicRadarBtn->setActiveState(false);

        m_radarPlaylist.clear();
        QSet<QString> seenKeys;

        // Interleave tracks across all related artist buckets so the playlist has maximum variety
        int round = 0;
        while (m_radarPlaylist.size() < 20 && round < 12) {
            bool addedAny = false;
            for (const auto &bucket : m_pendingBuckets) {
                if (round < bucket.size()) {
                    const auto &pair = bucket.at(round);
                    QString key = normalizeTrackKey(pair.first, pair.second);
                    if (!seenKeys.contains(key)) {
                        seenKeys.insert(key);
                        m_radarPlaylist.append(pair);
                        addedAny = true;
                        if (m_radarPlaylist.size() >= 20) break;
                    }
                }
            }
            if (!addedAny) break;
            round++;
        }
        m_pendingBuckets.clear();

        if (m_radarPlaylist.isEmpty()) {
            m_expArtist->setText(QStringLiteral("SonicRadar: no se encontraron pistas nuevas"));
            return;
        }

        m_playlistWidget->clear();
        for (int i = 0; i < m_radarPlaylist.size(); ++i) {
            QString itemText = QStringLiteral("%1. %2 — %3")
                .arg(i + 1, 2, 10, QLatin1Char('0'))
                .arg(m_radarPlaylist.at(i).second, m_radarPlaylist.at(i).first);
            m_playlistWidget->addItem(itemText);
        }

        m_playlistPanel->show();
        if (autoPlayFirst) {
            playRadarPlaylistAtIndex(0);
        } else {
            updatePlaylistHeader();
        }
    }

    void updatePlaylistHeader() {
        if (!m_playlistHeaderLabel) return;
        if (m_radarPlaylist.isEmpty()) {
            m_playlistHeaderLabel->setText(QStringLiteral("SonicRadar Mix"));
            return;
        }
        m_playlistHeaderLabel->setText(
            QStringLiteral("SonicRadar Mix · Basado en %1 (%2/%3)")
                .arg(m_radarSeedName)
                .arg(qMax(1, m_radarIndex + 1))
                .arg(m_radarPlaylist.size())
        );
    }

    void playRadarPlaylistAtIndex(int idx) {
        if (idx < 0 || idx >= m_radarPlaylist.size()) return;
        m_radarIndex = idx;
        m_lastRadarLaunchMs = QDateTime::currentMSecsSinceEpoch();
        if (m_playlistWidget) {
            m_playlistWidget->setCurrentRow(idx);
        }
        updatePlaylistHeader();
        const auto &track = m_radarPlaylist.at(idx);
        launchTrackInAmazon(track.first, track.second);
    }

    void launchTrackInAmazon(const QString &artist, const QString &title) {
        ensureWebViewInitialized();
        if (!m_webView || !m_webView->page()) return;

        m_expArtist->setText(QStringLiteral("SonicRadar [%1/%2]: %3 - %4")
            .arg(m_radarIndex + 1)
            .arg(qMax(1, int(m_radarPlaylist.size())))
            .arg(artist, title));

        QString query = artist + QStringLiteral(" ") + title;
        QJsonObject payloadObj;
        payloadObj.insert(QStringLiteral("query"), query);
        payloadObj.insert(QStringLiteral("title"), title);
        payloadObj.insert(QStringLiteral("artist"), artist);
        QString payloadJson = QString::fromUtf8(QJsonDocument(payloadObj).toJson(QJsonDocument::Compact));

        // Drive Amazon Music's Single Page App (SPA) search & deep Shadow DOM play WITHOUT page reload!
        QString spaPlayJs = QStringLiteral(R"(
            (function(payload) {
                const query = payload.query;
                const targetTitle = payload.title.toLowerCase();

                function findAllDeep(root, predicate, results = []) {
                    if (!root) return results;
                    const children = root.children || (root.childNodes ? Array.from(root.childNodes).filter(n => n.nodeType === 1) : []);
                    for (const el of children) {
                        const tag = (el.tagName || '').toLowerCase();
                        if (tag === 'music-transport' || el.id === 'transport' || el.getAttribute('slot') === 'trackInfo') {
                            continue;
                        }
                        if (predicate(el)) results.push(el);
                        if (el.shadowRoot) findAllDeep(el.shadowRoot, predicate, results);
                        findAllDeep(el, predicate, results);
                    }
                    return results;
                }

                function findSearchInput(root) {
                    if (!root) return null;
                    const children = root.children || [];
                    for (const el of children) {
                        const tag = (el.tagName || '').toLowerCase();
                        if (tag === 'input') {
                            const type = (el.getAttribute('type') || '').toLowerCase();
                            const id = (el.id || '').toLowerCase();
                            const ph = (el.getAttribute('placeholder') || '').toLowerCase();
                            const al = (el.getAttribute('aria-label') || '').toLowerCase();
                            if (type === 'search' || id.includes('search') || ph.includes('buscar') || ph.includes('search') || al.includes('buscar') || al.includes('search')) {
                                return el;
                            }
                        }
                        if (el.shadowRoot) {
                            const found = findSearchInput(el.shadowRoot);
                            if (found) return found;
                        }
                        const found = findSearchInput(el);
                        if (found) return found;
                    }
                    return null;
                }

                // 1) Navigate SPA to search results without reloading the page
                let input = findSearchInput(document);
                if (!input) {
                    const searchToggles = findAllDeep(document, el => {
                        const icon = (el.getAttribute('icon-name') || '').toLowerCase();
                        const href = (el.getAttribute('href') || '').toLowerCase();
                        return icon.includes('search') || href === '/search';
                    });
                    if (searchToggles.length > 0) searchToggles[0].click();
                    input = findSearchInput(document);
                }

                if (input) {
                    input.focus();
                    const setter = Object.getOwnPropertyDescriptor(window.HTMLInputElement.prototype, 'value').set;
                    if (setter) setter.call(input, query);
                    else input.value = query;
                    input.dispatchEvent(new Event('input', { bubbles: true, composed: true }));
                    input.dispatchEvent(new Event('change', { bubbles: true, composed: true }));
                    const enterOpts = { key: 'Enter', code: 'Enter', keyCode: 13, which: 13, bubbles: true, cancelable: true, composed: true };
                    input.dispatchEvent(new KeyboardEvent('keydown', enterOpts));
                    input.dispatchEvent(new KeyboardEvent('keypress', enterOpts));
                    input.dispatchEvent(new KeyboardEvent('keyup', enterOpts));
                    if (input.form) {
                        input.form.dispatchEvent(new Event('submit', { bubbles: true, cancelable: true }));
                    }
                }

                const encoded = encodeURIComponent(query);
                setTimeout(() => {
                    if (!window.location.pathname.includes('/search')) {
                        window.history.pushState({}, '', '/search/' + encoded);
                        window.dispatchEvent(new PopStateEvent('popstate'));
                    }
                }, 140);

                // 2) Poll rendered search results and click the Play button inside Shadow DOM
                if (window.__sonicRadarTimer) clearInterval(window.__sonicRadarTimer);
                let attempts = 0;
                const initialMediaTitle = (navigator.mediaSession && navigator.mediaSession.metadata)
                    ? (navigator.mediaSession.metadata.title || '') : '';

                window.__sonicRadarTimer = setInterval(() => {
                    attempts++;
                    const curMediaTitle = (navigator.mediaSession && navigator.mediaSession.metadata)
                        ? (navigator.mediaSession.metadata.title || '') : '';
                    if ((curMediaTitle && curMediaTitle !== initialMediaTitle && attempts > 2) || attempts > 14) {
                        clearInterval(window.__sonicRadarTimer);
                        window.__sonicRadarTimer = null;
                        return;
                    }

                    // Find song rows in search results
                    const rows = findAllDeep(document, el => {
                        const tag = (el.tagName || '').toLowerCase();
                        return tag === 'music-image-row' || tag === 'music-horizontal-item';
                    });

                    if (rows.length === 0) return;

                    // Prefer a row whose primary-text matches the target title
                    let bestRow = rows[0];
                    for (const r of rows) {
                        const pText = (r.getAttribute('primary-text') || r.textContent || '').toLowerCase();
                        if (pText && pText.includes(targetTitle.slice(0, Math.min(8, targetTitle.length)))) {
                            bestRow = r;
                            break;
                        }
                    }

                    // Trigger hover events so Amazon Music renders/enables the row's play overlay
                    bestRow.dispatchEvent(new MouseEvent('mouseenter', { bubbles: true, composed: true }));
                    bestRow.dispatchEvent(new MouseEvent('mouseover', { bubbles: true, composed: true }));

                    // Find play button inside bestRow (and its shadowRoot)
                    const playBtns = [];
                    function collectPlayBtns(node) {
                        if (!node) return;
                        const children = node.children || [];
                        for (const c of children) {
                            const tag = (c.tagName || '').toLowerCase();
                            const icon = (c.getAttribute('icon-name') || '').toLowerCase();
                            const aria = (c.getAttribute('aria-label') || '').toLowerCase();
                            const cls = (c.className && typeof c.className === 'string') ? c.className.toLowerCase() : '';
                            if ((tag === 'music-button' && icon.includes('play')) ||
                                (tag === 'button' && (aria.includes('play') || aria.includes('reproducir') || cls.includes('play')))) {
                                playBtns.push(c);
                            }
                            if (c.shadowRoot) collectPlayBtns(c.shadowRoot);
                            collectPlayBtns(c);
                        }
                    }
                    if (bestRow.shadowRoot) collectPlayBtns(bestRow.shadowRoot);
                    collectPlayBtns(bestRow);

                    if (playBtns.length > 0) {
                        const btn = playBtns[0];
                        btn.click();
                        if (btn.shadowRoot) {
                            const innerBtn = btn.shadowRoot.querySelector('button, [role="button"]');
                            if (innerBtn) innerBtn.click();
                        }
                        clearInterval(window.__sonicRadarTimer);
                        window.__sonicRadarTimer = null;
                    }
                }, 420);
            })(%1);
        )").arg(payloadJson);

        m_webView->page()->runJavaScript(spaPlayJs);
    }

    void triggerMediaControl(const QString &method) {
        // When SonicRadar playlist is active, Next / Previous navigate the SonicRadar playlist directly!
        if (method == QStringLiteral("Next") && !m_radarPlaylist.isEmpty()) {
            int nextIdx = (m_radarIndex + 1) % m_radarPlaylist.size();
            playRadarPlaylistAtIndex(nextIdx);
            return;
        }
        if (method == QStringLiteral("Previous") && !m_radarPlaylist.isEmpty() && m_radarIndex > 0) {
            playRadarPlaylistAtIndex(m_radarIndex - 1);
            return;
        }

        if (m_webView && m_webView->page() && m_webHasTrack) {
            QString js;
            if (method == QStringLiteral("PlayPause")) {
                js = QStringLiteral(R"(
                    (function() {
                        const btn = document.querySelector('music-button[icon-name="pause"], music-button[icon-name="play"], [aria-label*="Pausa"], [aria-label*="Reproducir"], [aria-label*="Pause"], [aria-label*="Play"]');
                        if (btn) { btn.click(); return; }
                        const a = document.querySelector('audio, video');
                        if (a) { a.paused ? a.play() : a.pause(); }
                    })();
                )");
            } else if (method == QStringLiteral("Next")) {
                js = QStringLiteral(R"(
                    (function() {
                        const btn = document.querySelector('music-button[icon-name="next"], [aria-label*="Siguiente"], [aria-label*="Next"]');
                        if (btn) btn.click();
                    })();
                )");
            } else if (method == QStringLiteral("Previous")) {
                js = QStringLiteral(R"(
                    (function() {
                        const btn = document.querySelector('music-button[icon-name="previous"], [aria-label*="Anterior"], [aria-label*="Previous"]');
                        if (btn) btn.click();
                    })();
                )");
            } else if (method == QStringLiteral("Shuffle")) {
                m_shuffleState = !m_shuffleState;
                if (m_shuffleBtn) m_shuffleBtn->setActiveState(m_shuffleState);
                js = QStringLiteral(R"(
                    (function() {
                        const btn = document.querySelector('music-button[icon-name*="shuffle"], [aria-label*="Aleatori"], [aria-label*="Shuffle"], [aria-label*="aleatori"], [aria-label*="shuffle"]');
                        if (btn) btn.click();
                    })();
                )");
            } else if (method == QStringLiteral("Repeat")) {
                m_repeatState = !m_repeatState;
                if (m_repeatBtn) m_repeatBtn->setActiveState(m_repeatState);
                js = QStringLiteral(R"(
                    (function() {
                        const btn = document.querySelector('music-button[icon-name*="repeat"], [aria-label*="Repetir"], [aria-label*="Repeat"], [aria-label*="repetir"], [aria-label*="repeat"]');
                        if (btn) btn.click();
                    })();
                )");
            }
            m_webView->page()->runJavaScript(js);
            QTimer::singleShot(180, this, &DynamicNotch::pollMpris);
            return;
        }
        if (method == QStringLiteral("Shuffle")) {
            m_shuffleState = !m_shuffleState;
            if (m_shuffleBtn) m_shuffleBtn->setActiveState(m_shuffleState);
            setMprisProperty(QStringLiteral("Shuffle"), m_shuffleState);
            return;
        }
        if (method == QStringLiteral("Repeat")) {
            m_repeatState = !m_repeatState;
            if (m_repeatBtn) m_repeatBtn->setActiveState(m_repeatState);
            setMprisProperty(QStringLiteral("LoopStatus"), m_repeatState ? QStringLiteral("Playlist") : QStringLiteral("None"));
            return;
        }
        callMprisMethod(method);
    }

    void setMprisProperty(const QString &propName, const QVariant &val) {
        QString service = m_activeService.isEmpty() ? findActiveMprisService() : m_activeService;
        if (service.isEmpty()) return;
        QDBusMessage msg = QDBusMessage::createMethodCall(
            service,
            QStringLiteral("/org/mpris/MediaPlayer2"),
            QStringLiteral("org.freedesktop.DBus.Properties"),
            QStringLiteral("Set")
        );
        msg << QStringLiteral("org.mpris.MediaPlayer2.Player") << propName << QVariant::fromValue(QDBusVariant(val));
        QDBusConnection::sessionBus().asyncCall(msg);
        QTimer::singleShot(150, this, &DynamicNotch::pollMpris);
    }

    void callMprisMethod(const QString &method) {
        QString service = m_activeService.isEmpty() ? findActiveMprisService() : m_activeService;
        if (service.isEmpty()) return;
        QDBusMessage msgPlayer = QDBusMessage::createMethodCall(
            service,
            QStringLiteral("/org/mpris/MediaPlayer2"),
            QStringLiteral("org.mpris.MediaPlayer2.Player"),
            method
        );
        QDBusConnection::sessionBus().asyncCall(msgPlayer);
        QTimer::singleShot(150, this, &DynamicNotch::pollMpris);
    }

    void setupLocalServer() {
        QLocalServer::removeServer(QStringLiteral("amazon-music-notch-ipc"));
        m_ipcServer = new QLocalServer(this);
        connect(m_ipcServer, &QLocalServer::newConnection, this, [this]() {
            while (QLocalSocket *client = m_ipcServer->nextPendingConnection()) {
                connect(client, &QLocalSocket::readyRead, this, [this, client]() {
                    QByteArray cmd = client->readAll().trimmed();
                    if (cmd == "toggle" || cmd.isEmpty()) {
                        toggleExpanded();
                    } else if (cmd == "expand") {
                        expand();
                    } else if (cmd == "collapse") {
                        collapse();
                    }
                    client->disconnectFromServer();
                });
                connect(client, &QLocalSocket::disconnected, client, &QObject::deleteLater);
            }
        });
        m_ipcServer->setSocketOptions(QLocalServer::UserAccessOption);
        m_ipcServer->listen(QStringLiteral("amazon-music-notch-ipc"));
    }

    void setupGlobalShortcuts() {
        m_toggleAction = new QAction(this);
        m_toggleAction->setObjectName(QStringLiteral("toggle_amazon_music_notch"));
        m_toggleAction->setText(QStringLiteral("Mostrar/Ocultar Isla de Amazon Music (Tecla Copilot)"));

        QList<QKeySequence> shortcuts = {
            QKeySequence(Qt::META | Qt::SHIFT | Qt::Key_F23),
            QKeySequence(Qt::Key_F23),
            QKeySequence(Qt::META | Qt::Key_F23),
            QKeySequence(Qt::META | Qt::SHIFT | Qt::Key_TouchpadOff),
            QKeySequence(Qt::Key_Menu)
        };

        QString customKey = m_settings.value(QStringLiteral("customShortcut")).toString();
        if (!customKey.isEmpty()) {
            QKeySequence customSeq(customKey);
            if (!customSeq.isEmpty() && !shortcuts.contains(customSeq)) {
                shortcuts.prepend(customSeq);
            }
        }

        KGlobalAccel::self()->setDefaultShortcut(m_toggleAction, shortcuts);
        KGlobalAccel::self()->setShortcut(m_toggleAction, shortcuts, KGlobalAccel::NoAutoloading);

        connect(m_toggleAction, &QAction::triggered, this, &DynamicNotch::toggleExpanded);
    }

    void bindCustomShortcut(const QKeySequence &seq) {
        m_settings.setValue(QStringLiteral("customShortcut"), seq.toString());
        m_settings.sync();

        QList<QKeySequence> shortcuts = {
            seq,
            QKeySequence(Qt::META | Qt::SHIFT | Qt::Key_F23),
            QKeySequence(Qt::Key_F23),
            QKeySequence(Qt::META | Qt::Key_F23)
        };
        KGlobalAccel::self()->setDefaultShortcut(m_toggleAction, shortcuts);
        KGlobalAccel::self()->setShortcut(m_toggleAction, shortcuts, KGlobalAccel::NoAutoloading);
    }

private:
    QSettings m_settings;
    LayerShellQt::Window *m_layerWindow = nullptr;
    QAction *m_toggleAction = nullptr;

    QWidget *m_compactContainer = nullptr;
    QWidget *m_expandedContainer = nullptr;
    QVBoxLayout *m_expLayout = nullptr;

    AlbumArtWidget *m_artCompact = nullptr;
    QLabel *m_compactTitle = nullptr;
    EqualizerWidget *m_eqCompact = nullptr;

    AlbumArtWidget *m_artExpanded = nullptr;
    QLabel *m_expTitle = nullptr;
    QLabel *m_expArtist = nullptr;
    VectorIconButton *m_shuffleBtn = nullptr;
    VectorIconButton *m_prevBtn = nullptr;
    VectorIconButton *m_playBtn = nullptr;
    VectorIconButton *m_nextBtn = nullptr;
    VectorIconButton *m_repeatBtn = nullptr;
    VectorIconButton *m_freshFlowBtn = nullptr;
    VectorIconButton *m_sonicRadarBtn = nullptr;
    VectorIconButton *m_pinBtn = nullptr;
    VectorIconButton *m_keyBtn = nullptr;
    VectorIconButton *m_minBtn = nullptr;
    VectorIconButton *m_closeBtn = nullptr;
    QWidget *m_playlistPanel = nullptr;
    QLabel *m_playlistHeaderLabel = nullptr;
    QListWidget *m_playlistWidget = nullptr;
    QWebEngineView *m_webView = nullptr;

    QTimer *m_pollTimer = nullptr;
    QTimer *m_autoCollapseTimer = nullptr;
    QNetworkAccessManager *m_netManager = nullptr;
    QLocalServer *m_ipcServer = nullptr;

    QString m_activeService;
    QString m_lastArtUrl;
    QString m_lastTitle;
    QString m_lastArtist;
    QString m_lastRawArtist;
    QString m_lastNormKey;
    QString m_radarSeedName;

    QHash<QString, qint64> m_trackHistory;
    QStringList m_seedArtists;
    QList<QPair<QString, QString>> m_radarPlaylist;
    QList<QList<QPair<QString, QString>>> m_pendingBuckets;
    int m_pendingArtistRequests = 0;
    int m_radarIndex = -1;
    qint64 m_lastRadarLaunchMs = 0;
    qint64 m_lastDragStepMs = 0;
    qint64 m_trackStartEpoch = 0;
    int m_consecutiveSkips = 0;
    int m_radarCycle = 0;

    QSize m_compactSize{216, 26};
    QSize m_expandedSize{584, 480};
    QSize m_currentPillSize;

    bool m_expanded = false;
    bool m_webHasTrack = false;
    bool m_webPlaying = false;
    bool m_shuffleState = false;
    bool m_repeatState = false;
    bool m_freshFlowEnabled = true;
    bool m_radarBusy = false;
    bool m_pinned = true;
    bool m_recordingKey = false;
    bool m_mouseDown = false;
    bool m_dragging = false;
    QPoint m_pressLocalPos;
    int m_posX = -1;
    int m_posY = 6;
};

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        QString arg = QString::fromLocal8Bit(argv[i]);
        if (arg == QStringLiteral("--toggle") || arg == QStringLiteral("--expand") || arg == QStringLiteral("--collapse")) {
            QCoreApplication coreApp(argc, argv);
            QLocalSocket sock;
            sock.connectToServer(QStringLiteral("amazon-music-notch-ipc"));
            if (sock.waitForConnected(400)) {
                QByteArray cmd = arg.mid(2).toUtf8();
                sock.write(cmd);
                sock.flush();
                sock.waitForBytesWritten(300);
                return 0;
            }
        }
    }

    qputenv("QT_QPA_PLATFORM", "wayland");

    // Performance & Low-RAM flags for embedded Chromium so it runs featherlight even with 100+ browser tabs open!
    QString chromiumFlags = QStringLiteral(
        "--autoplay-policy=no-user-gesture-required "
        "--enable-features=Widevine "
        "--renderer-process-limit=1 "
        "--disable-background-networking "
        "--disable-component-update "
        "--disable-domain-reliability "
        "--disable-sync"
    );
    const QString widevinePath = findWidevineLib();
    if (!widevinePath.isEmpty()) {
        chromiumFlags.prepend(QStringLiteral("--widevine-path=%1 ").arg(widevinePath));
    }
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", chromiumFlags.toUtf8());

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("amazon-music-notch"));
    app.setApplicationDisplayName(QStringLiteral("Amazon Music Island"));
    app.setDesktopFileName(QStringLiteral("amazon-music-notch"));
    app.setWindowIcon(QIcon(appCacheDir() + QStringLiteral("/icon.svg")));
    app.setQuitOnLastWindowClosed(false);

    {
        QLocalSocket testSock;
        testSock.connectToServer(QStringLiteral("amazon-music-notch-ipc"));
        if (testSock.waitForConnected(200)) {
            testSock.write("toggle");
            testSock.flush();
            testSock.waitForBytesWritten(200);
            return 0;
        }
    }

    migrateLegacyState();

    DynamicNotch notch;
    notch.setupLayerShell();
    notch.show();

    for (int i = 1; i < argc; ++i) {
        QString arg = QString::fromLocal8Bit(argv[i]);
        if (arg == QStringLiteral("--toggle") || arg == QStringLiteral("--expand")) {
            QTimer::singleShot(80, &notch, [&notch]() { notch.expand(); });
        }
    }

    return app.exec();
}

#include "main.moc"
