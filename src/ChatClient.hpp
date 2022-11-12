#ifndef BOTTAWS_CHATCLIENT_HPP
#define BOTTAWS_CHATCLIENT_HPP


#include <QObject>
#include <QWebSocket>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    ChatClient(const QUrl& url, QObject* parent = nullptr);

signals:

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString& message);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
};


#endif // BOTTAWS_CHATCLIENT_HPP
