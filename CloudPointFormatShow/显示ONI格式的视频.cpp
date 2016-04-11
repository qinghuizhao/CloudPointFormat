#include <XnCppWrapper.h>
#include "highgui.h"
#include "cv.h"
using namespace xn; 
using namespace std; 
int ShowOni(string sFilename);
int main(int argc, char* argv[])
{
    string video = "1.oni";
    ShowOni(video);
    return 0;
}
int ShowOni(string sFilename)
{
    vector<IplImage*> depths,images;
    XnStatus nRetVal = XN_STATUS_OK;
    xn::Context g_context;     //创建上下文对象
    nRetVal=g_context.Init();     //上下文对象初始化
    g_context.OpenFileRecording(sFilename.c_str());     //打开已有的oni文件
    xn::ImageGenerator g_image;     //创建image generator
    nRetVal=g_context.FindExistingNode(XN_NODE_TYPE_IMAGE,g_image);     //获取oni文件中的image节点
    xn::DepthGenerator g_depth;     //创建depth generator
    nRetVal=g_context.FindExistingNode(XN_NODE_TYPE_DEPTH,g_depth);     //获取oni文件中的depth节点
    xn::ImageMetaData g_imd;     //创建image节点元数据对象
    xn::DepthMetaData g_dmd;     //创建depth节点元数据对象

    int nWidth;     //oni数据中灰度/彩色图像的宽度
    int nHeight;    //oni数据中灰度/彩色图像的高度
    IplImage *g_pImgColor=0;     //定义用于opencv显示的彩色图像
    IplImage *g_pImgDepth=0;     //定义用于opencv显示的深度图像
    IplImage *imgDepth16u=0;     //存储灰度信息
    IplImage *imgRGB8u=0;        //存储彩色信息
    //保存oni深度图像到IplImage*格式vector
    XnUInt32 frameDepth = 0;
    while(1)
    {
        nRetVal = g_context.WaitOneUpdateAll(g_depth);     //更新深度数据
        if(nRetVal!=XN_STATUS_OK)     //判断是否更新成功
        {
            printf("failed update depth image/n");
            continue;
        }
        if(g_depth.GetFrameID()<frameDepth)break;     //判断是否循环结束
        else
        {
            g_depth.GetMetaData(g_dmd);     //获取g_depth的元数据g_dmd
            if(g_pImgDepth==0)     //根据当前的深度图像元数据设置g_pImgDepth的大小和通道数
            {
                nWidth=g_dmd.XRes();
                nHeight=g_dmd.YRes();
                g_pImgDepth=cvCreateImage(cvSize(nWidth,nHeight),8,1);
                imgDepth16u=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_16U,1);
                cvZero(g_pImgDepth);
                cvZero(imgDepth16u);
            }
            memcpy(imgDepth16u->imageData,g_dmd.Data(),nWidth*nHeight*2);
            cvConvertScale(imgDepth16u,g_pImgDepth,255/4096.0,0);
            cvEqualizeHist(g_pImgDepth,g_pImgDepth);     //直方图均衡化，拉开深度层次
            depths.push_back(g_pImgDepth);     //保存到vector中
            frameDepth = g_depth.GetFrameID();//记录当前frameID
            cvNamedWindow("深度图");
            cvShowImage("深度图",depths.front());     //输出测试
            char c=cvWaitKey(30);
            if(27==c)break;
        }

        //保存oni彩色图像到IplImage*格式vector
        XnUInt32 frameImage = 0;
        nRetVal = g_context.WaitOneUpdateAll(g_depth);     //更新深度数据
        if(nRetVal!=XN_STATUS_OK)     //判断是否更新成功
        {
            printf("failed update color image/n");
            continue;
        }
        if(g_image.GetFrameID()<frameImage)break;     //判断是否循环结束
        else
        {
            g_image.GetMetaData(g_imd);     //获取g_image的元数据g_imd
            if(g_pImgColor==0)     //根据当前的彩色图像元数据设置g_pImgColor的大小和通道数
            {
                nWidth=g_imd.XRes();
                nHeight=g_imd.YRes();
                g_pImgColor=cvCreateImage(cvSize(nWidth,nHeight),8,3);
                imgRGB8u=cvCreateImage(cvSize(nWidth,nHeight),8,3);
                cvZero(g_pImgColor);
                cvZero(imgRGB8u);
            }
            memcpy(imgRGB8u->imageData,g_imd.Data(),nWidth*nHeight*3);
            cvCvtColor(imgRGB8u,g_pImgColor,CV_RGB2BGR);
            images.push_back(g_pImgColor);     //保存到vector中
            frameImage = g_image.GetFrameID(); //记录当前frameID
            cvNamedWindow("RGB图");
            cvShowImage("RGB图",images.front());      //输出测试
            char c=cvWaitKey(30);
            if(27==c)break;
        }
    }

    //一起清理
    if(g_pImgDepth)cvReleaseImage(&g_pImgDepth);
    if(g_pImgColor)cvReleaseImage(&g_pImgColor);
    if(imgDepth16u)cvReleaseImage(&imgDepth16u);
    if(imgRGB8u)cvReleaseImage(&imgRGB8u);
    g_context.StopGeneratingAll();
    g_context.Shutdown();
    cvDestroyAllWindows();
    return 0;
}




