#ifndef PROCESSWORKER_H
#define PROCESSWORKER_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QThread>

class ProcessWorker : public QObject
{
    Q_OBJECT
public:
    explicit ProcessWorker(const QString& command, QObject* parent = nullptr);

signals:
    void finished(int exitCode);

public slots:
    void startProcess();
private:
    QString m_command;
    QProcess m_process;
};

#endif // PROCESSWORKER_H
