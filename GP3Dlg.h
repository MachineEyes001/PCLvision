#pragma once

#include <QWidget>
#include "ui_GP3Dlg.h"
#include <pcl/surface/gp3.h>      //̰��ͶӰ���ǻ��㷨

class GP3Dlg : public QWidget
{
	Q_OBJECT

public:
	GP3Dlg(QWidget *parent = nullptr);
	~GP3Dlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void GP3(int KSearch,double SearchRadius,double Mu,int MaximumNearestNeighbors,double MaximumSurfaceAngle,double MinimumAngle,double MaximumAngle,int NormalConsistency);

private:
	Ui::GP3DlgClass ui;
};
