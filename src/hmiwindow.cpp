#include "hmiwindow.h"
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
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
    replayTimer_ = new QTimer(this);
    replayTimer_->setInterval(120);
    connect(replayTimer_, &QTimer::timeout, this, &HmiWindow::replayNext);
    connect(start_, &QPushButton::clicked, this, &HmiWindow::startReplay);
    connect(stop_, &QPushButton::clicked, this, &HmiWindow::stopReplay);

}
void HmiWindow::appendSample(const lab::Sample &sample) {
    const int row = table_->rowCount();
    table_->insertRow(row);
    table_->setItem(row, 0, new QTableWidgetItem(QString::number(sample.sequence)));
    table_->setItem(row, 1, new QTableWidgetItem(QString::number(sample.rpm)));
    table_->setItem(row, 2, new QTableWidgetItem(QString::number(sample.temperatureC, 'f', 1)));
    table_->setItem(row, 3, new QTableWidgetItem(lab::stateText(sample.state)));
}

void HmiWindow::setReplaying(bool replaying) {
    start_->setEnabled(!replaying);
    stop_->setEnabled(replaying);
}

void HmiWindow::startReplay() {
    replayTimer_->stop();
    pending_.clear();
    next_ = 0;
    table_->setRowCount(0);
    const auto result = lab::loadTelemetry(path_->text());
    if (!result.ok()) {
        const auto &error = result.errors.first();
        status_->setText(QString("Zeile %1: %2").arg(error.line).arg(error.message));
        setReplaying(false);
        return;
    }
    pending_ = result.samples;
    status_->setText("Wiedergabe läuft");
    setReplaying(true);
    replayTimer_->start();
}

void HmiWindow::stopReplay() {
    replayTimer_->stop();
    pending_.clear();
    status_->setText("Gestoppt");
    setReplaying(false);
}

void HmiWindow::replayNext() {
    if (next_ < pending_.size()) appendSample(pending_.at(next_++));
    if (next_ >= pending_.size()) {
        replayTimer_->stop();
        pending_.clear();
        status_->setText("Fertig");
        setReplaying(false);
    }
}
