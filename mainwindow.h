#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QLayout>
#include <QAction>
#include <QLabel>
#include <QProcess>
#include <QObject>
#include <QSplitter>
#include <codeeditor.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    CodeEditor *codeEditor;
    QLayout *layout;
    QString projectDir;
    QString fileName;
    QFileDialog qd;
    QPlainTextEdit *outputBox;
    QAction *breakPointAction;
    QAction *compileAction;
    QAction *runAction;
    QAction *debugAction;
    QLabel *debugStatus;
    bool isDebug;
    QSplitter *splitter;



private slots:
    void FileClose();
    void FileOpen();
    void FileNew();
    void Exit();
    void ToggleBreakpoint();
    void on_pushButton_clicked();
    void DoCompile();
    void Run();
    void Debug();
};

#endif // MAINWINDOW_H
