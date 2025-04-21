#pragma once

#include <QWidget>
#include "ui_UpSamplingDlg.h"
#include <pcl/surface/mls.h>

class UpSamplingDlg : public QWidget
{
	Q_OBJECT

public:
	UpSamplingDlg(QWidget *parent = nullptr);
	~UpSamplingDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void UpSampling(double SearchRadius, double SamplingRadius, double SamplingStep,int KeepOrganized);

private:
	Ui::UpSamplingDlgClass ui;
};
