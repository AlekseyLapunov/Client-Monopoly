#include "userinterface/loginwindow.h"
#include "helpers/servercommunicator.h"
#include "helpers/usermetainfo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ServerCommunicator server;
    UserMetaInfo userMeta;

    LoginWindow loginWindow;
    loginWindow.sharePointers(&server, &userMeta);
    loginWindow.show();

    return a.exec();
}
