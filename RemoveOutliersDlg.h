#pragma once

#include <QWidget>
#include "ui_RemoveOutliersDlg.h"
#include <pcl/filters/radius_outlier_removal.h>

class RemoveOutliersDlg : public QWidget
{
	Q_OBJECT

public:
	RemoveOutliersDlg(QWidget *parent = nullptr);
	~RemoveOutliersDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void RemoveOutliers(int Knum, double Thresh, int KeepOrganized);

private:
	Ui::RemoveOutliersDlgClass ui;
};
