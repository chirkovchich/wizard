#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <DevMachines/QtitanStyle>
#include <DevMachines/QtitanGrid>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

class SerialNumberTab : public QWidget
{
    Q_OBJECT

public:
    explicit SerialNumberTab(QWidget *parent = nullptr);
    Grid *createGridSerial();

public Q_SLOTS:
    void slotExistingDirectory();
private:
    QLineEdit *folderNameEdit;
};

class TabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TabDialog(QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;

};

#endif // TABDIALOG_H
