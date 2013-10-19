#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    manager->append("/Users/samepp/git/FuelPHP_docs_jp/favicon.ico");
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
int n=selections.count();
    for (int i = selections.count() - 1; 0 <= i; --i)
    {
        if (0 == selections.at(i).column())
        {
            int row = selections.at(i).row();
            manager->remove(selections.at(i));
        }
    }
}

void MainWindow::onClearFileList()
{
    manager->clear();
}

void MainWindow::onAbout()
{

}
