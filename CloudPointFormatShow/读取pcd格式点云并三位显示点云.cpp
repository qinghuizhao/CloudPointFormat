#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <string>
#include <iostream>
using namespace std;

void  readPCD(string namefile);
void view(string namefile);
void viewerPsycho (pcl::visualization::PCLVisualizer& viewer);
void viewerOneOff (pcl::visualization::PCLVisualizer& viewer);;
int user_data;

int
main(int argc,char** argv)
{
    string pcd= "1.pcd";
    // readPCD(pcd);
    view(pcd);
    return 0;
}

void  readPCD(string namefile)
{

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile<pcl::PointXYZ>(namefile,*cloud);
    if(pcl::io::loadPCDFile<pcl::PointXYZ>(namefile,*cloud)==-1)//*打开点云文件
    {
        PCL_ERROR("Couldn't read file test_pcd.pcd\n");
    }
    std::cout<<"Loaded "
            <<cloud->width*cloud->height
           <<" data points from test_pcd.pcd with the following fields: "
          <<std::endl;
    for(size_t i=0;i<cloud->points.size();++i)
        std::cout<<"    "<<cloud->points[i].x
                <<" "<<cloud->points[i].y
               <<" "<<cloud->points[i].z<<std::endl;

}
void view(string namefile)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile<pcl::PointXYZ>(namefile,*cloud);
    if(pcl::io::loadPCDFile<pcl::PointXYZ>(namefile,*cloud)==-1)//*打开点云文件
    {
        PCL_ERROR("Couldn't read file 1.pcd\n");
    }
    static int p=1;
    char namefile2[55];
    sprintf(namefile2,"第%d帧Cloud Viewer",p++);
    pcl::visualization::CloudViewer viewer(namefile2);
    //pcl::visualization::CloudViewer viewer("Cloud Viewer");
    //showCloud函数是同步的，在此处等待直到渲染显示为止
    viewer.showCloud(cloud);
    //该注册函数在可视化时只调用一次
    viewer.runOnVisualizationThreadOnce (viewerOneOff);
    //该注册函数在渲染输出时每次都调用
    viewer.runOnVisualizationThread (viewerPsycho);

    while (!viewer.wasStopped ())
    {
        //在此处可以添加其他处理
        user_data++;
    }
    /*
        Sleep(1);
        viewer.wasStopped ();*/
}
void 
viewerOneOff (pcl::visualization::PCLVisualizer& viewer)
{
    //viewer.setBackgroundColor (1.0, 0.5, 1.0);
    viewer.setBackgroundColor (0.0, 0.0, 0.0);

    pcl::PointXYZ o;
    o.x = 1.0;
    o.y = 0;
    o.z = 0;
    viewer.addSphere (o, 0, "sphere", 0);
    std::cout << "i only run once" << std::endl;

}

void 
viewerPsycho (pcl::visualization::PCLVisualizer& viewer)
{
    static unsigned count = 0;
    std::stringstream ss;
    ss <<"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"<< count++;
    // ss << "Once per viewer loop: " << count++;
    viewer.removeShape ("text", 0);
    viewer.addText (ss.str(), 200, 300, "text", 0);
    //FIXME: possible race condition here:
    user_data++;
}
