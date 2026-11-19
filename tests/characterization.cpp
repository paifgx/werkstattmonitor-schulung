#include "telemetry.h"
#include <QtTest>
class Characterization : public QObject {
    Q_OBJECT
private slots:
    void validInput() {
        const auto result = lab::parseTelemetry(
            "sequence;rpm;temperature_c;state\n1;0;22.0;IDLE\n2;1200;36.5;RUN\n"
            "3;1800;48.0;RUN\n4;0;65.5;ALARM\n5;0;28.0;IDLE\n");
        QVERIFY(result.ok());
        QCOMPARE(result.samples.size(), 5);
        QCOMPARE(result.samples.at(1).rpm, 1200);
        QCOMPARE(result.samples.at(3).state, lab::State::Alarm);
        const auto summary = lab::summarize(result.samples);
        QCOMPARE(summary.count, 5);
        QCOMPARE(summary.peakRpm, 1800);
        QCOMPARE(summary.alarms, 1);
        QVERIFY(qAbs(summary.meanTemperatureC - 40.0) < 0.000001);
    }
    void independentValidInput() {
        const auto result = lab::parseTelemetry(
            "sequence;rpm;temperature_c;state\n"
            "1;300;10;RUN\n2;600;20;ALARM\n3;0;30;IDLE\n");
        QVERIFY(result.ok());
        QCOMPARE(result.samples.size(), 3);
        const auto summary = lab::summarize(result.samples);
        QCOMPARE(summary.count, 3);
        QCOMPARE(summary.peakRpm, 600);
        QCOMPARE(summary.meanTemperatureC, 20.0);
        QCOMPARE(summary.alarms, 1);
    }
    void emptySummary() {
        const auto summary = lab::summarize({});
        QCOMPARE(summary.count, 0);
        QCOMPARE(summary.meanTemperatureC, 0.0);
    }
    void names() {
        QCOMPARE(lab::stateText(lab::State::Running), QString("RUN"));
        QCOMPARE(lab::stateText(lab::State::Alarm), QString("ALARM"));
        QCOMPARE(lab::stateText(lab::State::Idle), QString("IDLE"));
    }
};
QTEST_APPLESS_MAIN(Characterization)
#include "characterization.moc"
