#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QCryptographicHash>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QAction>

#include <list>
#include <ctime>
#include <iomanip>
#include <sstream>

using std::time_t, std::tm, std::localtime,
      std::list, std::stringstream;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow (QWidget *parent = nullptr);
        ~MainWindow();
    public slots:
        void countHashes (QTreeWidgetItem *parent, QStringList files, QString pref = "");
    private slots:
        void on_pushButton_clicked();

        void on_pushButton_2_clicked();
        void saveResults();


    private:
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
