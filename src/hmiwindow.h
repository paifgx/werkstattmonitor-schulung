#pragma once
#include "telemetry.h"
#include <QWidget>
class QLineEdit;
class QPushButton;
class QLabel;
class QTableWidget;
class QTimer;

class HmiWindow : public QWidget {
    Q_OBJECT
public:
    explicit HmiWindow(const QString &path, QWidget *parent = nullptr);
private:
    void startReplay();
    void appendSample(const lab::Sample &sample);
    QLineEdit *path_;
    QPushButton *start_;
    QPushButton *stop_;
    QLabel *status_;
    QLabel *heartbeat_;
    QTableWidget *table_;
    int ticks_ = 0;

};
