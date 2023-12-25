#pragma once

#include <QWidget>
#include "ui_IntegralImageDlg.h"
#include <pcl/features/integral_image_normal.h>

class IntegralImageDlg : public QWidget
{
	Q_OBJECT

public:
	IntegralImageDlg(QWidget *parent = nullptr);
	~IntegralImageDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void IntegralImage(int EstimationMethod, double DepthChangeFactor, double NormalSmoothingSize, int Level, double Scale);

private:
	Ui::IntegralImageDlgClass ui;
};
