#include "FLCModule.h"

#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>

FLCModule::FLCModule(QWidget *parent, const QString functionalName, const QIcon *icon)
	: QWidget(parent),
	  m_pModuleIcon(icon)
{
	QPalette p = palette();
	QBrush b = p.brush(QPalette::ColorRole::Background);
	b.setColor(QColor(200,100,100));
	p.setBrush(QPalette::ColorRole::Background, b);
	setPalette(p);
	setMinimumSize(120,45);
	m_pVerticalLayout = new QVBoxLayout(this);
	m_pHorizontalLayot = new QHBoxLayout();
	m_pFunctionalLabel = new QLabel(functionalName, this);
	QLabel *iconLabel = new QLabel(this);
	iconLabel->setPixmap(icon->pixmap(QSize(20,20)));
	m_pHorizontalLayot->addWidget(iconLabel);
	m_pHorizontalLayot->addWidget(m_pFunctionalLabel);
	m_pVerticalLayout->insertLayout(-1, m_pHorizontalLayot);
	setLayout(m_pVerticalLayout);
}
