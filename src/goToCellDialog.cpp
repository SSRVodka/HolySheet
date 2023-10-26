#include "goToCellDialog.h"

goToCellDialog::goToCellDialog(QWidget* parent)
	: QDialog(parent) {
	setupUi(this);
	QRegularExpression reg("[a-zA-Z][1-9][0-9]{0,2}");
	lineEdit->setValidator(new QRegularExpressionValidator(reg, this));
	
	connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

void goToCellDialog::on_lineEdit_textChanged() {
	okBtn->setEnabled(lineEdit->hasAcceptableInput());
}
