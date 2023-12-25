#pragma once

#include <QWidget>
#include "ui_DepthMapDlg.h"
#include <pcl/range_image/range_image.h>    //深度图像的头文件
#include <pcl/visualization/range_image_visualizer.h>
#include <pcl/visualization/common/float_image_utils.h>//保存深度图像
#include <pcl/io/png_io.h>//保存深度图像

class DepthMapDlg : public QWidget
{
	Q_OBJECT

public:
	DepthMapDlg(QWidget *parent = nullptr);
	~DepthMapDlg();

	QString DepthMappath = "";

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

	// 浏览
	void on_Browse_clicked();

signals:
	void DepthMap(double angularResolution, double maxAngleWidth, double maxAngleHeight, double X, double Y, double Z, double noiseLevel, double minRange, int borderSize);

private:
	Ui::DepthMapDlgClass ui;
};
