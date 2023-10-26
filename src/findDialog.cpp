#include "findDialog.h"

findDialog::findDialog(QWidget* parent) {
	findLabel = new QLabel(tr("Find &what"));
	lineEdit = new QLineEdit;
	findLabel->setBuddy(lineEdit);

	matchCheck = new QCheckBox(tr("Match &case"));
	matchCheck->setChecked(true);
	backCheck = new QCheckBox(tr("Search &backward"));
	
	findBtn = new QPushButton(tr("&Find"));
	findBtn->setDefault(true);
	findBtn->setEnabled(false);
	closeBtn = new QPushButton(tr("Close"));

	connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkFindButton(const QString&)));
	connect(findBtn, SIGNAL(clicked()), this, SLOT(findClicked()));
	connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
	
	QHBoxLayout* LU = new QHBoxLayout;
	LU->addWidget(findLabel);
	LU->addWidget(lineEdit);
	QVBoxLayout* LL = new QVBoxLayout;
	LL->addLayout(LU);
	LL->addWidget(matchCheck);
	LL->addWidget(backCheck);
	QVBoxLayout* R = new QVBoxLayout;
	R->addWidget(findBtn);
	R->addWidget(closeBtn);
	R->addStretch();
	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addLayout(LL);
	mainLayout->addLayout(R);
	
	setLayout(mainLayout);
	setWindowTitle(tr("Find"));
	setFixedHeight(sizeHint().height());
}

void findDialog::findClicked() {
	QString text = lineEdit->text();
	Qt::CaseSensitivity cs = (matchCheck->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
	if (backCheck->isChecked()) emit findPrev(text, cs);
	else emit findNext(text, cs);
}

void findDialog::checkFindButton(const QString& text) {
	findBtn->setEnabled(!text.isEmpty());
}