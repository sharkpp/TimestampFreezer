#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(new ItemManager)
{
    ui->setupUi(this);

//    manager->append("/Users/samepp/git/FuelPHP_docs_jp/index.html");
//    manager->append("/Users/samepp/git/FuelPHP_docs_jp/CREDITS.md");

    ui->listTargetFile->setModel(manager);
    ui->listTargetFile->setColumnWidth(1, ui->listTargetFile->fontMetrics().width("9999-99-99 99:99:99"));
    ui->listTargetFile->setColumnWidth(2, ui->listTargetFile->fontMetrics().width("9999-99-99 99:99:99"));
    ui->listTargetFile->setColumnWidth(3, ui->listTargetFile->fontMetrics().width("9999-99-99 99:99:99"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!manager->empty())
    {
        int ret = QMessageBox::question(
                    this, windowTitle(),
                    tr("Are you sure you want to exit the application?\n"
                       "Time stamp will be returned to the original all."),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
        if (QMessageBox::No == ret)
        {
            event->ignore();
            return;
        }
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if( mimeData->hasUrls() &&
        !mimeData->urls().empty() )
    {
        QList<QUrl> urls = mimeData->urls();
        for(int i = 0; i < urls.size(); i++) {
            manager->append(urls.at(i).toLocalFile());
        }
    }
}

void MainWindow::onAppendFile()
{
    QFileDialog dlg(this);
    dlg.setFileMode(QFileDialog::ExistingFiles);
    //dlg.setOption(QFileDialog::DontUseNativeDialog, false);
    if (dlg.exec())
    {
        QStringList files = dlg.selectedFiles();
        for(int i = 0; i < files.size(); i++) {
            manager->append(files.at(i));
        }
    }
}

void MainWindow::onRemoveFile()
{
    QItemSelectionModel* model = ui->listTargetFile->selectionModel();

    if (!model->hasSelection()) {
        return;
    }

    QModelIndexList selections = model->selectedIndexes();
    if (selections.isEmpty()) {
        return;
    }

    for (int i = selections.count() - 1; 0 <= i; --i)
    {
        if (0 == selections.at(i).column())
        {
            manager->remove(selections.at(i));
        }
    }
}

void MainWindow::onClearFileList()
{
    int ret = QMessageBox::question(
                this, windowTitle(),
                tr("Are you sure you want to exclude from the list of all files?"),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);
    if (QMessageBox::Yes == ret)
    {
        manager->clear();
    }
}

void MainWindow::onAbout()
{

}
