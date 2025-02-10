#include "tabdialog.h"

TabDialog::TabDialog(QWidget *parent)
    : QDialog(parent)
{

    tabWidget = new QTabWidget;
    tabWidget->addTab(new SerialNumberTab(), "Серийные номера и загрузка");
    tabWidget->addTab(new QLabel("Не реализовано", tabWidget), "Настройки подключения");

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle("Настройки");
    QPixmap pixmap{1, 1};
    pixmap.fill(Qt::transparent);
    setWindowIcon(QIcon{pixmap});
}

SerialNumberTab::SerialNumberTab(QWidget *parent)
    : QWidget(parent)
{
    QLabel *folderNameLabel = new QLabel("Папка по умолчанию:");
    QPushButton *btnChange = new QPushButton(this);
    btnChange->setIcon(QIcon(QStringLiteral(":/res/Change.svg")));
    btnChange->setIconSize({16,16});
    btnChange->setFixedSize(20,20);
    folderNameEdit = new QLineEdit();
    connect(btnChange, SIGNAL(clicked()), this, SLOT(slotExistingDirectory()));

    QCheckBox *formed = new QCheckBox("Формировать пакет обновлений");
    formed->setChecked(true);

    QPushButton *btnAdd = new QPushButton(this);
    btnAdd->setIcon(QIcon(QStringLiteral(":/res/Add.svg")));
    btnAdd->setIconSize({32,32});
    btnAdd->setFixedSize(40,40);

    QPushButton *btnRemove = new QPushButton(this);
    btnRemove->setIcon(QIcon(QStringLiteral(":/res/Remove.svg")));
    btnRemove->setIconSize({32,32});
    btnRemove->setFixedSize(40,40);

    QPushButton *btnRecognize = new QPushButton(this);
    btnRecognize->setText("Определить серийный номер");
    btnRecognize->setFixedSize(200,40);

    QHBoxLayout *folderLayout = new QHBoxLayout;
    folderLayout->addWidget(folderNameEdit);
    folderLayout->addWidget(btnChange);

    QGroupBox *serialNumberGroup = new QGroupBox("Серийные номера:");
    QVBoxLayout *serialLayoutV = new QVBoxLayout;
    QHBoxLayout *serialLayoutH = new QHBoxLayout;

    serialLayoutH->setContentsMargins(10, 10,10, 10);
    serialLayoutH->setSpacing(10);
    serialLayoutH->addWidget(btnAdd);
    serialLayoutH->addWidget(btnRemove);
    serialLayoutH->addStretch(1);
    serialLayoutH->addWidget(btnRecognize);

    serialLayoutV->addLayout(serialLayoutH);
    serialLayoutV->addWidget(createGridSerial());
    serialNumberGroup->setLayout(serialLayoutV);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(20, 5, 5, 5);
    mainLayout->addWidget(folderNameLabel);
    mainLayout->addLayout(folderLayout);
    mainLayout->addWidget(formed);
    mainLayout->addWidget(serialNumberGroup);

    setLayout(mainLayout);
}

Grid *SerialNumberTab::createGridSerial()
{
    Grid *grid = new Grid();
    grid->setViewType(Qtitan::Grid::TableView);
    GridTableView *view = grid->view<GridTableView>();

    QStandardItemModel *model=  new QStandardItemModel(0, 4);

    model->setHeaderData(0, Qt::Horizontal, "Серийный номер");
    model->setHeaderData(1, Qt::Horizontal, "ID ключа");
    model->setHeaderData(2, Qt::Horizontal, "Имя ключа");
    model->setHeaderData(3, Qt::Horizontal, " ");
    model->insertRow(0);
    model->insertRow(1);

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
        column->editorRepository()->setEditable(true);
        column->editorRepository()->setAlignment(Qt::AlignLeft);
    }
    view->endUpdate();

    return grid;
}

void SerialNumberTab::slotExistingDirectory()
{
    QString str = QFileDialog::getExistingDirectory(0, "Выбор папки по умолчанию", "");
    folderNameEdit->setText(str);
}
