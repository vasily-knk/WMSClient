#include "StdAfx.h"
#include "requester.h"

requester::requester()
    : QObject(NULL)
{

}

requester::~requester()
{

}

void requester::go()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(replyFinished(QNetworkReply*)));

    QUrl url;
    url.setScheme("http");
    url.setHost("192.168.121.129");
    url.setPath("/cgi-bin/tilecache.cgi");
    url.setQueryDelimiters('=', '&');
    url.addQueryItem("layers", "osm");
    url.addQueryItem("service", "mapnik");
    url.addQueryItem("version", "1.1.1");
    url.addQueryItem("request", "GetMap");
    url.addQueryItem("bbox", "-78271.51696402048, 6653078.941941741, 0.0, 6731350.458905762");

    QNetworkRequest request(url);

    timer_.start();

    counter_ = 100;
    for (int i = 0; i < counter_; ++i)
        manager->get(request);                                      
}

void requester::replyFinished(QNetworkReply* reply)
{
    --counter_;
    if (counter_ == 0)
        cout << timer_.elapsed() << " until reply" << endl;

    /*
    QByteArray array = reply->readAll();
    timer_.restart();
    cout << timer_.elapsed() << " until read" << endl;


    cout << array.size() << endl;
    cout << reply->error() << endl;
    cout << endl;

    cout << "Done!" << endl;
*/
}

void requester::errorInRequest(QNetworkReply::NetworkError code)
{
    cout << "Error" << code << endl;
}
