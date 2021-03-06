// use c++ as script
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>

#include <vector>
#include <map>
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

bool isDir(dirent* fileinfo) {
    if (fileinfo->d_type == DT_DIR) {
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

    size_t pos = str.rfind(".");

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

            
            if (isDir(filename))
            {
                if (ext == ".imageset")
                {
                    images.push_back(base);
                }
                else
                {
                    string fullpath = path + "/" + string(filename->d_name);
                    vector<string> subImages = getImages(fullpath);

                    images.insert(images.end(), subImages.begin(), subImages.end());
                }
            }
        }
    }
    return images;
}

void checkForFile(string path, vector<string> &images)
{
    if (isDir(path))
    {
        return;
    }

    ifstream t(path);
    stringstream buffer;

    //buffer << t.rdbuf();
    //string contents(buffer.str());

    string contents((istreambuf_iterator<char>(t)),  
                 istreambuf_iterator<char>());


    for (vector<string>::const_iterator p = images.cbegin();
         p != images.cend();
    )
    {
        if ( strstr(contents.c_str(), ("\"" + (*p) + "\"").c_str()) != NULL ) {
            p = images.erase(p);
        } else {
            ++p;
        }
    }


    buffer.clear();
    t.close();

    //cout << path << " check over!" << endl;
}

void checkForDir(string path, vector<string> &images)
{
    if (!isDir(path))
    {
        return;
    }

    DIR *dir;

    dir = opendir(path.c_str());

    if (NULL == dir)
    {
        return;
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
        
        if (!isDir(filename))
        {
            // 需要处理
            if (ext == ".m" || ext == ".h" || ext == ".xib" || ext == ".swift")
            {
                string fullpath = path + "/" + string(filename->d_name);
                checkForFile(fullpath, images);
            }
        }
        else
        {
            string fullpath = path + "/" + string(filename->d_name);
            checkForDir(fullpath, images);
        }
    }
}

int main()
{
    clock_t start, finish;

    start = clock();

    const string rootpath = "/Users/wangqing/Workplace/cloudwood_swift_3_0later";
    const string checkpath = rootpath + "/ip";
    const string assetspath = rootpath + "/ip/Resource/Assets.xcassets";

    vector<string> images = getImages(assetspath);

    checkForDir(checkpath, images);


    for (vector<string>::const_iterator p = images.cbegin();
         p != images.cend();
         p++)
    {


        cout << (*p) << endl;
    }

    finish = clock();

    cout << "=======================" << endl
         << "total :" << images.size() << endl
         << "cost : " << (double)(finish - start) / CLOCKS_PER_SEC << " seconds!" << endl;

    return 0;
}