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

void FLCModuleWidget::addPorts(int portsCount, QVector<QPainterPath> &v, flaarlib::PORT_TYPE portType)
{
	int radius = 8;
	int y = 0;
	if( portType == flaarlib::PORT_TYPE::OUTPUT_PORT)
		y = height();
	v.clear();
	int distance = (width() - (m_iHandleWidth + 5))
				   / (portsCount + 1);
	for( int i = 0; i < portsCount; i++)
	{
		QPainterPath path;
		QFont myfont = font();
		myfont.setPointSize(6);
		QPoint point( m_iHandleWidth + ((i+1) * distance), y);
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
		addPorts(m_iInputPortsCount, m_vInputPorts, flaarlib::PORT_TYPE::INPUT_PORT);
	}
	if( this->m_iOutputPortsCount != instanceData->getParameter("outputPorts")->value().toInt() )
	{
		this->m_iOutputPortsCount = instanceData->getParameter("outputPorts")->value().toInt();
		addPorts(m_iOutputPortsCount, m_vOutputPorts, flaarlib::PORT_TYPE::OUTPUT_PORT);
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
			borderColor.setRgb(117, 199, 203);
			break;
		case INVALID:
			borderColor.setRgb(223, 53, 23);
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

void FLCModuleWidget::mousePressEvent(QMouseEvent *event)
{
	QPoint p = event->pos();
	int inputPort = 0;
	int outputPort = 0;
	inputPort = inInputPort(p);
	if( inputPort )
	{
		qDebug() << "click on input port";
		emit(portClicked(this, flaarlib::PORT_TYPE::INPUT_PORT, inputPort));
		event->ignore();
		return;
	}
	outputPort = inOutputPort(p);
	if( outputPort)
	{
		qDebug() << "click on output port";
		emit(portClicked(this, flaarlib::PORT_TYPE::OUTPUT_PORT, outputPort));
		event->ignore();
		return;
	}
	if( inHandleArea( p ))
	{
		m_bMoving = true;
		m_pOffset = event->pos();
	}
	QWidget::mousePressEvent(event);
}

void FLCModuleWidget::mouseReleaseEvent(QMouseEvent *event __attribute__((unused)) )
{
	QPoint p = event->pos();
	if( inInputPort(p))
	{
		event->ignore();
		return;
	}
	if( inOutputPort(p))
	{
		event->ignore();
		return;
	}
	if( inHandleArea(p))
	{
		if( m_bMoving )
		{
			QPoint newPos = snapToGrid(pos(), QPoint(0,0));
			move(newPos);
			m_bMoving = false;
			emit( widgetMoved(this, newPos));
		}
	}
	emit( widgetSelected(this) );
}

void FLCModuleWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPoint p = event->pos();
	if( inInputPort(p))
	{
		setCursor(Qt::PointingHandCursor);
		event->ignore();
		return;
	}
	if( inOutputPort(p))
	{
		setCursor(Qt::PointingHandCursor);
		event->ignore();
		return;
	}
	if( inHandleArea(p) )
	{
		if( !event->buttons() )
			setCursor(Qt::OpenHandCursor);
		if(event->buttons() & Qt::LeftButton)
			this->move(mapToParent(event->pos() - m_pOffset));
		return;
	}
	unsetCursor();
}

QPoint FLCModuleWidget::snapToGrid(QPoint position, QPoint offset)
{
	int snap = 10;
	QPoint p = position - offset;
	p.setX(static_cast<int>(round(static_cast<double>(p.x())/snap)
							*static_cast<int>(snap)));
	p.setY(static_cast<int>(round(static_cast<double>(p.y())/snap)
							*static_cast<int>(snap)));
	return p;
}

int FLCModuleWidget::inInputPort(QPoint p)
{
	for (int i = 0; i < m_vInputPorts.size(); ++i)
	{
		if( m_vInputPorts.at(i).contains(p))
			return i+1;
	}
	return 0;
}

int FLCModuleWidget::inOutputPort(QPoint p)
{
	for (int i = 0; i < m_vOutputPorts.size(); ++i)
	{
		if( m_vOutputPorts.at(i).contains(p))
			return i+1;
	}
	return 0;
}

int FLCModuleWidget::inHandleArea(QPoint p)
{
	if( m_handleRect.contains(p))
		return 1;
	return 0;
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
	setFixedSize(m_iWidth, m_iHeight);
	//setMinimumSize(m_iWidth,m_iHeight);
	//setMaximumSize(m_iWidth,m_iHeight);
	setValid(UNDEFINED);
	setAutoFillBackground(true);
	m_pVerticalLayout = new QVBoxLayout(this);
	m_pVerticalLayout->addSpacing(5);
	m_pHorizontalLayot = new QHBoxLayout();
	functionalLabel = new QLabel(this);
	functionalLabel->setAlignment(Qt::AlignCenter);
	functionalLabel->setWordWrap(true);
	functionalLabel->setMinimumWidth(70);
	functionalLabel->setFrameShape(QFrame::Panel);
	functionalLabel->setFrameShadow(QFrame::Sunken);
	functionalLabel->setMouseTracking(true);
	iconLabel = new QLabel(this);
	iconLabel->setStyleSheet("padding-left:0;");
	m_pHorizontalLayot->addSpacing(m_iHandleWidth);
	m_pHorizontalLayot->addWidget(iconLabel);
	m_pHorizontalLayot->addWidget(functionalLabel);
	m_pVerticalLayout->insertLayout(-1, m_pHorizontalLayot);
	m_pVerticalLayout->addSpacing(5);
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

QPoint FLCModuleWidget::getPortOrigin(flaarlib::PORT_TYPE portType, int portNumber)
{
	QRect r;
	QPainterPath path;
	if( portNumber < 1 )
		return QPoint(-1, -1);
	if(portType == flaarlib::PORT_TYPE::OUTPUT_PORT)
	{
		if( portNumber > m_vOutputPorts.size())
			return  QPoint(-1,-1);
		path = m_vOutputPorts.at(portNumber-1);
	}
	else if(portType == flaarlib::PORT_TYPE::INPUT_PORT)
	{
		if( portNumber > m_vInputPorts.size())
			return  QPoint(-1,-1);
		path = m_vInputPorts.at(portNumber-1);
	}
	else
		return QPoint(-1,-1);
	r = path.boundingRect().toRect();
	return mapToParent(QPoint( r.x() + r.width()/2, r.top() + r.height() / 2 ));
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
	for( int y = 8; y <= 47; y+=7)
		painter.drawLine(4,y,11,y);
	painter.drawLine(14,4,14,50);
	p.setColor(QColor(170,170,170));
	p.setWidth(1);
	painter.setPen(p);
	for( int y = 10; y <= 48; y+=7)
		painter.drawLine(4,y,11,y);
	painter.drawLine(m_iHandleWidth,4,m_iHandleWidth,50);
	paintPorts(painter);
	painter.restore();
	QWidget::paintEvent(e);
}


void FLCModuleWidget::paintPorts(QPainter &painter)
{
	QColor portsColor = QWidget::palette().color(backgroundRole());
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
