#include "telemetry.h"
#include <QtTest>
#include <QTemporaryDir>
#include <QFile>
#include <QLocale>

class ParserContract : public QObject {
    Q_OBJECT
private slots:
    void invalid_data() {
        QTest::addColumn<QString>("row");
        QTest::newRow("empty-rpm") << "1;;42.0;RUN";
        QTest::newRow("empty-state") << "1;1;42.0;";
        QTest::newRow("too-few") << "1;1;42.0";
        QTest::newRow("too-many") << "1;1;42.0;RUN;extra";
        QTest::newRow("rpm-text") << "1;abc;42.0;RUN";
        QTest::newRow("negative-rpm") << "1;-1;42.0;RUN";
        QTest::newRow("over-rpm") << "1;24001;42.0;RUN";
        QTest::newRow("overflow") << "1;9999999999999999999999;42.0;RUN";
        QTest::newRow("nan") << "1;1;nan;RUN";
        QTest::newRow("inf") << "1;1;inf;RUN";
        QTest::newRow("comma") << "1;1;42,5;RUN";
        QTest::newRow("exponent") << "1;1;1e2;RUN";
        QTest::newRow("too-cold") << "1;1;-20.1;RUN";
        QTest::newRow("too-hot") << "1;1;150.1;RUN";
        QTest::newRow("unknown") << "1;1;42.0;READY";
        QTest::newRow("case") << "1;1;42.0;run";
        QTest::newRow("zero-seq") << "0;1;42.0;RUN";
        QTest::newRow("negative-seq") << "-1;1;42.0;RUN";
    }
    void invalid() {
        QFETCH(QString, row);
        const auto result = lab::parseTelemetry("sequence;rpm;temperature_c;state\n" + row);
        QVERIFY2(!result.ok(), qPrintable(row));
        QVERIFY(result.samples.isEmpty());
        QCOMPARE(result.errors.first().line, 2);
        QVERIFY(!result.errors.first().message.isEmpty());
    }
    void boundaryAndLocale() {
        const QLocale old = QLocale();
        QLocale::setDefault(QLocale(QLocale::German));
        const auto result = lab::parseTelemetry(QString(QChar(0xfeff)) +
            "sequence;rpm;temperature_c;state\r\n1;0;-20;IDLE\r\n\r\n 2 ;24000;150.0;ALARM\r\n");
        QLocale::setDefault(old);
        QVERIFY(result.ok());
        QCOMPARE(result.samples.size(), 2);
        QCOMPARE(result.samples.at(1).rpm, 24000);
        QCOMPARE(result.samples.first().temperatureC, -20.0);
    }
    void header_data() {
        QTest::addColumn<QString>("text");
        QTest::newRow("empty") << "";
        QTest::newRow("wrong-header") << "seq;rpm;temp;state\n1;1;1;RUN";
        QTest::newRow("header-only") << "sequence;rpm;temperature_c;state\n";
    }
    void header() {
        QFETCH(QString, text);
        const auto result = lab::parseTelemetry(text);
        QVERIFY(!result.ok());
        QVERIFY(result.samples.isEmpty());
    }
    void atomicAndLineNumbers() {
        auto result = lab::parseTelemetry("sequence;rpm;temperature_c;state\n1;1;42;RUN\n\n2;;42;RUN\n3;1;42;OTHER\n");
        QVERIFY(!result.ok());
        QVERIFY(result.samples.isEmpty());
        QCOMPARE(result.errors.size(), 2);
        QCOMPARE(result.errors.at(0).line, 4);
        QCOMPARE(result.errors.at(1).line, 5);
    }
    void sequenceOrder_data() {
        QTest::addColumn<QString>("last");
        QTest::newRow("duplicate") << "2;1;42;RUN";
        QTest::newRow("backwards") << "1;1;42;RUN";
    }
    void sequenceOrder() {
        QFETCH(QString, last);
        const auto result = lab::parseTelemetry("sequence;rpm;temperature_c;state\n2;1;42;RUN\n" + last);
        QVERIFY(!result.ok());
        QVERIFY(result.samples.isEmpty());
        QCOMPARE(result.errors.first().line, 3);
    }
    void fileLoading() {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        auto missing = lab::loadTelemetry(dir.filePath("missing.csv"));
        QVERIFY(!missing.ok());
        QCOMPARE(missing.errors.first().line, 0);
        QFile file(dir.filePath("good.csv"));
        QVERIFY(file.open(QIODevice::WriteOnly));
        const QByteArray bytes("sequence;rpm;temperature_c;state\n1;1200;42;RUN\n");
        QCOMPARE(file.write(bytes), qint64(bytes.size()));
        file.close();
        const auto loaded = lab::loadTelemetry(file.fileName());
        QVERIFY(loaded.ok());
        QCOMPARE(loaded.samples.size(), 1);
        QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
        const QByteArray large(1024 * 1024 + 1, 'x');
        QCOMPARE(file.write(large), qint64(large.size()));
        file.close();
        QVERIFY(!lab::loadTelemetry(file.fileName()).ok());
    }
};
QTEST_APPLESS_MAIN(ParserContract)
#include "parser_contract.moc"
