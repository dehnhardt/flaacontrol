#include "FLCModuleWidget.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"

#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QPainter>
#include <QMenu>
#include <QGraphicsEllipseItem>

FLCModuleWidget::FLCModuleWidget(QWidget *parent, const QIcon icon, FLOModuleInstanceDAO *instanceData)
	: QWidget(parent),
	  m_pModuleIcon(icon)
{
	createGUI();
	iconLabel->setPixmap(icon.pixmap(QSize(20,20)));
	setData(instanceData);
}

void FLCModuleWidget::addPorts(int portsCount, QVector<QPainterPath> &v, flaarlib::MODULE_TYPE moduleType)
{
	int radius = 8;
	int y = 0;
	if( moduleType == flaarlib::MODULE_TYPE::OUTPUT)
		y = height();
	v.clear();
	int distance = (width() - (m_iLeftOffset + 5))
				   / (portsCount + 1);
	for( int i = 0; i < portsCount; i++)
	{
		QPainterPath path;
		QFont myfont = font();
		myfont.setPointSize(6);
		QPoint point( m_iLeftOffset + ((i+1) * distance), y);
		path.addEllipse(point, radius, radius );
		//path.addText(point, myfont, "\u25bc");
		v.append(path);
	}
}

void FLCModuleWidget::setData(FLOModuleInstanceDAO *instanceData)
{
	this->m_uuid = instanceData->uuid();
	this->m_sModuleName = instanceData->moduleName();
	this->m_sFunctionalName = instanceData->moduleFunctionalName();
	if( m_iInputPortsCount != instanceData->getParameter("inputPorts")->value().toInt() )
	{
		this->m_iInputPortsCount = instanceData->getParameter("inputPorts")->value().toInt();
		addPorts(m_iInputPortsCount, m_vInputPorts, flaarlib::MODULE_TYPE::INPUT);
	}
	if( this->m_iOutputPortsCount != instanceData->getParameter("outputPorts")->value().toInt() )
	{
		this->m_iOutputPortsCount = instanceData->getParameter("outputPorts")->value().toInt();
		addPorts(m_iOutputPortsCount, m_vOutputPorts, flaarlib::MODULE_TYPE::OUTPUT);
	}
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
			borderColor.setRgb(60, 160, 60);
			break;
		case INVALID:
			borderColor.setRgb(160,60,60);
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

void FLCModuleWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPoint p = event->pos();
	for(QPainterPath path : m_vInputPorts )
	{
		if( path.contains(p) )
		{
			setCursor(Qt::CrossCursor);
			return;
		}
	}
	for(QPainterPath path : m_vOutputPorts )
	{
		if( path.contains(p) )
		{
			setCursor(Qt::CrossCursor);
			return;
		}
	}
	unsetCursor();
}

void FLCModuleWidget::createGUI()
{
	QPalette p = palette();
	QBrush b = p.brush(QPalette::ColorRole::Background);
	setMouseTracking(true);
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
	return m_iOutputPortsCount;
}

void FLCModuleWidget::setOutputPorts(int iOutputPorts)
{
	m_iOutputPortsCount = iOutputPorts;
}

int FLCModuleWidget::inputPorts() const
{
	return m_iInputPortsCount;
}

void FLCModuleWidget::setInputPorts(int iInputPorts)
{
	m_iInputPortsCount = iInputPorts;
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
	paintPorts(painter);
	painter.restore();
	QWidget::paintEvent(e);
}


void FLCModuleWidget::paintPorts(QPainter &painter)
{
	QColor portsColor(120, 120, 120);
	QPen pen(this->seleced()? borderColor.lighter():borderColor);
	pen.setWidth(2);
	painter.setPen(pen);
	for( QPainterPath path : m_vInputPorts)
	{
		painter.fillPath(path, portsColor);
		painter.drawPath(path);
	}
	for( QPainterPath path : m_vOutputPorts)
	{
		painter.fillPath(path, portsColor);
		painter.drawPath(path);
	}
}
