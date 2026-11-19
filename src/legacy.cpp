#include "telemetry.h"
#include <QRegularExpression>

// Deliberately simplified legacy code for this training.
namespace lab {
ImportResult parseTelemetry(const QString &text) {
    ImportResult result;
    const auto lines = text.split('\n');
    QString header = lines.first();
    if (header.startsWith(QChar(0xfeff))) header.remove(0, 1);
    if (header.endsWith('\r')) header.chop(1);
    if (header != "sequence;rpm;temperature_c;state") {
        result.errors.append({1, "Ungültige Kopfzeile"});
        return result;
    }

    const QRegularExpression integerPattern("^[0-9]+$");
    const QRegularExpression temperaturePattern("^-?[0-9]+(?:\\.[0-9]+)?$");
    int previousSequence = 0;
    for (int i = 1; i < lines.size(); ++i) {
        if (lines.at(i).trimmed().isEmpty()) continue;
        const auto fields = lines.at(i).split(';', Qt::KeepEmptyParts);
        if (fields.size() != 4) {
            result.errors.append({i + 1, "Erwartet werden vier Felder"});
            continue;
        }

        const QString sequence = fields.at(0).trimmed();
        const QString rpm = fields.at(1).trimmed();
        const QString temperature = fields.at(2).trimmed();
        const QString state = fields.at(3).trimmed();
        bool sequenceOk = false, rpmOk = false, temperatureOk = false;
        Sample sample;
        sample.sequence = sequence.toInt(&sequenceOk);
        sample.rpm = rpm.toInt(&rpmOk);
        sample.temperatureC = temperature.toDouble(&temperatureOk);
        const bool validSequence = integerPattern.match(sequence).hasMatch() &&
            sequenceOk && sample.sequence > previousSequence;
        const bool validRpm = integerPattern.match(rpm).hasMatch() &&
            rpmOk && sample.rpm <= 24000;
        bool temperatureInRange = false;
        if (temperaturePattern.match(temperature).hasMatch()) {
            const bool negative = temperature.startsWith('-');
            QString magnitude = negative ? temperature.mid(1) : temperature;
            const auto parts = magnitude.split('.');
            QString whole = parts.first();
            while (whole.size() > 1 && whole.startsWith('0')) whole.remove(0, 1);
            const QString limit = negative ? "20" : "150";
            const bool fractionIsZero = parts.size() == 1 ||
                !parts.at(1).contains(QRegularExpression("[1-9]"));
            temperatureInRange = whole.size() < limit.size() ||
                (whole.size() == limit.size() &&
                 (whole < limit || (whole == limit && fractionIsZero)));
        }
        const bool validTemperature = temperatureOk && temperatureInRange;
        const bool validState = state == "IDLE" || state == "RUN" || state == "ALARM";
        if (!validSequence || !validRpm || !validTemperature || !validState) {
            result.errors.append({i + 1, "Ungültige Messwerte oder Reihenfolge"});
        } else {
            if (state == "RUN") sample.state = State::Running;
            else if (state == "ALARM") sample.state = State::Alarm;
            else sample.state = State::Idle;
            result.samples.append(sample);
        }
        if (integerPattern.match(sequence).hasMatch() && sequenceOk && sample.sequence > 0)
            previousSequence = sample.sequence;
    }
    if (result.samples.isEmpty() && result.errors.isEmpty())
        result.errors.append({1, "Keine Messwerte vorhanden"});
    if (!result.errors.isEmpty()) result.samples.clear();
    return result;
}
}
