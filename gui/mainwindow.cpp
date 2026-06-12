#include "mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(900, 750);
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
    PathGridL->setSpacing(5);

    m_lineSearchPath = new QLineEdit(); // ПУТЬ ВЫБОРКИ
    m_lineSearchPath->setPlaceholderText("Укажите путь директории для выбора файлов...");
    m_btnBrowseSearch = new QPushButton("Обзор");

    m_lineSavePath = new QLineEdit(); // ПУТЬ СОХРАНЕНИЯ
    m_lineSavePath->setPlaceholderText("Укажите путь директории для сохранения...");
    m_btnBrowseSave = new QPushButton("Обзор");

    PathGridL->addWidget(m_lineSearchPath, 0, 0); // УСТАНОВКА ВИДЖЕТОВ НА ГРИД
    PathGridL->addWidget(m_btnBrowseSearch, 0, 1);
    PathGridL->addWidget(m_lineSavePath, 1, 0);
    PathGridL->addWidget(m_btnBrowseSave, 1, 1);
    m_lineSearchPath->setReadOnly(true);
    m_lineSavePath->setReadOnly(true);

    // 2 ЧАСТЬ ОКНА_____________
    QHBoxLayout* optionsLayout = new QHBoxLayout();
    mainLayout->addLayout(optionsLayout);

    m_groupOptions = new QGroupBox("Параметры шифрования");
    QVBoxLayout* optLeftLayout = new QVBoxLayout(m_groupOptions);
    m_lineHex = new QLineEdit();
    m_lineHex->setPlaceholderText("Введите Ключ (HEX, например: A1B2C3D4)");
    m_coincidenceComboBox = new QComboBox();
    m_coincidenceComboBox->addItems(QStringList() << "Пропустить" << "Перезаписать" << "Счетчик");
    m_deleteFileCheck = new QCheckBox("Удалять оригиналы файлов");
    optLeftLayout->addWidget(new QLabel("Ключ XOR (Hex):"));
    optLeftLayout->addWidget(m_lineHex);
    optLeftLayout->addWidget(new QLabel("Стратегия при совпадении имен:"));
    optLeftLayout->addWidget(m_coincidenceComboBox);
    optLeftLayout->addWidget(m_deleteFileCheck);
    optionsLayout->addWidget(m_groupOptions);

    m_groupOptionsStart = new QGroupBox("Режим запуска");
    QVBoxLayout* optRightLayout = new QVBoxLayout(m_groupOptionsStart);
    m_radioBtnSingle = new QRadioButton("Одиночный запуск");
    m_radioBtnTimer = new QRadioButton("По таймеру (следить за папкой)");
    m_radioBtnSingle->setChecked(true);
    m_spinInterval = new QSpinBox();
    m_spinInterval->setRange(1, 3600);
    m_spinInterval->setValue(5);
    m_spinInterval->setSuffix(" сек");
    optRightLayout->addWidget(m_radioBtnSingle);
    optRightLayout->addWidget(m_radioBtnTimer);
    optRightLayout->addWidget(new QLabel("Интервал проверки:"));
    optRightLayout->addWidget(m_spinInterval);
    optionsLayout->addWidget(m_groupOptionsStart);

    // 3 ЧАСТЬ ОКНА_____________
    QHBoxLayout* listsLayout = new QHBoxLayout();
    mainLayout->addLayout(listsLayout);

    m_groupListFiles = new QGroupBox("Список файлов");
    QVBoxLayout* listFilesLayout = new QVBoxLayout(m_groupListFiles);
    m_comboTypeFiles = new QComboBox();
    m_comboTypeFiles->addItems(QStringList() << "Все файлы (*.*)" << "Текстовые (*.txt)" << "Изображения (*.png;*.jpg)");
    m_filesList = new QListWidget();
    m_separatelyCheck = new QCheckBox("Выбирать файлы по отдельности");
    listFilesLayout->addWidget(m_comboTypeFiles);
    listFilesLayout->addWidget(m_filesList);
    listFilesLayout->addWidget(m_separatelyCheck);
    listsLayout->addWidget(m_groupListFiles);

    m_groupListLogs = new QGroupBox("Логи работы");
    QVBoxLayout* listLogsLayout = new QVBoxLayout(m_groupListLogs);
    m_logsList = new QListWidget();
    listLogsLayout->addWidget(m_logsList);
    listsLayout->addWidget(m_groupListLogs);

    // 4 ЧАСТЬ ОКНА_____________
    m_groupDown1 = new QGroupBox("Прогресс");
    QVBoxLayout* progressLayout = new QVBoxLayout(m_groupDown1);
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    progressLayout->addWidget(m_progressBar);
    mainLayout->addWidget(m_groupDown1);

    m_groupDown2 = new QGroupBox("Управление");
    QHBoxLayout* buttonsLayout = new QHBoxLayout(m_groupDown2);
    m_btnStart = new QPushButton("Старт");
    m_btnPause = new QPushButton("Пауза");
    m_btnStop = new QPushButton("Стоп");
    m_btnPause->setEnabled(false);
    m_btnStop->setEnabled(false);
    buttonsLayout->addWidget(m_btnStart);
    buttonsLayout->addWidget(m_btnPause);
    buttonsLayout->addWidget(m_btnStop);
    mainLayout->addWidget(m_groupDown2);

    m_fileWatcher = new QFileSystemWatcher(this);
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(2000);

    setupConnections();
}

