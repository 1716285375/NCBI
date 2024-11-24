#include "processworker.h"

ProcessWorker::ProcessWorker(const QString& command, QObject* parent) : QObject(parent),
    m_command(command),
    m_process(this)
{
}

void ProcessWorker::startProcess()
{

}
