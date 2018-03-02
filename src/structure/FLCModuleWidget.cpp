#include "FLCModuleWidget.h"

#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QPainter>
#include <QMenu>

FLCModuleWidget::FLCModuleWidget(QWidget *parent, const QString functionalName, const QIcon icon)
	: QWidget(parent),
	  m_pModuleIcon(icon),
	  m_sFunctionalName(functionalName)
{
	QPalette p = palette();
	QBrush b = p.brush(QPalette::ColorRole::Background);
	b.setColor(QColor(200,200,200));
	p.setBrush(QPalette::ColorRole::Background, b);
	setPalette(p);
	setMinimumSize(100,20);
	setValid(UNDEFINED);
	setAutoFillBackground(true);
	m_pVerticalLayout = new QVBoxLayout(this);
	m_pHorizontalLayot = new QHBoxLayout();
	QLabel *functionalLabel = new QLabel(m_sFunctionalName, this);
	QLabel *iconLabel = new QLabel(this);
	iconLabel->setPixmap(icon.pixmap(QSize(20,20)));
	m_pHorizontalLayot->addWidget(iconLabel);
	m_pHorizontalLayot->addWidget(functionalLabel);
	m_pVerticalLayout->insertLayout(-1, m_pHorizontalLayot);
	createActions();
	setLayout(m_pVerticalLayout);
}

void FLCModuleWidget::setValid(FLCModuleWidget::VALIDITY validity)
{
	switch( validity)
	{
		case UNDEFINED:
			borderColor.setRgb(100,100,100);
			break;
		case VALID:
			borderColor.setRgb(100, 200, 100);
			break;
		case INVALID:
			borderColor.setRgb(200,100,100);
			break;
	}
	repaint();
}

void FLCModuleWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.save();
	QPen p = QPen(borderColor);
	p.setWidth(2);
	painter.setPen(p);
	painter.drawRect(2,2,width()-4, height()-4);
	painter.restore();
	QWidget::paintEvent(e);
}

#ifndef QT_NO_CONTEXTMENU
void FLCModuleWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	menu.addAction(m_pRemoveAction.get());
	menu.exec(event->globalPos());
}
#endif

void FLCModuleWidget::createActions()
{
	this->m_pRemoveAction = std::make_unique<QAction>(tr("&Remove"), this);
	connect(this->m_pRemoveAction.get(), &QAction::triggered, [this] {emit(this->removeModuleWidget(this->m_uuid));});
}