MainWindow::~MainWindow()
{
    onStop();
}

void MainWindow::setupConnections()
{
    connect(m_btnBrowseSearch, &QPushButton::clicked, this, &MainWindow::onBrowseSearch);
    connect(m_btnBrowseSave, &QPushButton::clicked, this, &MainWindow::onBrowseSave);
    connect(m_btnStart, &QPushButton::clicked, this, &MainWindow::onStart);
    connect(m_btnPause, &QPushButton::clicked, this, &MainWindow::onPauseToggle);
    connect(m_btnStop, &QPushButton::clicked, this, &MainWindow::onStop);
    connect(m_comboTypeFiles, &QComboBox::currentTextChanged, this, &MainWindow::updateFileList);
    connect(m_separatelyCheck, &QCheckBox::toggled, this, &MainWindow::updateFileList);
    connect(m_fileWatcher, &QFileSystemWatcher::directoryChanged, this, &MainWindow::updateFileList);
    connect(m_refreshTimer, &QTimer::timeout, this, &MainWindow::updateFileList);
}

void MainWindow::onBrowseSearch()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите директорию поиска");
    if (!dir.isEmpty()) {
        m_lineSearchPath->setText(dir);
        setupWatcher(dir);
        updateFileList();
        m_refreshTimer->start();
    }
}

void MainWindow::onBrowseSave()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите директорию сохранения");
    if (!dir.isEmpty()) {
        m_lineSavePath->setText(dir);
    }
}

void MainWindow::onStart()
{
    if (m_lineSearchPath->text().isEmpty() || m_lineSavePath->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Укажите пути поиска и сохранения!");
        return;
    }

    bool ok;
    QString hexStr = m_lineHex->text().trimmed();
    if (hexStr.isEmpty()) hexStr = "0";
    uint64_t mask = hexStr.toULongLong(&ok, 16);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Некорректный HEX-ключ!");
        return;
    }

    QStringList selectedFiles;
    if (m_separatelyCheck->isChecked()) {
        for (int i = 0; i < m_filesList->count(); ++i) {
            QListWidgetItem* item = m_filesList->item(i);
            if (item->checkState() == Qt::Checked) {
                selectedFiles.append(item->text());
            }
        }
        if (selectedFiles.isEmpty()) {
            QMessageBox::warning(this, "Внимание", "Вы не выбрали ни одного файла галочкой!");
            return;
        }
    } else {
        for (int i = 0; i < m_filesList->count(); ++i) {
            selectedFiles.append(m_filesList->item(i)->text());
        }
    }

    config.inputFiles = selectedFiles;
    config.inputPath = m_lineSearchPath->text().toStdString();
    config.outputPath = m_lineSavePath->text().toStdString();
    config.mask = mask;
    config.isTimerMode = m_radioBtnTimer->isChecked();
    config.intervalS = m_spinInterval->value();
    config.conflictStrategy = m_coincidenceComboBox->currentIndex();
    config.deleteOriginals = m_deleteFileCheck->isChecked();

    m_workerThread = new QThread();
    worker = new XorWorker();
    worker->moveToThread(m_workerThread);
    TaskConfig currentConfig = this->config;
    XorWorker* localWorker = worker;

    connect(m_workerThread, &QThread::started, localWorker, [localWorker, currentConfig]() {
        localWorker->startProcessing(currentConfig);
    });

    connect(worker, &XorWorker::logMessage, this, &MainWindow::onLogMessage);
    connect(worker, &XorWorker::progressUpdated, this, &MainWindow::onProgressUpdated);
    connect(worker, &XorWorker::finished, this, &MainWindow::onWorkerFinished);
    connect(this, &MainWindow::filesAdded, worker, &XorWorker::enqueueFiles);

    m_workerThread->start();
    m_isProcessing = true;
    m_isPaused = false;
    m_btnStart->setEnabled(false);
    m_btnPause->setEnabled(true);
    m_btnPause->setText("Пауза");
    m_btnStop->setEnabled(true);

    m_groupPaths->setEnabled(false);
    m_groupOptions->setEnabled(false);
    m_groupOptionsStart->setEnabled(false);
    m_separatelyCheck->setEnabled(false);

    m_progressBar->setValue(0);
    m_logsList->addItem("--- Сессия обработки запущена ---");
}

