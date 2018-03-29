#include "marketbrowser.h"
#include "ui_marketbrowser.h"
#include "main.h"
#include "wallet.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include <QDesktopServices>

#include <sstream>
#include <string>

using namespace json_spirit;

const QString kBaseUrl = "http://qwoyn.io/qwnusd.php";
const QString kBaseUrl1 = "http://blockchain.info/tobtc?currency=USD&value=1";
const QString kBaseUrl2 = "http://qwoyn.io/qwnmc.php";
const QString kBaseUrl3 = "http://qwoyn.io/qwnbtc.php";

QString bitcoinp = "";
QString qwoynp = "";
QString qwnmcp = "";
QString qwnbtcp = "";
double bitcoin2;
double qwoyn2;
double qwnmc2;
double qwnbtc2;
QString bitcoing;
QString qwnmarket;
QString dollarg;
int mode=1;
int o = 0;


MarketBrowser::MarketBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketBrowser)
{
    ui->setupUi(this);
    setFixedSize(400, 420);


requests();
QObject::connect(&m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponse(QNetworkReply*)));
connect(ui->startButton, SIGNAL(pressed()), this, SLOT( requests()));
connect(ui->egal, SIGNAL(pressed()), this, SLOT( update()));

}

void MarketBrowser::update()
{
    QString temps = ui->egals->text();
    double totald = dollarg.toDouble() * temps.toDouble();
    double totaldq = bitcoing.toDouble() * temps.toDouble();
    ui->egald->setText("$ "+QString::number(totald)+" USD or "+QString::number(totaldq)+" BTC");

}

void MarketBrowser::requests()
{
	getRequest(kBaseUrl);
    getRequest(kBaseUrl1);
	getRequest(kBaseUrl2);
	getRequest(kBaseUrl3);
}

void MarketBrowser::getRequest( const QString &urlString )
{
    QUrl url ( urlString );
    QNetworkRequest req ( url );
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    m_nam.get(req);
}

void MarketBrowser::parseNetworkResponse(QNetworkReply *finished )
{

    QUrl what = finished->url();

    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        emit networkError( finished->error() );
        return;
    }
	
if (what == kBaseUrl) // Qwoyn Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString qwoyn = finished->readAll();
    qwoyn2 = (qwoyn.toDouble());
    qwoyn = QString::number(qwoyn2, 'f', 2);
	
    if(qwoyn > qwoynp)
    {
        ui->qwoyn->setText("<font color=\"yellow\">$" + qwoyn + "</font>");
    } else if (qwoyn < qwoynp) {
        ui->qwoyn->setText("<font color=\"red\">$" + qwoyn + "</font>");
        } else {
    ui->qwoyn->setText("$"+qwoyn+" USD");
    }

    qwoynp = qwoyn;
	dollarg = qwoyn;
}

if (what == kBaseUrl1) // Bitcoin Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString bitcoin = finished->readAll();
    bitcoin2 = (1 / bitcoin.toDouble());
    bitcoin = QString::number(bitcoin2, 'f', 2);
    if(bitcoin > bitcoinp)
    {
        ui->bitcoin->setText("<font color=\"yellow\">$" + bitcoin + " USD</font>");
    } else if (bitcoin < bitcoinp) {
        ui->bitcoin->setText("<font color=\"red\">$" + bitcoin + " USD</font>");
        } else {
    ui->bitcoin->setText("$"+bitcoin+" USD");
    }

    bitcoinp = bitcoin;
}

if (what == kBaseUrl2) // Qwoyn Market Cap
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString qwnmc = finished->readAll();
    qwnmc2 = (qwnmc.toDouble());
    qwnmc = QString::number(qwnmc2, 'f', 2);
	
    if(qwnmc > qwnmcp)
    {
        ui->qwnmc->setText("<font color=\"yellow\">$" + qwnmc + "</font>");
    } else if (qwnmc < qwnmcp) {
        ui->qwnmc->setText("<font color=\"red\">$" + qwnmc + "</font>");
        } else {
    ui->qwnmc->setText("$"+qwnmc+" USD");
    }

    qwnmcp = qwnmc;
	qwnmarket = qwnmc;
}

if (what == kBaseUrl3) // Qwoyn BTC Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString qwnbtc = finished->readAll();
    qwnbtc2 = (qwnbtc.toDouble());
    qwnbtc = QString::number(qwnbtc2, 'f', 8);
	
    if(qwnbtc > qwnbtcp)
    {
        ui->qwnbtc->setText("<font color=\"yellow\">" + qwnbtc + " BTC</font>");
    } else if (qwnbtc < qwnbtcp) {
        ui->qwnbtc->setText("<font color=\"red\">" + qwnbtc + " BTC</font>");
        } else {
    ui->qwnbtc->setText(qwnbtc+" BTC");
    }

    qwnbtcp = qwnbtc;
	bitcoing = qwnbtc;
}

finished->deleteLater();
}


void MarketBrowser::setModel(ClientModel *model)
{
    this->model = model;
}

MarketBrowser::~MarketBrowser()
{
    delete ui;
}
