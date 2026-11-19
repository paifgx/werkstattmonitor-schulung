#include "telemetry.h"
#include <QFile>

namespace lab {
ImportResult loadTelemetry(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return {{}, {{0, "Datei konnte nicht geöffnet werden"}}};
    }
    constexpr qint64 maxBytes = 1024 * 1024;
    const QByteArray bytes = file.read(maxBytes + 1);
    if (file.error() != QFileDevice::NoError) {
        return {{}, {{0, "Datei konnte nicht gelesen werden"}}};
    }
    if (bytes.size() > maxBytes) {
        return {{}, {{0, "Datei ist größer als 1 MiB"}}};
    }
    return parseTelemetry(QString::fromUtf8(bytes));
}
}
