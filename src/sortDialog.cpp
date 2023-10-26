#include "sortDialog.h"

sortDialog::sortDialog(QWidget* parent)
	: QDialog(parent) {
	setupUi(this);
	
	secondaryGB->hide();
	tertiaryGB->hide();
	
	layout()->setSizeConstraint(QLayout::SetFixedSize);
	
	setColumnRange('A', 'Z');
}

void sortDialog::setColumnRange(QChar start, QChar last) {
	primaryGBColBox->clear();
	secondaryGBColBox->clear();
	tertiaryGBColBox->clear();
	
	secondaryGBColBox->addItem(tr("None"));
	tertiaryGBColBox->addItem(tr("None"));
	primaryGBColBox->setMinimumSize(secondaryGBColBox->sizeHint());

	QChar cur = start;
	while (cur <= last) {
		primaryGBColBox->addItem(QString(cur));
		secondaryGBColBox->addItem(QString(cur));
		tertiaryGBColBox->addItem(QString(cur));
		cur = cur.unicode() + 1;
	}
}

void sortDialog::on_moreBtn_toggled(bool state) {
	if (state) moreBtn->setText(tr("&Less"));
	else moreBtn->setText(tr("&More"));
}