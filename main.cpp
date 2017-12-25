#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QUdpSocket>
#include <QDebug>
#include <qqmlcontext.h>

class zhGlobalSocket: public QThread
{
    Q_OBJECT
public:
    zhGlobalSocket(QThread *parent = 0): QThread(parent)
    {
        udpRecv = 0;
        bindUDPRecive();
    }
private:
    QUdpSocket *udpRecv;

    void bindUDPRecive(){
        if(udpRecv != 0)
            delete udpRecv;
        udpRecv = new QUdpSocket(this);
        udpRecv->bind(8889, QUdpSocket::ShareAddress);//port for RECEIVE
        connect(udpRecv, SIGNAL(readyRead()), this, SLOT(onReadyReadUDPMsg()));
        qDebug() << "bindUDPRecive bind OK <------";
    }
public slots:
    void onReadyReadUDPMsg(){
        QByteArray buffer;
        buffer.resize(udpRecv->pendingDatagramSize());
        QHostAddress AddressForSender;
        quint16 PortForSender;
        udpRecv->readDatagram(buffer.data(), buffer.size(),
                              &AddressForSender, &PortForSender);
        qDebug() << "onReadyReadUDPMsg():" << buffer.toHex();
    }


};

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQmlContext *ctext=engine.rootContext();
    zhGlobalSocket g_socket;
    ctext->setContextProperty("zhGlobalSocket", &g_socket);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
#include "main.moc"
