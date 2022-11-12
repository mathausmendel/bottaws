#include "TwitchAuth.hpp"
#include <QtNetworkAuth>
#include <QDesktopServices>
#include <fmt/format.h>


static const std::string clientId = "vvz87m9e3a6s4041xdrkdwg9orpqug";
static const std::string redirectUri = "http://localhost:3000";
static const std::vector<std::string> scope
{
    "channel:manage:polls",
    "channel:read:polls",
    "chat:edit"
};


TwitchAuth::TwitchAuth(QObject* parent)
    : QObject(parent)
{
    auto replyHandler = new QOAuthHttpServerReplyHandler(3000, this);
    m_oauth2.setReplyHandler(replyHandler);
    m_oauth2.setAuthorizationUrl(QUrl("https://id.twitch.tv/oauth2/authorize"));
    m_oauth2.setAccessTokenUrl(QUrl(QString::fromStdString(redirectUri)));
    m_oauth2.setClientIdentifier(QString::fromStdString(clientId));
    m_oauth2.setScope(QString::fromStdString(fmt::format("{}", fmt::join(scope, " "))));

    connect(&m_oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged,
    [&](QAbstractOAuth::Status status)
    {
        if(status == QAbstractOAuth::Status::Granted)
        {
            emit authenticated();
        }
    });

    m_oauth2.setModifyParametersFunction(
    [&](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters)
    {
        if(stage == QAbstractOAuth::Stage::RequestingAuthorization)
        {
            parameters->insert("response_type", "token");
        }
    });

    // Open the desktop browser if auth is not persisted
    connect(&m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
}

void TwitchAuth::grant()
{
    m_oauth2.grant();
}

QNetworkReply* TwitchAuth::get(const QUrl& url, const QVariantMap& parameters)
{
    return m_oauth2.get(url, parameters);
}
