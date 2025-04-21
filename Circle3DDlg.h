#pragma once

#include <QWidget>
#include "ui_Circle3DDlg.h"

class Circle3DDlg : public QWidget
{
	Q_OBJECT

public:
	Circle3DDlg(QWidget *parent = nullptr);
	~Circle3DDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void RANSAC(int model, double localpoint, int iteration, int minpointnum, double confidence, int KdTreeSearhNum = 0, int ModelOPTI = 0, double NormalWeight = 0, int ifReverse = 0, double MinR = 0, double MaxR = 0);

private:
	Ui::Circle3DDlgClass ui;
};
