#ifndef FLCMODULEWIDGET_H
#define FLCMODULEWIDGET_H

#include "FLModuleDefs.h"

#include <memory>

#include <QObject>
#include <QWidget>
#include <QIcon>
#include <QUuid>
#include <QDebug>
#include <QApplication>
#include <QMoveEvent>
#include <QAction>
#include <QPainterPath>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class FLOModuleInstanceDAO;

class FLCModuleWidget : public QWidget
{
	Q_OBJECT
public:
	enum VALIDITY
	{
		UNDEFINED = 0,
		VALID=1,
		INVALID = 2,
		LOCKED = 3
	};

	explicit FLCModuleWidget(QWidget *parent, const QIcon icon, FLOModuleInstanceDAO *instanceData);

	void setData(FLOModuleInstanceDAO *instanceData);

	QIcon moduleIcon() const
	{
		return  m_pModuleIcon;
	}
	QString functionalName() const
	{
		return  m_sFunctionalName;
	}

	QUuid getUuid() const
	{
		return m_uuid;
	}

	void setUuid(const QUuid &uuid)
	{
		m_uuid = uuid;
	}

	void setValid( VALIDITY validity);

	bool seleced() const;
	void setSeleced(bool bSeleced);

	QString moduleName() const;
	void setModuleName(const QString &sModuleName);

	int inputPorts() const;
	void setInputPorts(int iInputPorts);

	int outputPorts() const;
	void setOutputPorts(int iOutputPorts);

	QPoint getPortOrigin( flaarlib::PORT_TYPE portType, int portNumber);

signals:
	void removeModuleWidget( QUuid uuid);
	void widgetSelected(FLCModuleWidget *flcModuleWidget);
	void portClicked( FLCModuleWidget *flcModuleWidget, flaarlib::PORT_TYPE portType, int portNumber );

public slots:

protected:
	void paintEvent(QPaintEvent *e) override;
	void contextMenuEvent(QContextMenuEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

	int inInputPort(QPoint p);
	int inOutputPort(QPoint p);
	int inHandleArea(QPoint p);

private: //members
	bool m_bSeleced = false;
	QHBoxLayout *m_pHorizontalLayot;
	QVBoxLayout *m_pVerticalLayout;
	QIcon m_pModuleIcon;

	QLabel *iconLabel = 0;
	QLabel *functionalLabel = 0;

	QColor borderColor = QColor(100,100,100);

	QString m_sModuleName;
	QString m_sFunctionalName;
	int m_iInputPortsCount;
	int m_iOutputPortsCount;
	QUuid m_uuid;

	int m_iHandleWidth = 16;
	int m_iHeight = 55;
	int m_iWidth = 125;
	QRect m_handleRect = QRect(0,0,m_iHandleWidth,m_iHeight);

	// context menu action
	std::unique_ptr<QAction> m_pRemoveAction;

private: //methods
	void createGUI();
	void createActions();
	void paintPorts(QPainter &painter);

	QVector<QPainterPath> m_vInputPorts;
	QVector<QPainterPath> m_vOutputPorts;

	void addPorts(int portsCount, QVector<QPainterPath> &path, flaarlib::PORT_TYPE portType);

};

#endif // FLCMODULEWIDGET_H
