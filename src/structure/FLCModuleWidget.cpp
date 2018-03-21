#include "FLCModuleWidget.h"

#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QPainter>
#include <QMenu>

FLCModuleWidget::FLCModuleWidget(QWidget *parent, const QString functionalName, const QIcon icon, const QString moduleName)
	: QWidget(parent),
	  m_pModuleIcon(icon),
	  m_sFunctionalName(functionalName)
{
	QPalette p = palette();
	QBrush b = p.brush(QPalette::ColorRole::Background);
	setStyleSheet("font-size:8pt");
	b.setColor(QColor(200,200,200));
	p.setBrush(QPalette::ColorRole::Background, b);
	setPalette(p);
	setMinimumSize(115,45);
	setMaximumSize(115,45);
	setValid(UNDEFINED);
	setAutoFillBackground(true);
	m_pVerticalLayout = new QVBoxLayout(this);
	m_pHorizontalLayot = new QHBoxLayout();
	QLabel *functionalLabel = new QLabel(this);
	if( moduleName != "")
		functionalLabel->setText(moduleName);
	else
		functionalLabel->setText(functionalName);
	functionalLabel->setWordWrap(true);
	QLabel *iconLabel = new QLabel(this);
	iconLabel->setPixmap(icon.pixmap(QSize(20,20)));
	iconLabel->setStyleSheet("padding-left:0;");
	m_pHorizontalLayot->addSpacing(16);
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
		case LOCKED:
			borderColor.setRgb(100,100,100);
			break;
	}
	repaint();
}

void FLCModuleWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.save();
	QPen p = QPen(borderColor);
	if( seleced() )
		p.setColor(borderColor.lighter());
	p.setWidth(2);
	painter.setPen(p);
	painter.drawRect(2,2,width()-4, height()-4);
	p.setColor(QColor(190,190,190));
	p.setWidth(3);
	painter.setPen(p);
	for( int y = 8; y <= 37; y+=7)
		painter.drawLine(4,y,11,y);
	painter.drawLine(14,4,14,40);
	p.setColor(QColor(170,170,170));
	p.setWidth(1);
	painter.setPen(p);
	for( int y = 10; y <= 38; y+=7)
		painter.drawLine(4,y,11,y);
	painter.drawLine(16,4,16,40);
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

void FLCModuleWidget::mouseReleaseEvent(QMouseEvent *event __attribute__((unused)) )
{
	emit( widgetSelected(this) );
}

bool FLCModuleWidget::seleced() const
{
	return m_bSeleced;
}

void FLCModuleWidget::setSeleced(bool bSeleced)
{
	qDebug() << "selected: " << bSeleced;
	m_bSeleced = bSeleced;
	repaint();
}

void FLCModuleWidget::createActions()
{
	this->m_pRemoveAction = std::make_unique<QAction>(tr("&Remove"), this);
	connect(this->m_pRemoveAction.get(), &QAction::triggered, [this] {emit(this->removeModuleWidget(this->m_uuid));});
}
