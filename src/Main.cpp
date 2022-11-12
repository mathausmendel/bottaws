#include <QApplication>
#include <QMainWindow>
#include "ChatClient.hpp"
#include "TwitchAuth.hpp"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.show();

    //ChatClient client;

    TwitchAuth auth;
    auth.grant();

    return app.exec();
}
