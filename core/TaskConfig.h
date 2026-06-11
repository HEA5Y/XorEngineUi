#ifndef TASKCONFIG_H
#define TASKCONFIG_H

#include <cstdint>
#include <QMetaType>
#include <QStringList>
#include <QSet>

struct TaskConfig {
    std::string inputPath;
    std::string outputPath;
    uint64_t mask;
    bool isTimerMode;
    int intervalS;
    QStringList inputFiles;
    int conflictStrategy;
    bool deleteOriginals = false;
    QSet<QString> m_processedFiles;
};

Q_DECLARE_METATYPE(TaskConfig)

#endif // TASKCONFIG_H
