#include "testrunner.h"

#include "tst_modules/tst_filemanager.h"
#include "tst_modules/tst_sourcestructs.h"
#include "tst_modules/tst_usermetainfo.h"

int main(int argc, char *argv[])
{
    int status = 0;

    runTests<FileManagerTest>(argc, argv, &status);
    runTests<SourceStructsTest>(argc, argv, &status);
    runTests<UserMetaTest>(argc, argv, &status);

    return status;
}
