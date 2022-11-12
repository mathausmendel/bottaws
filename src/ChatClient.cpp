#include "ChatClient.hpp"
#include <iostream>

ChatClient::ChatClient(const QUrl& url, QObject* parent)
    : QObject(parent)
    , m_url(url)
{
    connect(&m_webSocket, &QWebSocket::connected,
            this, &ChatClient::onConnected);

    connect(&m_webSocket, &QWebSocket::disconnected,
            this, &ChatClient::onDisconnected);

    m_webSocket.open(QUrl(m_url));
}

void ChatClient::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &ChatClient::onTextMessageReceived);
}

void ChatClient::onDisconnected()
{

}

void ChatClient::onTextMessageReceived(const QString& message)
{
    std::cout << message.toStdString() << std::endl;
}
