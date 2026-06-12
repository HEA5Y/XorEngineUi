#include "xorworker.h"
#include <utility>

XorWorker::XorWorker(QObject *parent) : QObject(parent) {
    connect(this, &XorWorker::requestNextFile, this, &XorWorker::processNextFile, Qt::QueuedConnection);
}

void XorWorker::startProcessing(TaskConfig config) {
    m_config = config;
    m_isAlive = true;
    m_isPaused = false;
    m_processedFiles.clear();
    m_taskQueue.clear();

    emit logMessage("Наблюдение за папкой активно...");
    enqueueFiles(config.inputFiles);
}

void XorWorker::enqueueFiles(const QStringList &files) {
    bool wasEmpty = m_taskQueue.isEmpty();

    for (const QString &file : std::as_const(files)) {
        if (!m_taskQueue.contains(file) && !m_processedFiles.contains(file)) {
            m_taskQueue.enqueue(file);
        }
    }
    if (wasEmpty && !m_taskQueue.isEmpty() && !m_isPaused) {
        emit requestNextFile();
    }
}

void XorWorker::setPaused(bool paused) {
    qDebug() << "DEBUG: setPaused вызван с =" << paused;
    m_isPaused.store(paused);
    if (!paused) {
        qDebug() << "DEBUG: Разбудка потоков (wakeAll)";
        m_pauseMutex.lock();
        m_pauseCond.wakeAll();
        m_pauseMutex.unlock();
    }
}

void XorWorker::processNextFile() {
    if (m_taskQueue.isEmpty()) return;

    QString fileName = m_taskQueue.dequeue();
    QString fullIn = QString::fromStdString(m_config.inputPath) + "/" + fileName;
    QString outDir = QString::fromStdString(m_config.outputPath);
    QString fullOut = outDir + "/" + fileName;

    if (!QFile::exists(fullIn)) {
        m_processedFiles.insert(fileName);
        emit requestNextFile();
        return;
    }

    if (QFile::exists(fullOut)) {
        if (m_config.conflictStrategy == 0) {
            emit logMessage("Пропуск (уже существует): " + fileName);
            m_processedFiles.insert(fileName);
            emit requestNextFile();
            return;
        }
        else if (m_config.conflictStrategy == 2) {
            QFileInfo fi(fileName);
            int counter = 1;
            do {
                fullOut = QString("%1/%2_%3.%4").arg(outDir, fi.completeBaseName(), QString::number(counter++), fi.completeSuffix());
            } while (QFile::exists(fullOut));
        }
    }

    emit logMessage("Обработка: " + fileName);
    m_lastReportedProgress = -1;

    auto progressFunc = [this](int percent) {
        if (percent > m_lastReportedProgress) {
            m_lastReportedProgress = percent;
            emit progressUpdated(percent);
        }
    };

    bool success = m_engine.crypto(fullIn.toStdString().c_str(),
                                   fullOut.toStdString().c_str(),
                                   m_config.mask,progressFunc,
                                   &m_isPaused, &m_pauseMutex, &m_pauseCond);

    if (success) {
        emit logMessage("Готово: " + fileName);
        m_processedFiles.insert(fileName);
        if (m_config.deleteOriginals) {
            if (QFile::remove(fullIn)) {
                emit logMessage("Оригинал удален: " + fileName);
            }
        }
    } else {
        emit logMessage("Ошибка обработки: " + fileName);
        m_processedFiles.insert(fileName);
    }

    if (m_config.isTimerMode && m_config.intervalS > 0) {
        m_pauseMutex.lock();
        if (m_isPaused.load()) {
            m_pauseCond.wait(&m_pauseMutex);
        } else {
            m_pauseCond.wait(&m_pauseMutex, m_config.intervalS);
        }
        m_pauseMutex.unlock();
    }

    if (m_isAlive && !m_taskQueue.isEmpty()) {
        emit requestNextFile();
    } else {
        emit finished();
    }
}