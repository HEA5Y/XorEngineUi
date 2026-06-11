#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QListWidget>
#include <QProgressBar>
#include <QFileDialog>
#include <QThread>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QSet>
#include "..\core\TaskConfig.h"
#include "../core/worker/xorworker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void filesAdded(const QStringList &files);

private slots:
    void onBrowseSearch();
    void onBrowseSave();
    void onStart();
    void onPauseToggle();
    void onStop();
    void onLogMessage(const QString &message);
    void onProgressUpdated(int value);
    void onWorkerFinished();
    void updateFileList();

private:
    void setupConnections();
    void setupWatcher(const QString &path);
    void refreshComboBoxFiles(const QString &path);


    // 1 ЧАСТЬ ОКНА_____________
    QGroupBox* m_groupPaths;
    QLineEdit* m_lineSearchPath;
    QPushButton* m_btnBrowseSearch;
    QLineEdit* m_lineSavePath;
    QPushButton* m_btnBrowseSave;

    // 2 ЧАСТЬ ОКНА_____________
    // левая
    QGroupBox* m_groupOptions;
    QLineEdit* m_lineHex;
    QComboBox* m_coincidenceComboBox;
    QCheckBox* m_deleteFileCheck;

    // правая
    QGroupBox* m_groupOptionsStart;
    QRadioButton* m_radioBtnSingle;
    QRadioButton* m_radioBtnTimer;
    QSpinBox* m_spinInterval;

    // 3 ЧАСТЬ ОКНА_____________
    // левая
    QGroupBox* m_groupListFiles;
    QComboBox* m_comboTypeFiles;
    QListWidget* m_filesList;
    QCheckBox* m_separatelyCheck;

    // правая
    QGroupBox* m_groupListLogs;
    QListWidget* m_logsList;

    // 4 ЧАСТЬ ОКНА_____________
    QGroupBox* m_groupDown1;
    QProgressBar* m_progressBar;
    QGroupBox* m_groupDown2;
    QPushButton* m_btnStart;
    QPushButton* m_btnPause;
    QPushButton* m_btnStop;

    // Переменные состояния
    bool m_isProcessing = false;
    bool m_isPaused = false;

    TaskConfig config;
    XorWorker* worker = nullptr;
    QThread* m_workerThread = nullptr;
    QFileSystemWatcher* m_fileWatcher = nullptr;
    QTimer* m_refreshTimer = nullptr;
};

#endif // MAINWINDOW_H