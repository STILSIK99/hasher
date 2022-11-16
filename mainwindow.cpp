#include "mainwindow.h"
#include "ui_mainwindow.h"

QString getHashSum (QString path, QCryptographicHash::Algorithm algorithm)
{
    QString pref;
    switch (algorithm) {
        case QCryptographicHash::Md5:
            pref = "MD5: ";
            break;
        case QCryptographicHash::Sha1:
            pref = "SHA1: ";
            break;
        case QCryptographicHash::Sha256:
            pref = "SHA256: ";
            break;
        default:
            pref = "Unknown: ";
    }
    try {
        QFile file (path);
        if (!file.open (QIODevice::ReadOnly)) throw;
        QByteArray fileData = file.readAll();
        file.close();
        QByteArray hashData = QCryptographicHash::hash (fileData, algorithm);
        return pref + hashData.toHex();
    } catch (...) {
    
    }
    return "";
}


MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent)
    , ui (new Ui::MainWindow)
{
    ui->setupUi (this);
    connect (ui->action, &QAction::triggered, this, &MainWindow::saveResults);
}

void MainWindow::saveResults()
{
    QString path = QFileDialog::getSaveFileName (this, "Выберите файл для сохранения:", QDir::currentPath(), "Text Files | (*.txt)");
    if (path == "") return ;
    QFile file (path);
    file.open (QIODevice::WriteOnly);
    if (!file.isOpen())
        return;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        auto top = ui->treeWidget->topLevelItem (i);
        file.write (top->text (0).toUtf8() + "\n");
        for (int j = 0; j < top->childCount(); ++j) {
            auto _nFile = top->child (j);
            file.write ("\t" + _nFile->text (1).toUtf8() + "\n");
            for (int k = 0; k < _nFile->childCount(); ++k) {
                file.write ("\t\t" + _nFile->child (k)->text (2).toUtf8() + "\n");
            }
        }
        file.write ("\n");
    }
    file.close();
    system ("notepad " + path.toUtf8());
}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory (this, "Выберите директорию:", QDir::currentPath());
    if (dir == "") return;
    time_t start = time (nullptr);
    stringstream ss;
    tm local = *std::localtime (&start);
    ss << std::put_time (&local, " %H:%M:%S %d.%m.%Y");
    QStringList files;
    QStringList mask;
    if (ui->checkBox_4->isChecked()) mask << ui->lineEdit->text();
    else mask << "*";
    QDirIterator it (dir, mask, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        auto node = it.next();
        files << node.right (node.length() - dir.length() - 1);
    }
    auto parent = new QTreeWidgetItem();
    parent->setText (0, dir + QString::fromStdString (ss.str()));
    countHashes (parent, files, dir);
}

void MainWindow::countHashes (QTreeWidgetItem *parent, QStringList files, QString pref)
{
    list<QCryptographicHash::Algorithm> algos;
    if (ui->checkBox->isChecked()) algos.emplace_back (QCryptographicHash::Md5);
    if (ui->checkBox_2->isChecked()) algos.emplace_back (QCryptographicHash::Sha1);
    if (ui->checkBox_3->isChecked()) algos.emplace_back (QCryptographicHash::Sha256);
    ui->treeWidget->addTopLevelItem (parent);
    for (auto file : files) {
        auto child = new QTreeWidgetItem (parent);
        child->setText (1, file);
        for (auto algo : algos) {
            auto hash = new QTreeWidgetItem (child);
            hash->setText (2, getHashSum (pref + ((pref == "") ? "" : "/") + file, algo));
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    auto files = QFileDialog::getOpenFileNames (this, "Выберите файлы:", QDir::currentPath());
    if (files == QStringList()) return;

    time_t start = time (nullptr);
    stringstream ss;
    tm local = *std::localtime (&start);
    ss << std::put_time (&local, " %H:%M:%S %d.%m.%Y");
    auto parent = new QTreeWidgetItem();
    parent->setText (0, "Files " + QString::fromStdString (ss.str()));
    countHashes (parent, files);
}
































