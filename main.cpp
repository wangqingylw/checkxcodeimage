// use c++ as script
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main() {
    const string rootpath = "/Users/wangqing/Workplace/cloudwood_swift_3_0later";
    const string checkpath = rootpath + "/ip";
    const string assetspath = rootpath + "/ip/Resource/Assets.xcassets";

    struct stat info;

    stat(rootpath.c_str(), &info);

    if(S_ISDIR(info.st_mode)) {
        cout << rootpath << " is a directory" << endl;
    }

    return 0;
}