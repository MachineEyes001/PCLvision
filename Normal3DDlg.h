#pragma once

#include <QWidget>
#include "ui_Normal3DDlg.h"

class Normal3DDlg : public QWidget
{
	Q_OBJECT

public:
	Normal3DDlg(QWidget *parent = nullptr);
	~Normal3DDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void Normal3D(double SearchRadius,int Level,double Scale);

private:
	Ui::Normal3DDlgClass ui;
};
