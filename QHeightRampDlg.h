#pragma once

#include <QWidget>
#include "ui_QHeightRampDlg.h"

class QHeightRampDlg : public QWidget
{
	Q_OBJECT

public:
	QHeightRampDlg(QWidget *parent = nullptr);
	~QHeightRampDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void setHeightRamp(int dir, double height);

private:
	Ui::QHeightRampDlgClass ui;
};
