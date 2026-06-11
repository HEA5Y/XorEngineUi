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
    if (m_isPaused != paused) {
        m_isPaused = paused;
        if (m_isPaused) {
            emit logMessage("Обработка приостановлена.");
        } else {
            emit logMessage("Обработка возобновлена.");
            if (!m_taskQueue.isEmpty()) {
                emit requestNextFile();
            }
        }
    }
}

void XorWorker::processNextFile() {
    if (m_isPaused || m_taskQueue.isEmpty()) {
        return;
    }

    QString fileName = m_taskQueue.dequeue();
    QString fullIn = QString::fromStdString(m_config.inputPath) + "/" + fileName;
    QString outDir = QString::fromStdString(m_config.outputPath);
    QString fullOut = outDir + "/" + fileName;

    // Защита: Проверяем физическое существование файла на диске перед обработкой
    if (!QFile::exists(fullIn)) {
        m_processedFiles.insert(fileName);
        emit requestNextFile();
        return;
    }

    // Логика конфликтов имён файлов
    if (QFile::exists(fullOut)) {
        if (m_config.conflictStrategy == 0) { // Пропустить
            emit logMessage("Пропуск (уже существует): " + fileName);
            m_processedFiles.insert(fileName);
            emit requestNextFile();
            return;
        }
        else if (m_config.conflictStrategy == 2) { // Переименовать (Индекс)
            QFileInfo fi(fileName);
            int counter = 1;
            do {
                fullOut = QString("%1/%2_%3.%4").arg(outDir, fi.completeBaseName(), QString::number(counter++), fi.completeSuffix());
            } while (QFile::exists(fullOut));
        }
    }

    emit logMessage("Обработка: " + fileName);
    m_lastReportedProgress = -1;

    bool success = m_engine.crypto(fullIn.toStdString().c_str(),
                                   fullOut.toStdString().c_str(),
                                   m_config.mask,
                                   [this](int percent) {
                                       if (percent > m_lastReportedProgress) {
                                           m_lastReportedProgress = percent;
                                           emit progressUpdated(percent);
                                       }
                                   });

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

    // Режим таймера (задержка между файлами)
    if (m_config.isTimerMode && m_config.intervalS > 0) {
        QThread::msleep(m_config.intervalS * 1000);
    }

    // Запрашиваем следующий файл из очереди
    emit requestNextFile();
}