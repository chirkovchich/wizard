#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <DevMachines/QtitanStyle>
#include <DevMachines/QtitanRibbon>
#include <DevMachines/QtitanDocking>
#include <DevMachines/QtitanGrid>
#include <QStandardItemModel>
#include <QFileInfo>
#include "tabdialog.h"

class MainWindow : public RibbonMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULL);
    ~MainWindow();

public Q_SLOTS:
    void triggeredPanelTasks();
    void triggeredPanelProtocol();
    void closedPanels(DockPanelBase *panel, bool &handled);
    void notImplemented();
    void viewTabSettings();

private:
    void createRibbon();
    void createPanelTasks();
    void createPanelProtocol();
    Grid *createGridTasks();
    Grid *createGridProtocol();
    void setOptionsViewGrid(GridTableView *view, QStandardItemModel *model);

    TreeGrid *createTreeDb();
    TreeGrid *createTreeProgram();

private:
    DockPanelManager *managerPanels;
    DockWidgetPanel *panelTasks;
    DockWidgetPanel *panelProtocol;
    TabDialog *tabDialog;

protected:
    QAction *updateInfoAction;
    QAction *settingsAction;
    QAction *taskListAction;
    QAction *protocolAction;

};
#endif // MAINWINDOW_H
