#include "hmiwindow.h"
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

HmiWindow::HmiWindow(const QString &path, QWidget *parent) : QWidget(parent) {
    setWindowTitle("Werkstattmonitor · synthetisches Übungslabor");
    resize(700, 420);
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Nur Anzeige fiktiver Messwerte · keine Geräteanbindung", this));
    path_ = new QLineEdit(path, this);
    path_->setObjectName("filePath");
    layout->addWidget(path_);
    auto *buttons = new QHBoxLayout;
    start_ = new QPushButton("Datei abspielen", this);
    start_->setObjectName("startButton");
    stop_ = new QPushButton("Stopp", this);
    stop_->setObjectName("stopButton");
    stop_->setEnabled(false);
    buttons->addWidget(start_);
    buttons->addWidget(stop_);
    layout->addLayout(buttons);
    table_ = new QTableWidget(0, 4, this);
    table_->setObjectName("samplesTable");
    table_->setHorizontalHeaderLabels({"Folge", "Drehzahl", "Temperatur °C", "Zustand"});
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(table_);
    status_ = new QLabel("Bereit", this);
    status_->setObjectName("statusLabel");
    heartbeat_ = new QLabel("UI-Ticks: 0", this);
    heartbeat_->setObjectName("heartbeatLabel");
    layout->addWidget(status_);
    layout->addWidget(heartbeat_);
    auto *heartbeatTimer = new QTimer(this);
    connect(heartbeatTimer, &QTimer::timeout, this, [this] {
        heartbeat_->setText(QString("UI-Ticks: %1").arg(++ticks_));
    });
    heartbeatTimer->start(20);
    connect(start_, &QPushButton::clicked, this, &HmiWindow::startReplay);

}
void HmiWindow::appendSample(const lab::Sample &sample) {
    const int row = table_->rowCount();
    table_->insertRow(row);
    table_->setItem(row, 0, new QTableWidgetItem(QString::number(sample.sequence)));
    table_->setItem(row, 1, new QTableWidgetItem(QString::number(sample.rpm)));
    table_->setItem(row, 2, new QTableWidgetItem(QString::number(sample.temperatureC, 'f', 1)));
    table_->setItem(row, 3, new QTableWidgetItem(lab::stateText(sample.state)));
}

void HmiWindow::startReplay() {
    table_->setRowCount(0);
    auto result = lab::loadTelemetry(path_->text());
    if (!result.ok()) { status_->setText(result.errors.first().message); return; }
    start_->setEnabled(false);
    stop_->setEnabled(true);
    for (const auto &sample : result.samples) {
        QThread::msleep(120);
        appendSample(sample);
    }
    status_->setText("Fertig");
    start_->setEnabled(true);
    stop_->setEnabled(false);
}
