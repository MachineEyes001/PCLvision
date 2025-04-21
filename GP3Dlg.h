#pragma once

#include <QWidget>
#include "ui_GP3Dlg.h"
#include <pcl/surface/gp3.h>      //贪婪投影三角化算法

class GP3Dlg : public QWidget
{
	Q_OBJECT

public:
	GP3Dlg(QWidget *parent = nullptr);
	~GP3Dlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void GP3(int KSearch,double SearchRadius,double Mu,int MaximumNearestNeighbors,double MaximumSurfaceAngle,double MinimumAngle,double MaximumAngle,int NormalConsistency);

private:
	Ui::GP3DlgClass ui;
};
