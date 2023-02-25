#include <memory>

#include <QApplication>

#include "userinterface/loginwindow.h"
#include "helpers/servercommunicator.h"
#include "helpers/usermetainfo.h"

using std::unique_ptr;

unique_ptr<ServerCommunicator> server(new ServerCommunicator());
unique_ptr<UserMetaInfo> userMeta(new UserMetaInfo());

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    unique_ptr<LoginWindow> loginWindow(new LoginWindow(&server, &userMeta));
    loginWindow->show();

    return a.exec();
}
