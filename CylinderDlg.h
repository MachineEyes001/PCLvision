#pragma once

#include <QWidget>
#include "ui_CylinderDlg.h"
#include <pcl/sample_consensus/method_types.h>//模型定义头文件
#include <pcl/sample_consensus/model_types.h>//随机参数估计方法头文件
#include <pcl/segmentation/sac_segmentation.h>//基于采样一致性分割的类的头文件
#include <pcl/features/normal_3d.h>

class CylinderDlg : public QWidget
{
	Q_OBJECT

public:
	CylinderDlg(QWidget *parent = nullptr);
	~CylinderDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void RANSAC(int model, double localpoint, int iteration, int minpointnum, double confidence, int KdTreeSearhNum=0, int ModelOPTI=0, double NormalWeight=0, int ifReverse=0, double MinR=0, double MaxR=0);

private:
	Ui::CylinderDlgClass ui;
};
