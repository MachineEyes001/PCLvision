#pragma once

#include <QWidget>
#include "ui_NDTDlg.h"
//使用【正态分布变换算法】和【用来过滤数据的过滤器】对应的头文件，这个过滤器可以用其他过滤器来替换, 
//但是使用体素网格过滤器(approximate voxel filter)处理结果较好
#include <pcl/registration/ndt.h> // ndt配准头文件
#include <pcl/filters/approximate_voxel_grid.h>

class NDTDlg : public QWidget
{
	Q_OBJECT

public:
	NDTDlg(QWidget *parent = nullptr);
	~NDTDlg();

	QString pathtarget = "";
	QString pathscene = "";

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();
	// 浏览
	void on_BrowseTarget_clicked();
	// 浏览
	void on_BrowseScene_clicked();

signals:
	void NDT(double TransformationEpsilon, double StepSize, double Resolution, int MaximumIterations, int Axis, double AngleRotation, double X, double Y, double Z, double leaf);


private:
	Ui::NDTDlgClass ui;
};
