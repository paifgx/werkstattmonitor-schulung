#include "telemetry.h"

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
}
