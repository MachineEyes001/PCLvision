#pragma once

#include <QWidget>
#include "ui_PassThroughDlg.h"

class PassThroughDlg : public QWidget
{
	Q_OBJECT

public:
	PassThroughDlg(QWidget *parent = nullptr);
	~PassThroughDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void PassThrough(int dir, double min, double max, int KeepOrganized);

private:
	Ui::PassThroughDlgClass ui;
};
