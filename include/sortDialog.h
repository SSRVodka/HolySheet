#pragma once

#include "ui_sortDialog.h"
#include <QtWidgets/QDialog>

class sortDialog : public QDialog, public Ui::sortDialog {
	Q_OBJECT
public:
	sortDialog(QWidget* parent = 0);
	void setColumnRange(QChar start, QChar last);
private slots:
	void on_moreBtn_toggled(bool state);
};
