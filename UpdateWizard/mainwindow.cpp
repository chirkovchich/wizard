#include "mainwindow.h"
#include "treemodel.h"

#include <QMessageBox>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : RibbonMainWindow(parent),
    panelTasks(Q_NULL),
    panelProtocol(Q_NULL)

{
    tabDialog = new TabDialog(this);
    createRibbon();
    managerPanels = new DockPanelManager(this);

    createPanelTasks();
    createPanelProtocol();
    managerPanels->insertDockPanel(panelProtocol,Qtitan::InsideDockPanelArea, panelTasks);

    panelTasks->closePanel();
    panelProtocol->closePanel();

    connect(managerPanels, SIGNAL(aboutToClose(DockPanelBase*, bool&)),
            this, SLOT(closedPanels(DockPanelBase*,bool&)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::createRibbon()
{

    setWindowTitle("UpdateWIZARD");
    setWindowIcon(QIcon(QStringLiteral(":/res/UpdateWIZARD.ico")));

    resize(1200,800);

    QWidget *baseLayoutWidget = new QWidget();
    QVBoxLayout *baseLayoutV = new QVBoxLayout;

    baseLayoutV->setContentsMargins(0, 0, 0, 0);
    baseLayoutV->setSpacing(0);

    QTabWidget *tab = new QTabWidget(this);

    tab->setStyleSheet("QTabBar::tab { height: 40px; }");
    tab->setIconSize({30,30});
    tab->addTab(createTreeDb(),QIcon(QStringLiteral(":/res/DB.svg")), "Базы данных");
    tab->addTab(createTreeProgram(),QIcon(QStringLiteral(":/res/Program.svg")), "Программные продукты");

    baseLayoutV->addWidget(tab);
    baseLayoutWidget->setLayout(baseLayoutV);

    setCentralWidget(baseLayoutWidget);

    ribbonBar()->setFrameThemeEnabled();
    ribbonBar()->setSimplifiedModeEnabled(false);

    RibbonPage *pageMain = ribbonBar() ->addPage("&Главная");
    RibbonGroup *groupData = pageMain->addGroup("&Данные");

    updateInfoAction = groupData->addAction(QIcon(QStringLiteral(":/res/Update.svg")),
                                         "&Обновить информацию", Qt::ToolButtonTextUnderIcon);
    updateInfoAction->setToolTip("Обновить информацию по\nдобавленным серийным\nномерам CTRL+F5");
    connect(updateInfoAction, SIGNAL(triggered()), this, SLOT(notImplemented()));

    QMenu *menuPopup = new QMenu(ribbonBar());

    QAction *action = menuPopup->addAction(QIcon(QStringLiteral(":/res/DownloadDB.svg")), "&Скачать только базы");
    action->setDisabled(true);
    action = menuPopup->addAction(QIcon(QStringLiteral(":/res/DownloadLic.svg")), "&Скачать только лицензии");
    action->setDisabled(true);

    QAction *downloadAction = groupData->addAction(QIcon(QStringLiteral(":/res/FileDownloading.svg")),
                                                   "&Скачать", Qt::ToolButtonTextUnderIcon, menuPopup);
    connect(downloadAction, SIGNAL(triggered()), this, SLOT(notImplemented()));
    downloadAction->setToolTip("Загрузить выбранные данные");

    RibbonGroup *groupSettings = pageMain->addGroup("&Настройки");
    settingsAction = groupSettings->addAction(QIcon(QStringLiteral(":/res/Settings.svg")),
                                          "&Настройки", Qt::ToolButtonTextUnderIcon);
    settingsAction->setToolTip("Открыть диалог настроек\nприложения");
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(viewTabSettings()));

    RibbonGroup *groupView = pageMain->addGroup("&Вид");
    taskListAction = groupView->addAction(QIcon(QStringLiteral(":/res/TaskList.svg")),
                                                "&Список\nзадач", Qt::ToolButtonTextUnderIcon);
    taskListAction->setToolTip("Спрятать-показать список\nзадач");
    taskListAction->setCheckable(true);
    connect(taskListAction, SIGNAL(triggered()), this, SLOT(triggeredPanelTasks()));

    protocolAction = groupView->addAction(QIcon(QStringLiteral(":/res/Log.svg")),
                                                "&Протокол\nработы", Qt::ToolButtonTextUnderIcon);
    protocolAction->setToolTip("Спрятать-показать протокол\nработы");
    protocolAction->setCheckable(true);
    connect(protocolAction, SIGNAL(triggered()), this, SLOT(triggeredPanelProtocol()));
}

void MainWindow::createPanelTasks()
{
    panelTasks = managerPanels->addDockPanel("Список задач", Qtitan::BottomDockPanelArea);
    QFont font = panelTasks->font();
    font.setPixelSize(18);
    panelTasks->titleBar()->setFont(font);
    panelTasks->setIcon(QIcon(QStringLiteral(":/res/TaskList.svg")));

    QWidget *baseLayoutWidget = new QWidget();
    QVBoxLayout *baseLayoutV = new QVBoxLayout;
    QHBoxLayout *baseLayoutH = new QHBoxLayout;

    QPushButton *btnStop = new QPushButton(this);
    btnStop->setIcon(QIcon(QStringLiteral(":/res/Stop.svg")));
    btnStop->setIconSize({32,32});
    btnStop->setFixedSize(40,40);
    btnStop->setDisabled(true);

    QPushButton *btnRemove = new QPushButton(this);
    btnRemove->setIcon(QIcon(QStringLiteral(":/res/Remove.svg")));
    btnRemove->setIconSize({32,32});
    btnRemove->setFixedSize(40,40);
    btnRemove->setDisabled(true);

    QPushButton *btnChange = new QPushButton(this);
    btnChange->setIcon(QIcon(QStringLiteral(":/res/Change.svg")));
    btnChange->setIconSize({32,32});
    btnChange->setFixedSize(40,40);
    btnChange->setDisabled(true);

    baseLayoutH->setContentsMargins(10, 10,10, 10);
    baseLayoutH->setSpacing(10);
    baseLayoutH->addWidget(btnStop);
    baseLayoutH->addWidget(btnRemove);
    baseLayoutH->addWidget(btnChange);
    baseLayoutH->addStretch(1);

    baseLayoutV->setContentsMargins(0, 0, 0, 0);
    baseLayoutV->setSpacing(0);
    baseLayoutV->addLayout(baseLayoutH);
    baseLayoutV->addWidget(createGridTasks());

    baseLayoutWidget->setLayout(baseLayoutV);
    panelTasks->setWidget(baseLayoutWidget);
}

void MainWindow::createPanelProtocol()
{
    panelProtocol = managerPanels->addDockPanel("Протокол работы", Qtitan::BottomDockPanelArea);
    QFont font = panelProtocol->font();
    font.setPixelSize(18);
    panelProtocol->titleBar()->setFont(font);
    panelProtocol->setIcon(QIcon(QStringLiteral(":/res/Log.svg")));
    panelProtocol->setWidget(createGridProtocol());
}

Grid* MainWindow::createGridTasks()
{
    Grid *grid = new Grid();
    grid->setViewType(Grid::TableView);
    GridTableView *view = grid->view<GridTableView>();

    QStandardItemModel *model=  new QStandardItemModel(0, 3);

    model->setHeaderData(0, Qt::Horizontal, "Задача");
    model->setHeaderData(1, Qt::Horizontal, "Файл");
    model->setHeaderData(2, Qt::Horizontal, "Статус");
    model->insertRow(0);
    model->insertRow(1);

    setOptionsViewGrid(view, model);
    return grid;
}

Grid* MainWindow::createGridProtocol()
{
    Grid *grid = new Grid();
    grid->setViewType(Qtitan::Grid::TableView);
    GridTableView *view = grid->view<GridTableView>();

    QStandardItemModel *model=  new QStandardItemModel(0, 4);

    model->setHeaderData(0, Qt::Horizontal, "Время");
    model->setHeaderData(1, Qt::Horizontal, "Тип");
    model->setHeaderData(2, Qt::Horizontal, "Сообщение");
    model->setHeaderData(3, Qt::Horizontal, " ");
    model->insertRow(0);
    model->insertRow(1);

    setOptionsViewGrid(view, model);
    return grid;
}

void MainWindow::setOptionsViewGrid(GridTableView *view, QStandardItemModel *model)
{
    view->beginUpdate();
    view->setModel(model);
    view->options().setMainMenuDisabled(true);
    view->options().setFindEnabled(false);
    view->options().setGridLines(Qtitan::LinesNone);
    view->options().setAllowFilterEditor(false);
    view->options().setFilterEnabled(false);
    view->options().setDragEnabled(false);
    view->options().setFieldChooserEnabled(false);
    view->options().setGroupsHeader(false);
    view->options().setRowRemoveEnabled(false);
    view->options().setSortEnabled(false);
    view->options().setColumnHidingEnabled(false);
    view->options().setColumnMovingEnabled(false);

    view->tableOptions().setColumnsQuickCustomization(false);
    view->deselectAll();
    view->tableOptions().setColumnAutoWidth(true);

    for (int i = 0; i < view->getColumnCount(); ++i){
        Qtitan::GridTableColumn *column = (Qtitan::GridTableColumn *)view->getColumnByModelColumn(i);
        column->setTextAlignment(Qt::AlignLeft);
        column->editorRepository()->setEditable(false);
        column->editorRepository()->setAlignment(Qt::AlignLeft);
    }
    view->endUpdate();
}

TreeGrid* MainWindow::createTreeDb()
{
    Grid::loadTranslation();
    TreeGrid *grid = new TreeGrid();
    grid->setViewType(Qtitan::TreeGrid::TreeView);
    GridTreeView *view = grid->view<GridTreeView>();

    const QStringList headers({"Наименование базы", "Регион", "Дата обновления", "Размер"});

    QFile file(":/res/primerDB.txt");

    file.open(QIODevice::ReadOnly);
    TreeModel *model = new TreeModel(headers, file.readAll());
    file.close();

    view->beginUpdate();
    view->options().setGridLines(Qtitan::LinesBoth);
    view->setModel(model);
    view->expandToLevel(3);
    view->endUpdate();
    view->bestFit(Qtitan::FitToHeaderAndContent);
    return grid;
}

TreeGrid* MainWindow::createTreeProgram()
{
    Grid::loadTranslation();
    TreeGrid *grid = new TreeGrid();
    grid->setViewType(Qtitan::TreeGrid::TreeView);
    GridTreeView *view = grid->view<GridTreeView>();

    const QStringList headers({"Наименование", "Описание", "Размер", "Серийный номер", "Организация"});

    QFile file(":/res/primerProgram.txt");

    file.open(QIODevice::ReadOnly);
    TreeModel *model = new TreeModel(headers, file.readAll());
    file.close();

    view->beginUpdate();
    view->options().setGridLines(Qtitan::LinesBoth);
    view->setModel(model);
    view->expandToLevel(3);
    view->endUpdate();
    view->bestFit(Qtitan::FitToHeaderAndContent);
    return grid;
}

void MainWindow::triggeredPanelTasks()
{

    if(panelTasks->isClosed()){
        panelTasks->showPanel();
        taskListAction->setChecked(true);
    }
    else {
        panelTasks->closePanel();
        taskListAction->setChecked(false);
    }
}

void MainWindow::triggeredPanelProtocol()
{

    if(panelProtocol->isClosed()){
        panelProtocol->showPanel();
        protocolAction->setChecked(true);
    }
    else {
        panelProtocol->closePanel();
        protocolAction->setChecked(false);
    }
}

void MainWindow::closedPanels(DockPanelBase *panel, bool &handled)
{
    if (panel == panelTasks)
    {
        taskListAction->setChecked(false);
    }
    if (panel == panelProtocol){
        protocolAction->setChecked(false);
    }

}

void MainWindow::notImplemented()
{
    QMessageBox messageBox(QMessageBox::Warning, windowTitle(),
                           QString(), QMessageBox::Ok, this);
    messageBox.setText(QStringLiteral("Не релизовано"));
    messageBox.exec();
}

void MainWindow::viewTabSettings()
{
    tabDialog->setFixedSize(700, 600);
    tabDialog->exec();
}


