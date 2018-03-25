#include "FLCModuleWidget.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"

#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QPainter>
#include <QMenu>

FLCModuleWidget::FLCModuleWidget(QWidget *parent, const QIcon icon, FLOModuleInstanceDAO *instanceData)
	: QWidget(parent),
	  m_pModuleIcon(icon)
{
	createGUI();
	iconLabel->setPixmap(icon.pixmap(QSize(20,20)));
	setData(instanceData);
}

void FLCModuleWidget::setData(FLOModuleInstanceDAO *instanceData)
{
	this->m_uuid = instanceData->uuid();
	this->m_sModuleName = instanceData->moduleName();
	this->m_sFunctionalName = instanceData->moduleFunctionalName();
	this->m_iInputPorts = instanceData->getParameter("inputPorts")->value().toInt();
	this->m_iOutputPorts = instanceData->getParameter("outputPorts")->value().toInt();
	if( this->m_sModuleName != "")
		functionalLabel->setText(m_sModuleName);
	else
		functionalLabel->setText(m_sFunctionalName);
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

void FLCModuleWidget::createGUI()
{
	QPalette p = palette();
	QBrush b = p.brush(QPalette::ColorRole::Background);
	setStyleSheet("font-size:8pt;");
	b.setColor(QColor(200,200,200));
	p.setBrush(QPalette::ColorRole::Background, b);
	setPalette(p);
	setMinimumSize(125,45);
	setMaximumSize(125,45);
	setValid(UNDEFINED);
	setAutoFillBackground(true);
	m_pVerticalLayout = new QVBoxLayout(this);
	m_pHorizontalLayot = new QHBoxLayout();
	functionalLabel = new QLabel(this);
	functionalLabel->setAlignment(Qt::AlignCenter);
	functionalLabel->setWordWrap(true);
	functionalLabel->setMinimumWidth(70);
	functionalLabel->setFrameShape(QFrame::Panel);
	functionalLabel->setFrameShadow(QFrame::Sunken);
	iconLabel = new QLabel(this);
	iconLabel->setStyleSheet("padding-left:0;");
	m_pHorizontalLayot->addSpacing(16);
	m_pHorizontalLayot->addWidget(iconLabel);
	m_pHorizontalLayot->addWidget(functionalLabel);
	m_pVerticalLayout->insertLayout(-1, m_pHorizontalLayot);
	createActions();
	setLayout(m_pVerticalLayout);
}

int FLCModuleWidget::outputPorts() const
{
	return m_iOutputPorts;
}

void FLCModuleWidget::setOutputPorts(int iOutputPorts)
{
	m_iOutputPorts = iOutputPorts;
}

int FLCModuleWidget::inputPorts() const
{
	return m_iInputPorts;
}

void FLCModuleWidget::setInputPorts(int iInputPorts)
{
	m_iInputPorts = iInputPorts;
}

QString FLCModuleWidget::moduleName() const
{
	return m_sModuleName;
}

void FLCModuleWidget::setModuleName(const QString &sModuleName)
{
	m_sModuleName = sModuleName;
	if( m_sModuleName != "" )
		this->functionalLabel->setText(m_sModuleName);
	else
		this->functionalLabel->setText(m_sFunctionalName);
}

bool FLCModuleWidget::seleced() const
{
	return m_bSeleced;
}

void FLCModuleWidget::setSeleced(bool bSeleced)
{
	m_bSeleced = bSeleced;
	repaint();
}

void FLCModuleWidget::createActions()
{
	this->m_pRemoveAction = std::make_unique<QAction>(tr("&Remove"), this);
	connect(this->m_pRemoveAction.get(), &QAction::triggered, [this] {emit(this->removeModuleWidget(this->m_uuid));});
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
	paintPorts(painter, flaarlib::MODULE_TYPE::INPUT );
	paintPorts(painter, flaarlib::MODULE_TYPE::OUTPUT );
	painter.restore();
	QWidget::paintEvent(e);
}


void FLCModuleWidget::paintPorts(QPainter &painter, flaarlib::MODULE_TYPE moduleType)
{
	int portCount = 0;
	int distance = 0;
	int leftOffset = 16;
	int radius = 8;
	int y = 0;
	QColor color = QColor(180,180,180);
	if( seleced() )
		color = QColor(222,222,222);
	QPainterPath path;
	if( moduleType == flaarlib::MODULE_TYPE::INPUT )
		portCount = inputPorts();
	else if( moduleType == flaarlib::MODULE_TYPE::OUTPUT)
	{
		portCount = outputPorts();
		y = height();
	}
	if(portCount == 0)
		return;
	painter.setPen(borderColor);
	distance = (width() - (leftOffset + 5))
			   / (portCount + 1);
	for( int i = 0; i < portCount; i++)
	{
		QPoint point( leftOffset + ((i+1) * distance), y);
		path.addEllipse(point, radius, radius );
	}
	painter.fillPath(path, color);
	painter.drawPath(path);

}
