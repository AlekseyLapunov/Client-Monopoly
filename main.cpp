#include <memory>
#include <exception>

#include <QApplication>
#include <QDebug>

#include "userinterface/loginwindow.h"
#include "servercomm/servercommunicator.h"
#include "helpers/usermetainfo.h"
#include "helpers/common/filemanager.h"

using std::unique_ptr;

int main(int argc, char *argv[])
{
    FileManager::initLocalDirectory();
    QApplication a(argc, argv);

    unique_ptr<ServerCommunicator> server(new ServerCommunicator(&a));
    unique_ptr<UserMetaInfo> userMeta(new UserMetaInfo());

    unique_ptr<LoginWindow> loginWindow(new LoginWindow(&server, &userMeta));
    loginWindow->show();

    return a.exec();
}
