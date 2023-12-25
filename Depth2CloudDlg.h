#pragma once
#include <QWidget>
#include "ui_Depth2CloudDlg.h"
#include <opencv2/opencv.hpp>
#include <pcl/point_cloud.h>					//点云类
#include <sstream>
#include <iostream>
#include <pcl/point_types.h>					//点云数据类型

class Depth2CloudDlg : public QWidget
{
	Q_OBJECT

public:
	Depth2CloudDlg(QWidget *parent = nullptr);
	~Depth2CloudDlg();

	QString pathDepth = "";
	QString pathRGB = "";
	QString pathCameraData = "";

/**
* 将彩色图和深度图合并成点云
* @param matrix 相机内参矩阵3x3
* @param rgb    彩色图
* @param depth  深度图
* @param cloud  输出点云
*/
    static void convert(cv::Mat& matrix, cv::Mat& rgb, cv::Mat& depth, pcl::PointCloud<pcl::PointXYZRGB>::Ptr& cloud)
    {
        double camera_fx = matrix.at<double>(0, 0);
        double camera_fy = matrix.at<double>(1, 1);
        double camera_cx = matrix.at<double>(0, 2);
        double camera_cy = matrix.at<double>(1, 2);

        std::cout << "fx: " << camera_fx << std::endl;
        std::cout << "fy: " << camera_fy << std::endl;
        std::cout << "cx: " << camera_cx << std::endl;
        std::cout << "cy: " << camera_cy << std::endl;

        // 遍历深度图
        for (int v = 0; v < depth.rows; v++)
            for (int u = 0; u < depth.cols; u++) {
                // 获取深度图中(m,n)处的值
                ushort d = depth.ptr<ushort>(v)[u];
                // d 可能没有值，若如此，跳过此点
                if (isnan(static_cast<double>(d)) && abs(d) < 0.0001)
                    continue;
                // d 存在值，则向点云增加一个点
                pcl::PointXYZRGB p;

                // 计算这个点的空间坐标
                p.z = double(d) / 1000; //单位是米
                p.x = (u - camera_cx) * p.z / camera_fx;
                p.y = (v - camera_cy) * p.z / camera_fy;

                // 从rgb图像中获取它的颜色
                // rgb是三通道的BGR格式图，所以按下面的顺序获取颜色
                cv::Vec3b bgr = rgb.at<cv::Vec3b>(v, u);
                p.b = bgr[0];
                p.g = bgr[1];
                p.r = bgr[2];

                // 把p加入到点云中
                cloud->points.push_back(p);
                //cout << cloud->points.size() << endl;
            }


        // 设置并保存点云
        cloud->height = 1;
        cloud->width = cloud->points.size();
        std::cout << "point cloud size = " << cloud->points.size() << std::endl;
        cloud->is_dense = false;
    }


private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

	void on_BrowseDepth_clicked();
	void on_BrowseRGB_clicked();
	void on_BrowseCameraData_clicked();

signals:
	void Depth2Cloud();

private:
	Ui::Depth2CloudDlgClass ui;
};
