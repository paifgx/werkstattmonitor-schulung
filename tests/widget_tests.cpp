#include "hmiwindow.h"
#include <QtTest>
#include <QElapsedTimer>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTemporaryDir>
#include <QTimer>
#include <memory>

class WidgetTests : public QObject {
    Q_OBJECT
    QTemporaryDir dir_;
    QString path_;
private slots:
    void initTestCase() {
        QVERIFY(dir_.isValid());
        path_ = dir_.filePath("sample.csv");
        QFile file(path_);
        QVERIFY(file.open(QIODevice::WriteOnly));
        file.write("sequence;rpm;temperature_c;state\n1;0;22;IDLE\n2;1200;36;RUN\n"
                   "3;1800;48;RUN\n4;0;65;ALARM\n5;0;28;IDLE\n");
    }
    void smoke() {
        HmiWindow window(path_);
        QVERIFY(window.findChild<QPushButton *>("startButton"));
        QCOMPARE(window.findChild<QTableWidget *>("samplesTable")->rowCount(), 0);
    }
    void responsiveAndCompletes() {
        HmiWindow window(path_);
        auto *start = window.findChild<QPushButton *>("startButton");
        auto *status = window.findChild<QLabel *>("statusLabel");
        auto *table = window.findChild<QTableWidget *>("samplesTable");
        auto *heartbeat = window.findChild<QLabel *>("heartbeatLabel");
        QElapsedTimer elapsed;
        elapsed.start();
        start->click();
        QVERIFY2(elapsed.elapsed() < 300, "Start blockiert den GUI-Thread (Legacy: ca. 600 ms)");
        const QString previous = heartbeat->text();
        QTRY_VERIFY_WITH_TIMEOUT(heartbeat->text() != previous, 1000);
        QTRY_COMPARE_WITH_TIMEOUT(status->text(), QString("Fertig"), 2000);
        QCOMPARE(table->rowCount(), 5);
        QVERIFY(start->isEnabled());
    }
    void cancelAndRestart() {
        HmiWindow window(path_);
        auto *start = window.findChild<QPushButton *>("startButton");
        auto *stop = window.findChild<QPushButton *>("stopButton");
        auto *table = window.findChild<QTableWidget *>("samplesTable");
        start->click();
        QTRY_VERIFY_WITH_TIMEOUT(table->rowCount() > 0, 1000);
        QVERIFY(stop->isEnabled());
        stop->click();
        const int rows = table->rowCount();
        QVERIFY(rows < 5);
        QTest::qWait(300);
        QCOMPARE(table->rowCount(), rows);
        QVERIFY(start->isEnabled());
        start->click();
        QTRY_COMPARE_WITH_TIMEOUT(table->rowCount(), 5, 2000);
    }
    void invalidAfterSuccess() {
        HmiWindow window(path_);
        auto *start = window.findChild<QPushButton *>("startButton");
        auto *table = window.findChild<QTableWidget *>("samplesTable");
        start->click();
        QTRY_COMPARE_WITH_TIMEOUT(table->rowCount(), 5, 2000);
        QFile file(path_);
        QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
        file.write("sequence;rpm;temperature_c;state\n1;bad;42;RUN\n");
        file.close();
        start->click();
        QCOMPARE(table->rowCount(), 0);
        QVERIFY(start->isEnabled());
        QVERIFY(window.findChild<QLabel *>("statusLabel")->text().contains("Zeile 2"));
    }
    void destructionDuringReplay() {
        // A separate valid file; the previous test modified sample.csv.
        QFile file(dir_.filePath("close.csv"));
        QVERIFY(file.open(QIODevice::WriteOnly));
        file.write("sequence;rpm;temperature_c;state\n1;0;22;IDLE\n2;0;22;IDLE\n");
        file.close();
        auto window = std::make_unique<HmiWindow>(file.fileName());
        window->findChild<QPushButton *>("startButton")->click();
        window.reset();
        QTest::qWait(300); // Timers must not call into the deleted receiver.
    }
};
QTEST_MAIN(WidgetTests)
#include "widget_tests.moc"
