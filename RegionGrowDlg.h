#pragma once

#include <QWidget>
#include "ui_RegionGrowDlg.h"
#include <pcl/segmentation/region_growing.h>//区域增长点云分割算法

class RegionGrowDlg : public QWidget
{
	Q_OBJECT

public:
	RegionGrowDlg(QWidget *parent = nullptr);
	~RegionGrowDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void RegionGrow(int KSearch, int NumberOfNeighbours, double SmoothnessThreshold, double CurvatureThreshold, int MinClusterSize, int MaxClusterSize);

private:
	Ui::RegionGrowDlgClass ui;
};
