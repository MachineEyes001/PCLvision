#pragma once

#include <QWidget>
#include "ui_RadiusFilterDlg.h"
#include <pcl/filters/statistical_outlier_removal.h>

class RadiusFilterDlg : public QWidget
{
	Q_OBJECT

public:
	RadiusFilterDlg(QWidget *parent = nullptr);
	~RadiusFilterDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void RadiusFilter(double radius, int minKnum,int KeepOrganized);

private:
	Ui::RadiusFilterDlgClass ui;
};
