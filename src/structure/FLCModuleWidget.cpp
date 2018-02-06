#include "FLCModuleWidget.h"

#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>

FLCModuleWidget::FLCModuleWidget(QWidget *parent, const QString functionalName, const QIcon icon)
	: QWidget(parent),
	  m_pModuleIcon(icon),
	  m_sFunctionalName(functionalName)
{
	QPalette p = palette();
	QBrush b = p.brush(QPalette::ColorRole::Background);
	setAutoFillBackground(true);
	b.setColor(QColor(200,200,200));
	p.setBrush(QPalette::ColorRole::Background, b);
	setPalette(p);
	setMinimumSize(100,20);
	m_pVerticalLayout = new QVBoxLayout(this);
	m_pHorizontalLayot = new QHBoxLayout();
	QLabel *functionalLabel = new QLabel(m_sFunctionalName, this);
	QLabel *iconLabel = new QLabel(this);
	iconLabel->setPixmap(icon.pixmap(QSize(20,20)));
	m_pHorizontalLayot->addWidget(iconLabel);
	m_pHorizontalLayot->addWidget(functionalLabel);
	m_pVerticalLayout->insertLayout(-1, m_pHorizontalLayot);
	setLayout(m_pVerticalLayout);
}
