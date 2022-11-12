#ifndef BOTTAWS_TWITCHAUTH_HPP
#define BOTTAWS_TWITCHAUTH_HPP


#include <QOAuth2AuthorizationCodeFlow>


class TwitchAuth : public QObject
{
    Q_OBJECT
public:
    TwitchAuth(QObject* parent = nullptr);

signals:
    void authenticated();

public slots:
    void grant();
    QNetworkReply* get(const QUrl& url, const QVariantMap& parameters = QVariantMap());

private:
    QOAuth2AuthorizationCodeFlow m_oauth2;
};


#endif // BOTTAWS_TWITCHAUTH_HPP
