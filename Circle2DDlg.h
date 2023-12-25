#pragma once

#include <QWidget>
#include "ui_Circle2DDlg.h"

class Circle2DDlg : public QWidget
{
	Q_OBJECT

public:
	Circle2DDlg(QWidget *parent = nullptr);
	~Circle2DDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void RANSAC(int model, double localpoint, int iteration, int minpointnum, double confidence, int KdTreeSearhNum = 0, int ModelOPTI = 0, double NormalWeight = 0, int ifReverse = 0, double MinR = 0, double MaxR = 0);

private:
	Ui::Circle2DDlgClass ui;
};
