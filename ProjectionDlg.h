#pragma once

#include <QWidget>
#include "ui_ProjectionDlg.h"
#include <pcl/ModelCoefficients.h>             //模型系数头文件
#include <pcl/filters/project_inliers.h>       //投影滤波类头文件

class ProjectionDlg : public QWidget
{
	Q_OBJECT

public:
	ProjectionDlg(QWidget *parent = nullptr);
	~ProjectionDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void Projection(int plane,int KeepOrganized);

private:
	Ui::ProjectionDlgClass ui;
};
