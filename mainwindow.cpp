#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

const static QUrl server(QLatin1String("http://localhost:3000"));
const static QString servicePath(QLatin1String("/service"));
const static QString versionPath(QLatin1String("/service/version"));
const static QString portsPath(QLatin1String("/service/interfaces"));

const static QString contentTypeJson(QLatin1String("application/json"));

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_nam(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    connect(ui->getPB, &QPushButton::clicked, this, &MainWindow::getPorts);
    connect(ui->uuidCB, &QComboBox::currentTextChanged, this, &MainWindow::getPort);
    connect(ui->getVersionPb, &QPushButton::clicked,this,&MainWindow::getVersion);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::getPorts()
{
    while (ui->uuidCB->count() > 0) {
        ui->uuidCB->removeItem(0);
    }

    QUrl url = server;
    url.setPath(portsPath);

    QNetworkRequest request(url);

    QNetworkReply *reply = m_nam->get(request);

    ui->textBrowser->append("Sent get request " + url.toString());

    connect(reply, &QNetworkReply::finished, this, [this, reply] () {
        ui->textBrowser->append("Answer recieved");
        reply->deleteLater();
        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonArray array = doc.array();

        ui->textBrowser->append("{");
        for (const QJsonValue &value : array) {
            ui->textBrowser->append("\t"+value.toString());
            ui->uuidCB->addItem(value.toString());
        }
        ui->textBrowser->append("}");
    });
}

void MainWindow::getPort()
{
    const QString uuid = ui->uuidCB->currentText();
    if (uuid.isEmpty()) {
        return;
    }

    QUrl url = server;
    url.setPath(portsPath + QLatin1Char('/') + uuid);
    QNetworkRequest request(url);

    QNetworkReply *reply = m_nam->get(request);
    ui->textBrowser->append("Sent get request " + url.toString());

    ui->textEdit->clear();
    connect(reply, &QNetworkReply::finished, this, [this, reply] () {
        ui->textBrowser->append("Answer recieved");
        reply->deleteLater();
        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = doc.object();

       ui->textEdit->append(obj.value(QLatin1String("name")).toString());
       ui->textEdit->append(obj.value(QLatin1String("hw_addr")).toString());
       ui->textBrowser->append("{");
       ui->textBrowser->append("\t"+obj.value(QLatin1String("name")).toString());
       ui->textBrowser->append("\t"+obj.value(QLatin1String("hw_addr")).toString());
       ui->textBrowser->append("}");
       });
}

void MainWindow::getVersion()
{
    QUrl url = server;
    url.setPath(versionPath);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_nam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply] () {
        ui->textBrowser->append("Answer recieved");
        reply->deleteLater();
        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = doc.object();
        ui->textBrowser->append("{");
        for(QString key: obj.keys()){
            ui->textBrowser->append("\t"+key);
        }
        ui->textBrowser->append("}");

//        ui->textEdit->append("ver  " + obj.value(QLatin1String("ver")).toInt());
        ui->apiVersionLabel->setText(QString::number(obj.value(QLatin1String("ver")).toInt()));
    });

}

