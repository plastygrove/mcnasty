#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->menuExit, SIGNAL(triggered()), this, SLOT(Exit()));
    connect(this->ui->menuNew, SIGNAL(triggered()), this, SLOT(FileNew()));
    connect(this->ui->menuOpen, SIGNAL(triggered()), this, SLOT(FileOpen()));
    connect(this->ui->menuClose, SIGNAL(triggered()), this, SLOT(FileClose()));

    breakPointAction = new QAction(this);
    compileAction = new QAction(this);
    runAction = new QAction(this);
    debugAction = new QAction(this);

    breakPointAction->setShortcut(QKeySequence(tr("Ctrl+B")));
    compileAction->setShortcut(QKeySequence(tr("Ctrl+G")));
    runAction->setShortcut(QKeySequence(tr("Ctrl+R")));
    debugAction->setShortcut(QKeySequence(tr("Ctrl+D")));

    connect(breakPointAction, SIGNAL(triggered()), this, SLOT(ToggleBreakpoint()));
    connect(compileAction, SIGNAL(triggered()), this, SLOT(DoCompile()));
    connect(runAction, SIGNAL(triggered()), this, SLOT(Run()));
    connect(debugAction, SIGNAL(triggered()), this, SLOT(Debug()));

    this->addAction(breakPointAction);
    this->addAction(compileAction);
    this->addAction(runAction);
    this->addAction(debugAction);


    codeEditor = new CodeEditor();
    outputBox = new QPlainTextEdit();
    debugStatus = new QLabel();

    splitter = new QSplitter(); //Splitter helps resize the widgets

    splitter->addWidget(codeEditor);
    splitter->addWidget(outputBox);

    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false); //So that the child widgets don't get resized beyond minimum height

    this->ui->verticalLayout->addWidget(splitter);
    this->ui->verticalLayout->addWidget(debugStatus); //Don't want the status bar to be resizeable

    codeEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outputBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    codeEditor->setMinimumSize(200,140);
    outputBox->setMinimumHeight(50);

    isDebug = true;


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::SetCurrentBreakPoint()
{
    codeEditor->addBreakPoint();
}*/

void MainWindow::FileNew()
{

}

void MainWindow::FileClose()
{

}

void MainWindow::FileOpen()
{

    fileName = qd.getOpenFileName(this, tr("Open File"), projectDir.isEmpty()?"":projectDir, tr("Assembly Files (*.asm);;All Files (*.*)"));


    if(fileName.isEmpty())
    {
        return;
    }
    else
    {
        if(isDebug) qDebug(fileName.toLocal8Bit().data());
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QFileInfo qf(file);
        projectDir = qf.absolutePath();



        codeEditor->clear();
        QTextStream in(&file);

        while(!in.atEnd())
        {
            QString line = in.readLine();
            codeEditor->appendPlainText(line);
        }

    }
}

void MainWindow::Exit()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    codeEditor->addBreakPoint();
}

void MainWindow::ToggleBreakpoint()
{
    if(isDebug) qDebug("Setting Breakpoint!");
    codeEditor->toggleBreakPoint();
}

void MainWindow::DoCompile()
{
    if(isDebug) qDebug("Doing compile");
    QObject *parent = this;
    QString program = "/usr/bin/make";
    QStringList arguments;
    arguments<< projectDir<<"/makefile";
    QProcess *myProcess = new QProcess(parent);
    myProcess->start(program, arguments);
    if(!myProcess->waitForFinished())
        return;

    this->outputBox->setPlainText(myProcess->readAll());
}

void MainWindow::Run()
{
    if(isDebug) qDebug("Doing Run");
    if(projectDir.isEmpty() || fileName.isEmpty())
    {
        this->outputBox->appendPlainText("File not opened, please open the file and compile it");
        return;
    }
    QObject *parent = this;
    QString program = fileName;
    program.remove(".asm");
    //if(isDebug) qDebug(projectDir.toLocal8Bit().data());
    if(isDebug) qDebug(fileName.toLocal8Bit().data());
    //if(isDebug) qDebug(fileName.remove(".asm").toLocal8Bit().data());
    if(isDebug) qDebug(program.toLocal8Bit().data());
    QProcess *myProcess = new QProcess(parent);
    myProcess->start(program);
    if(!myProcess->waitForFinished())
        return;

    this->outputBox->setPlainText(myProcess->readAll());

}

void MainWindow::Debug()
{

}
