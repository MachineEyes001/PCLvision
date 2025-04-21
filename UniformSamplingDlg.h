#pragma once

#include <QWidget>
#include "ui_UniformSamplingDlg.h"
#include <pcl/filters/uniform_sampling.h>

class UniformSamplingDlg : public QWidget
{
	Q_OBJECT

public:
	UniformSamplingDlg(QWidget *parent = nullptr);
	~UniformSamplingDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void UniformSampling(double radius,int KeepOrganized);

private:
	Ui::UniformSamplingDlgClass ui;
};
