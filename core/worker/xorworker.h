#ifndef XORWORKER_H
#define XORWORKER_H

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QDebug>
#include <QThread>
#include <QQueue>
#include <QSet>
#include <QMutex>
#include <atomic>
#include <QWaitCondition>
#include "../libXorEngine.h"
#include "../TaskConfig.h"

class XorWorker : public QObject {
    Q_OBJECT
public:
    explicit XorWorker(QObject *parent = nullptr);

signals:
    void progressUpdated(int value);
    void requestNextFile();
    void finished();
    void logMessage(const QString &message);

public slots:
    void startProcessing(TaskConfig config);
    void processNextFile();
    void enqueueFiles(const QStringList &files);
    void setPaused(bool paused);

private:
    std::atomic<bool> m_isPaused{false};
    QMutex m_pauseMutex;
    QWaitCondition m_pauseCond;
    TaskConfig m_config;
    int m_currentIndex = 0;
    int m_lastReportedProgress = -1;
    XorEngine m_engine;
    bool m_isAlive;
    QQueue<QString> m_taskQueue;
    QSet<QString> m_processedFiles;
};

#endif // XORWORKER_H