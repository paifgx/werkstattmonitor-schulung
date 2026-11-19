#include "telemetry.h"

namespace lab {
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
