#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class QNetworkAccessManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void getPorts();
    void getPort();
    void getVersion();
    void command();
private:
    void newOrUpdateUser();
    void newUser();
    void updateUser();
    void removeUser();

    Ui::MainWindow *ui;
    QProcess *m_process;
    QNetworkAccessManager *m_nam;
};

#endif // MAINWINDOW_H
