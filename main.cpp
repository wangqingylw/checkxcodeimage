// use c++ as script
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>

#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

bool isDir(string path)
{
    struct stat info;

    stat(path.c_str(), &info);

    if (S_ISDIR(info.st_mode))
    {
        return true;
    }
    return false;
}

vector<string> getBaseAndExt(const string str)
{
    vector<string> res;

    if ("" == str)
    {
        res.push_back("");
        res.push_back("");
        return res;
    }

    size_t pos = str.find_last_of(".");

    if (pos != str.npos)
    {
        res.push_back(str.substr(0, pos));
        res.push_back(str.substr(pos));
    }

    return res;
}

vector<string> getImages(string path)
{
    vector<string> images;

    if (isDir(path))
    {
        DIR *dir;

        dir = opendir(path.c_str());
        if (NULL == dir)
        {
            cout << "Can not open dir " << dir << endl;
            return images;
        }

        struct dirent *filename;

        /* read all the files in the dir ~ */
        while ((filename = readdir(dir)) != NULL)
        {
            // get rid of "." and ".."
            if (strcmp(filename->d_name, ".") == 0 ||
                strcmp(filename->d_name, "..") == 0)
                continue;

            vector<string> baseAndExt = getBaseAndExt(string(filename->d_name));

            string base = "";
            string ext = "";

            if (baseAndExt.size() >= 2)
            {
                base = baseAndExt[0];
                ext = baseAndExt[1];
            }

            string fullpath = path + "/" + string(filename->d_name);

            if (isDir(fullpath))
            {
                if (ext == ".imageset")
                {
                    images.push_back(base);
                }
                else
                {
                    vector<string> subImages = getImages(fullpath);

                    images.insert(images.end(), subImages.begin(), subImages.end());
                }
            }
        }
    }

    return images;
}

int main()
{
    const string rootpath = "/Users/wangqing/Workplace/cloudwood_swift_3_0later";
    const string checkpath = rootpath + "/ip";
    const string assetspath = rootpath + "/ip/Resource/Assets.xcassets";

    vector<string> images = getImages(assetspath);

    for (vector<string>::const_iterator imagePoint = images.cbegin(); imagePoint != images.cend(); imagePoint++  ) {
        cout << (*imagePoint) << endl;
    }


    return 0;
}