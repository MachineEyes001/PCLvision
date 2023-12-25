#pragma once

#include <QWidget>
#include "ui_PolygonDlg.h"
#include <pcl/surface/concave_hull.h>                 //创建凹多边形类定义头文件

class PolygonDlg : public QWidget
{
	Q_OBJECT

public:
	PolygonDlg(QWidget *parent = nullptr);
	~PolygonDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void Polygon(double Alpha);

private:
	Ui::PolygonDlgClass ui;
};
