#ifndef REQUESTER_H
#define REQUESTER_H


class requester : public QObject
{
    Q_OBJECT

public:
    requester();
    ~requester();

    void go();
protected slots:
    void replyFinished(QNetworkReply* reply);
    void errorInRequest(QNetworkReply::NetworkError code);

private:
    QElapsedTimer timer_;
    int counter_;

};

#endif // REQUESTER_H
