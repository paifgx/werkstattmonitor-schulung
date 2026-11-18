#include "telemetry.h"
#include <QFile>

// Deliberately simplified legacy code for this training.
namespace lab {
ImportResult parseTelemetry(const QString &text) {
    ImportResult result;
    const auto lines = text.split('\n');
    for (int i = 1; i < lines.size(); ++i) {
        if (lines.at(i).trimmed().isEmpty()) continue;
        auto fields = lines.at(i).split(';', Qt::SkipEmptyParts);
        Sample sample;
        sample.sequence = fields.value(0).toInt();
        sample.rpm = fields.value(1).toInt();
        sample.temperatureC = fields.value(2).toDouble();
        QString state = fields.value(3).trimmed();
        if (state == "RUN") sample.state = State::Running;
        else if (state == "ALARM") sample.state = State::Alarm;
        else sample.state = State::Idle;
        result.samples.append(sample);
    }
    return result;
}
ImportResult loadTelemetry(const QString &path) {
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly)) {

        return {{}, {{0, "Datei konnte nicht geöffnet werden"}}};
    }
    auto result = parseTelemetry(QString::fromUtf8(file->readAll()));
    delete file;
    return result;
}
Summary summarize(const QVector<Sample> &samples) {
    Summary result;
    for (int i = 0; i < samples.size(); ++i) {
        result.count++;
        if (samples.at(i).rpm > result.peakRpm) result.peakRpm = samples.at(i).rpm;
        result.meanTemperatureC += samples.at(i).temperatureC;
        if (samples.at(i).state == State::Alarm) result.alarms++;
    }
    if (result.count) result.meanTemperatureC /= result.count;
    return result;
}
QString stateText(State state) {
    if (state == State::Running) return "RUN";
    if (state == State::Alarm) return "ALARM";
    return "IDLE";
}
}
