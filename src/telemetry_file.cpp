#include "telemetry.h"
#include <QFile>

namespace lab {
ImportResult loadTelemetry(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return {{}, {{0, "Datei konnte nicht geöffnet werden"}}};
    }
    return parseTelemetry(QString::fromUtf8(file.readAll()));
}
}
