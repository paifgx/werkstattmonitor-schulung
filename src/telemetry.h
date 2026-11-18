#pragma once
#include <QString>
#include <QVector>

namespace lab {
enum class State { Idle, Running, Alarm };
struct Sample {
    int sequence = 0;
    int rpm = 0;
    double temperatureC = 0.0;
    State state = State::Idle;
};
struct ImportError { int line = 0; QString message; };
struct ImportResult {
    QVector<Sample> samples;
    QVector<ImportError> errors;
    bool ok() const { return errors.isEmpty(); }
};
struct Summary {
    int count = 0;
    int peakRpm = 0;
    double meanTemperatureC = 0.0;
    int alarms = 0;
};
ImportResult parseTelemetry(const QString &text);
ImportResult loadTelemetry(const QString &path);
Summary summarize(const QVector<Sample> &samples);
QString stateText(State state);
} // namespace lab
