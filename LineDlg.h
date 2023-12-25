#pragma once

#include <QWidget>
#include "ui_LineDlg.h"

class LineDlg : public QWidget
{
	Q_OBJECT

public:
	LineDlg(QWidget *parent = nullptr);
	~LineDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void RANSAC(int model, double localpoint, int iteration, int minpointnum, double confidence, int KdTreeSearhNum = 0, int ModelOPTI = 0, double NormalWeight = 0, int ifReverse = 0, double MinR = 0, double MaxR = 0);

private:
	Ui::LineDlgClass ui;
};
