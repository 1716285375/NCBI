#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    QMap<QString, QString> args;
    QStringList data;

    int currentLoopIndex = 0;
    int totalLoops = 0;



protected slots:
    bool get_download();
    void get_filename();
    void get_output_folder();
    void clear_stuff();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void check_all();


public:
    QString getMannerText();
    QString getFormatText();

    QStringList read(const QString& filename);

    void startNextProcess(int &begin, int &end, QStringList& data);

private:
    void init();
};

#endif // MAINWINDOW_H
