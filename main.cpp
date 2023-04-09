#include <memory>
#include <exception>

#include <QApplication>
#include <QDebug>

#include "userinterface/loginwindow.h"
#include "helpers/servercommunicator.h"
#include "helpers/usermetainfo.h"
#include "helpers/common/filemanager.h"

using std::unique_ptr;

unique_ptr<ServerCommunicator> server(new ServerCommunicator());
unique_ptr<UserMetaInfo> userMeta(new UserMetaInfo());

int startApplication(int argc, char *argv[])
{
    initLocalDirectory();
    QApplication a(argc, argv);

    unique_ptr<LoginWindow> loginWindow(new LoginWindow(&server, &userMeta));
    loginWindow->show();

    return a.exec();
}

int main(int argc, char *argv[])
{
    startApplication(argc, argv);

    return 0;
}
