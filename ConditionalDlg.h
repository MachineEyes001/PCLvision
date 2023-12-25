#pragma once

#include <QWidget>
#include "ui_ConditionalDlg.h"
#include <pcl/filters/conditional_removal.h>     //�����˲���

class ConditionalDlg : public QWidget
{
	Q_OBJECT

public:
	ConditionalDlg(QWidget *parent = nullptr);
	~ConditionalDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void Conditional(int KeepOrganized, double GT_X_d, double LT_X_d, double GT_Y_d, double LT_Y_d, double GT_Z_d, double LT_Z_d);

private:
	Ui::ConditionalDlgClass ui;
};
