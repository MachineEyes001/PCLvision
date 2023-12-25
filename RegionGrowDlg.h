#pragma once

#include <QWidget>
#include "ui_RegionGrowDlg.h"
#include <pcl/segmentation/region_growing.h>//�����������Ʒָ��㷨

class RegionGrowDlg : public QWidget
{
	Q_OBJECT

public:
	RegionGrowDlg(QWidget *parent = nullptr);
	~RegionGrowDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void RegionGrow(int KSearch, int NumberOfNeighbours, double SmoothnessThreshold, double CurvatureThreshold, int MinClusterSize, int MaxClusterSize);

private:
	Ui::RegionGrowDlgClass ui;
};
