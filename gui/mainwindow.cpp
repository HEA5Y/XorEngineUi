#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(900,700);
    setWindowTitle("XorEngineUi");

    // ОСНОВА ОКНА
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);

    // 1 ЧАСТЬ ОКНА_____________
    m_groupPaths = new QGroupBox("Пути");
    mainLayout->addWidget(m_groupPaths);

    QGridLayout* PathGridL = new QGridLayout(m_groupPaths);
    PathGridL->setSpacing(0);

    m_lineSearchPath = new QLineEdit();//ПУТЬ ВЫБОРКИ
    m_lineSearchPath->setPlaceholderText("Укажите путь директории для выбора файлов...");
    m_btnBrowseSearch = new QPushButton("Обзор");

    m_lineSavePath = new QLineEdit();//ПУТЬ СОХРАНЕНИЯ
    m_lineSavePath->setPlaceholderText("Укажите путь директории для сохранения...");
    m_btnBrowseSave = new QPushButton("Обзор");

    PathGridL->addWidget(m_lineSearchPath,0,0); //УСТАНОВКА ВИДЖЕТОВ НА ГРИД
    PathGridL->addWidget(m_btnBrowseSearch,0,1);
    PathGridL->addWidget(m_lineSavePath,1,0);
    PathGridL->addWidget(m_btnBrowseSave,1,1);


    // 2 ЧАСТЬ ОКНА_____________
    QHBoxLayout* optionsLayout = new QHBoxLayout();
    mainLayout->addLayout(optionsLayout);

    // ОБРАБОТКА
    m_groupOptions = new QGroupBox("Параметры обработки", this);
    optionsLayout->addWidget(m_groupOptions);
    QGridLayout* optionsGrid = new QGridLayout(m_groupOptions);

    QLabel* hexLabel = new QLabel("HEX: ");//ВИДЖЕТЫ
    m_lineHex = new QLineEdit();
    QLabel* coincidenceLabel = new QLabel("При совпадении: ");
    m_coincidenceComboBox = new QComboBox();
    m_deleteFileCheck = new QCheckBox();
    m_deleteFileCheck->setText("Удалять оригиналы");

    optionsGrid->addWidget(hexLabel,0,0); //УСТАНОВКА ВИДЖЕТОВ НА ГРИД
    optionsGrid->addWidget(m_lineHex,0,1);
    optionsGrid->addWidget(coincidenceLabel,1,0);
    optionsGrid->addWidget(m_coincidenceComboBox,1,1);
    optionsGrid->addWidget(m_deleteFileCheck,2,1);


    //ЗАПУСК
    m_groupOptionsStart = new QGroupBox("Параметры запуска", this);
    optionsLayout->addWidget(m_groupOptionsStart);
    QGridLayout* optionsStartGrid = new QGridLayout(m_groupOptionsStart);

    m_radioBtnSingle = new QRadioButton("Разовый запуск");
    m_radioBtnTimer = new QRadioButton("Запуск по таймеру");

    QLabel* intervalLabel = new QLabel("Интервал опроса(сек): ");
    m_spinInterval = new QSpinBox();
    m_spinInterval->setRange(0,3600);
    m_spinInterval->setEnabled(true);

    optionsStartGrid->addWidget(m_radioBtnSingle,0,0);
    optionsStartGrid->addWidget(m_radioBtnTimer,1,0);
    optionsStartGrid->addWidget(intervalLabel,2,0);
    optionsStartGrid->addWidget(m_spinInterval,2,1);

    // 3 ЧАСТЬ ОКНА_____________
    QHBoxLayout* ListsLayout = new QHBoxLayout();
    mainLayout->addLayout(ListsLayout);

    m_groupListFiles = new QGroupBox("Файлы в директрии", this);
    ListsLayout->addWidget(m_groupListFiles);
    QGridLayout* listFilesGrid = new QGridLayout(m_groupListFiles);
    m_filesList = new QListWidget();
    listFilesGrid->addWidget(m_filesList,1,0);

    m_groupListLogs = new QGroupBox("Логи", this);
    ListsLayout->addWidget(m_groupListLogs);
    QGridLayout* listLogsGrid = new QGridLayout(m_groupListLogs);
    m_logsList = new QListWidget();
    listLogsGrid->addWidget(m_logsList,1,0);

    // 4 ЧАСТЬ ОКНА
    m_groupDown1 = new QGroupBox("Прогресс", this);
    mainLayout->addWidget(m_groupDown1);
    QVBoxLayout* progressLayout = new QVBoxLayout(m_groupDown1);

    m_progressBar = new QProgressBar(m_groupDown1);
    m_progressBar->setValue(50);
    m_progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    progressLayout->addWidget(m_progressBar);
    m_groupDown2 = new QGroupBox("Управление", this);
    mainLayout->addWidget(m_groupDown2);

    QHBoxLayout* btnLayot = new QHBoxLayout(m_groupDown2);
    m_btnStart = new QPushButton("Старт");
    m_btnPause = new QPushButton("Пауза");
    m_btnStop = new QPushButton("Стоп");

    btnLayot->addWidget(m_btnStart);
    btnLayot->addWidget(m_btnPause);
    btnLayot->addWidget(m_btnStop);
}

MainWindow::~MainWindow()
{

}
