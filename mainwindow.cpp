#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QStringBuilder>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->button_download, SIGNAL(clicked()), this, SLOT(get_download()));
    connect(ui->button_clear, SIGNAL(clicked()), this, SLOT(get_download()));
    connect(ui->button_input, SIGNAL(clicked()), this, SLOT(get_filename()));
    connect(ui->button_output, SIGNAL(clicked()), this, SLOT(get_output_folder()));
    connect(ui->button_clear, SIGNAL(clicked()), this, SLOT(clear_stuff()));

    connect(ui->check_all, SIGNAL(clicked()), this, SLOT(check_all()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化
void MainWindow::init()
{

}

// 下载数据
bool MainWindow::get_download()
{
    QString filename_path = ui->label_input->text();
    QString output_dir = ui->label_output->text();
    if (filename_path == QString("...") || output_dir == QString("..."))
    {

        // qDebug() << "select your filename or output dir";
        QMessageBox msg( QMessageBox::Warning,
                        QString("警告"),
                        QString("请选择文件或文件目录"),
                        QMessageBox::Ok,
                        this);
        msg.exec();
    }
    else
    {
        ui->button_download->setEnabled(false);
        ui->button_download->setText(QString("下载中"));
        ui->progressbar_download->setValue(0);

        args.insert("group", ui->combo_group->currentText());
        args.insert("section", ui->combo_section->currentText());
        args.insert("format", getFormatText());
        args.insert("manner", getMannerText());
        args.insert("output", ui->label_output->text());

        // 读取文件数据
        QString filename = ui->label_input->text();
        data = read(filename);
        totalLoops = data.length();

        // 打印下载信息
        QString info("--------------------------开始下载------------------------------");
        ui->text_browser_info->setText(info + "\n\n\n");

        // qDebug() << args["manner"];
        // qDebug() << data[0];

        startNextProcess(currentLoopIndex, totalLoops, data);
    }
    return true;
}

// 选择下载文件
void MainWindow::get_filename()
{
    QString curDir=QDir::currentPath();
    QString aFile=QFileDialog::getOpenFileName(this,"打开一个文件",curDir,"txt(*.txt)");
    ui->label_input->setText(aFile);
}

// 选择输出目录
void MainWindow::get_output_folder()
{
    QString curDir=QDir::currentPath();
    QString aDir=QFileDialog::getExistingDirectory(this,"选择一个目录",curDir,QFileDialog::ShowDirsOnly);
    ui->label_output->setText(aDir);
}


// 获取下载方式
QString MainWindow::getMannerText()
{
    if (ui->radio_taxid->isChecked())
        return QString("-t");
    else if (ui->radio_assembly->isChecked())
        return QString("-A");
    else
        return QString("-g");
}

//
QString MainWindow::getFormatText()
{
    QString res;
    if (ui->check_all->isChecked())
        res = "all";
    else
    {
        if (ui->check_assembly_report->isChecked())
            res.append("assembly-report,");

        if (ui->check_assembly_stats->isChecked())
            res.append("assembly-stats,");

        if (ui->check_cds_fasta->isChecked())
            res.append("cds-fasta,");

        if (ui->check_rna_fasta->isChecked())
            res.append("rna_fasta,");

        if (ui->check_fasta->isChecked())
            res.append("fasta,");

        if (ui->check_wgs->isChecked())
            res.append("wgs,");

        if (ui->check_gff->isChecked())
            res.append("gff,");

        if (ui->check_features->isChecked())
            res.append("features,");

        if (ui->check_protein_fasta->isChecked())
            res.append("protein_fasta,");

        if (ui->check_cds_fasta->isChecked())
            res.append("cds-fasta,");

        if (ui->check_genbank->isChecked())
            res.append("genbank,");

        if (ui->check_genpept->isChecked())
            res.append("genpept,");

        if (ui->check_translated_cds->isChecked())
            res.append("translated-cds,");
    }

    res.truncate(res.length() - 1);
    qDebug() << res;
    return res;
}

// 全选
void MainWindow::check_all()
{
    if (!ui->check_all->isChecked())
    {
        ui->check_assembly_report->setChecked(false);
        ui->check_assembly_stats->setChecked(false);
        ui->check_features->setChecked(false);
        ui->check_translated_cds->setChecked(false);
        ui->check_gff->setChecked(false);
        ui->check_wgs->setChecked(false);
        ui->check_rm->setChecked(false);
        ui->check_fasta->setChecked(false);
        ui->check_rna_fasta->setChecked(false);
        ui->check_rna_fna->setChecked(false);
        ui->check_cds_fasta->setChecked(false);
        ui->check_protein_fasta->setChecked(false);
        ui->check_genbank->setChecked(false);
        ui->check_genpept->setChecked(false);
    }
    else
    {
        ui->check_assembly_report->setChecked(true);
        ui->check_assembly_stats->setChecked(true);
        ui->check_features->setChecked(true);
        ui->check_translated_cds->setChecked(true);
        ui->check_gff->setChecked(true);
        ui->check_wgs->setChecked(true);
        ui->check_rm->setChecked(true);
        ui->check_fasta->setChecked(true);
        ui->check_rna_fasta->setChecked(true);
        ui->check_rna_fna->setChecked(true);
        ui->check_cds_fasta->setChecked(true);
        ui->check_protein_fasta->setChecked(true);
        ui->check_genbank->setChecked(true);
        ui->check_genpept->setChecked(true);
    }
}

// 解析下载的文件数据
QStringList MainWindow::read(const QString& filename)
{
    QFile aFile;
    QStringList res;
    aFile.setFileName(filename);
    if (!aFile.exists())
        return res;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return res;

    while(!aFile.atEnd())
    {
        QByteArray line = aFile.readLine();
        QString str = QString::fromUtf8(line);
        str.truncate(str.length()-1);
        res << str;
    }
    aFile.close();
    // qDebug() << "res: " << res;
    return res;
}

void MainWindow::startNextProcess(int& begin, int& end, QStringList &data)
{
    if (begin < end)
    {
        QString current_folder = QDir::currentPath() + "/ncbi-genome-download.exe";
        // qDebug() << current_folder;
        QString ncbi(current_folder);
        QStringList arguments;
        arguments << "-s" << args["section"] << "-F" << args["format"] << args["manner"] << data[begin] << "-o" << args["output"] << args["group"];
        QString info_start(" 正在下载!");
        QProcess* process = new QProcess();
        process->start(ncbi, arguments);
        // qDebug() << begin;
        // qDebug() << data[begin];
        ui->text_browser_info->append(data[begin] + info_start);
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
        ui->progressbar_download->setValue(100 * begin / end);
    }
}


//
void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit)
    {
        if (exitCode == 0)
        {
            QString info_end(" 下载完成!\n");
            ui->text_browser_info->append(data[currentLoopIndex] + info_end);
            ++currentLoopIndex;
            // if (process->state() == QProcess::Running) {
            //     process->terminate();
            //     // 或者使用process.kill();，不过kill是更强制的终止方式
            //     process->kill();
            // }
        }
        else
        {
            // qDebug() << "进程正常结束，但出现错误，返回码为" << exitCode << " " << process->readAllStandardError();
            QString log;
            log.append("进程正常结束，但出现错误，返回码为");
            log.append(QString::number(exitCode));
            log.append(" 文件索引为");
            log.append(QString::number(currentLoopIndex));
            log.append(" 文件名字为");
            log.append(data[currentLoopIndex]);
            log.append("\n");
            ui->text_browser_info->append(log);
        }
        startNextProcess(currentLoopIndex, totalLoops, data);
    }
    else
    {
        qDebug() << "下载错误!\n";
        QString error("下载错误!\n");
        ui->text_browser_info->append(error);
    }
}



// 清除输出日志
void MainWindow::clear_stuff()
{
    ui->label_input->setText(QString("..."));
    ui->label_output->setText(QString("..."));
    ui->text_browser_info->clear();
    ui->button_download->setEnabled(true);
}
