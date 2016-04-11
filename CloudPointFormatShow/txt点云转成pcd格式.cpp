#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <iostream>
#include "imagehlp.h"
using namespace std;
#pragma comment(lib,"imagehlp.lib")
void  TxtToPCD(string namefile);
int user_data;

int
main(int argc,char** argv)
{

    char namefile[55];
    for (int i=1;i<39;i++)
    {
        sprintf(namefile,"E:/aa/%d.txt",i);
        TxtToPCD(namefile);
    }

    return 0;
}

void  TxtToPCD(string namefile)
{
    //pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ> cloud1;
    ifstream file(namefile);//读取
    if (!file)//如果文件不存在返回-1
    {
        cout<<"文件不存在2"<<endl;;
    }
    int i=1;
    while(1)
    {

        float tempX,tempY,tempZ;
        if(!(file>>tempX))
            break;
        if(!(file>>tempY))
            break;
        if(!(file>>tempZ))
            break;
        else
            //vPointCloud.push_back(tempX,tempY,tempZ);
            i++;
    }
    cloud1.width    = i-1;
    cloud1.height   =1;
    cloud1.is_dense = false;
    cloud1.points.resize (cloud1.width * cloud1.height);
    ifstream file1(namefile);//读取
    if (!file1)//如果文件不存在返回-1
    {
        cout<<"文件不存在2"<<endl;;
    }
    int ii=0;
    while(1)
    {

        float tempX,tempY,tempZ;
        if(!(file1>>tempX))
            break;
        if(!(file1>>tempY))
            break;
        if(!(file1>>tempZ))
            break;
        else
            cloud1.points[ii].x=tempX;
        cloud1.points[ii].y=tempY;
        cloud1.points[ii++].z=tempZ;
    }
    file.close();
    char *path="D:\\数据\\txt转换成pcd\\";
    if (_access(path,0)!=0)
    {
        MakeSureDirectoryPathExists(path);
    }
    char name[50];
    static int p=0;
    sprintf(name,"D:\\数据\\txt转换成pcd\\%d.pcd",++p);
    pcl::io::savePCDFileASCII(name,cloud1);
    cout<<"第"<<p<<"帧TXT转换成PCD点云已完成!"<<endl;
}
