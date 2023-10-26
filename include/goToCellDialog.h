#pragma once

#include "ui_goToCellDialog.h"
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtCore/QRegularExpression>
#include <QtGui/QRegularExpressionValidator>

class goToCellDialog : public QDialog, public Ui::goToCellDialog {
	Q_OBJECT
private slots:
	void on_lineEdit_textChanged();
public:
	goToCellDialog(QWidget* parent = 0);
};