void MainWindow::onPauseToggle()
{
    if (!m_isProcessing || !worker) return;

    m_isPaused = !m_isPaused;

    if (m_isPaused) {
        m_btnPause->setText("Возобновить");
    } else {
        m_btnPause->setText("Пауза");
    }
    worker->setPaused(m_isPaused);

    qDebug() << "Главный поток: пауза переключена в" << m_isPaused;
}

void MainWindow::onStop()
{
    if (!m_isProcessing) return;

    if (worker) {
        worker->setPaused(false);
    }

    if (m_workerThread) {
        m_workerThread->quit();
        if (!m_workerThread->wait(2000)) {
            m_workerThread->terminate();
        }
        delete m_workerThread;
        m_workerThread = nullptr;
    }
    if (worker) {
        delete worker;
        worker = nullptr;
    }

    m_isProcessing = false;
    onWorkerFinished();
}

void MainWindow::onWorkerFinished()
{
    m_isProcessing = false;
    m_isPaused = false;

    m_btnStart->setEnabled(true);
    m_btnPause->setEnabled(false);
    m_btnPause->setText("Пауза");
    m_btnStop->setEnabled(false);

    m_groupPaths->setEnabled(true);
    m_groupOptions->setEnabled(true);
    m_groupOptionsStart->setEnabled(true);
    m_separatelyCheck->setEnabled(true);

    m_logsList->addItem("--- Работа завершена/остановлена ---");
}

void MainWindow::onLogMessage(const QString &message)
{
    m_logsList->addItem(message);
    m_logsList->scrollToBottom();
}

void MainWindow::onProgressUpdated(int value)
{
    m_progressBar->setValue(value);
}

void MainWindow::setupWatcher(const QString &path)
{
    if (!m_fileWatcher) return;
    QStringList paths = m_fileWatcher->directories();
    if (!paths.isEmpty()) {
        m_fileWatcher->removePaths(paths);
    }
    m_fileWatcher->addPath(path);
}

void MainWindow::refreshComboBoxFiles(const QString &path) {
    QString currentFilter = m_comboTypeFiles->currentText();

    m_comboTypeFiles->blockSignals(true);
    m_comboTypeFiles->clear();
    m_comboTypeFiles->addItem("Все файлы (*.*)");

    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    QSet<QString> extensions;

    for (const QString &file : std::as_const(files)) {
        QFileInfo fi(file);
        QString ext = fi.suffix().toLower();
        if (!ext.isEmpty()) {
            extensions.insert(ext);
        }
    }

    for (const QString &ext : std::as_const(extensions)) {
        m_comboTypeFiles->addItem(QString("Файлы (*.%1)").arg(ext));
    }
    int index = m_comboTypeFiles->findText(currentFilter);
    if (index != -1) {
        m_comboTypeFiles->setCurrentIndex(index);
    } else {
        m_comboTypeFiles->setCurrentIndex(0);
    }
    m_comboTypeFiles->blockSignals(false);
}

void MainWindow::updateFileList() {
    QString path = m_lineSearchPath->text();
    if (path.isEmpty() || !QDir(path).exists()) return;
    QSet<QString> previouslyChecked;
    for (int i = 0; i < m_filesList->count(); ++i) {
        QListWidgetItem* item = m_filesList->item(i);
        if (item->checkState() == Qt::Checked) {
            previouslyChecked.insert(item->text());
        }
    }
    refreshComboBoxFiles(path);
    QDir dir(path);
    QString currentFilter = m_comboTypeFiles->currentText();
    QString mask = "*";
    if (currentFilter != "Все файлы (*.*)") {
        int start = currentFilter.indexOf("(*.") + 3;
        int end = currentFilter.indexOf(")");
        if (start > 2 && end > start) {
            mask = "*." + currentFilter.mid(start, end - start);
        }
    }
    dir.setNameFilters(QStringList() << mask);
    QStringList files = dir.entryList(QDir::Files | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);
    m_filesList->clear();
    bool isSeparatelyMode = m_separatelyCheck->isChecked();

    for (const QString &fileName : std::as_const(files)) {
        QListWidgetItem* item = new QListWidgetItem(fileName);
        if (isSeparatelyMode) {
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            if (previouslyChecked.contains(fileName)) {
                item->setCheckState(Qt::Checked);
            } else {
                item->setCheckState(Qt::Unchecked);
            }
        }
        m_filesList->addItem(item);
    }

    if (m_isProcessing) {
        emit filesAdded(files);
    }
}