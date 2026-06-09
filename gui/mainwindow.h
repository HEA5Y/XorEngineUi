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


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // 1 ЧАСТЬ ОКНА_____________
    QGroupBox* m_groupPaths;
    QLineEdit* m_lineSearchPath;
    QPushButton* m_btnBrowseSearch;
    QLineEdit* m_lineSavePath;
    QPushButton* m_btnBrowseSave;
    // 2 ЧАСТЬ ОКНА_____________
    //левая
    QGroupBox* m_groupOptions;
    QLineEdit* m_lineHex;
    QComboBox* m_coincidenceComboBox;
    QCheckBox* m_deleteFileCheck;

    //правая
    QGroupBox* m_groupOptionsStart;
    QRadioButton* m_radioBtnSingle;
    QRadioButton* m_radioBtnTimer;
    QSpinBox* m_spinInterval;

    // 3 ЧАСТЬ ОКНА_____________
    //левая
    QGroupBox* m_groupListFiles;
    QGroupBox* m_groupListLogs;
public:
    QString m_status;
private:
    QListWidget* m_filesList;
    //правая
    QListWidget* m_logsList;

    // 4 ЧАСТЬ ОКНА_____________
    QGroupBox* m_groupDown1;
    QProgressBar* m_progressBar;
    QGroupBox* m_groupDown2;
    QPushButton* m_btnStart;
    QPushButton* m_btnPause;
    QPushButton* m_btnStop;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

};
#endif // MAINWINDOW_H
