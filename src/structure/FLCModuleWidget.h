#ifndef FLCMODULEWIDGET_H
#define FLCMODULEWIDGET_H

#include <memory>

#include <QObject>
#include <QWidget>
#include <QIcon>
#include <QUuid>
#include <QDebug>
#include <QApplication>
#include <QMoveEvent>
#include <QAction>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;

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
	explicit FLCModuleWidget(QWidget *parent, const QString fuctionalName, const QIcon icon, const QString moduleName = "");

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

signals:
	void removeModuleWidget( QUuid uuid);
	void widgetSelected(FLCModuleWidget *flcModuleWidget);

public slots:

protected:
	void paintEvent(QPaintEvent *e) override;
	void contextMenuEvent(QContextMenuEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event);

private: //members
	bool m_bSeleced = false;
	QHBoxLayout *m_pHorizontalLayot;
	QVBoxLayout *m_pVerticalLayout;
	QIcon m_pModuleIcon;
	QString m_sFunctionalName;
	QUuid m_uuid;
	QColor borderColor = QColor(100,100,100);

	// context menu action
	std::unique_ptr<QAction> m_pRemoveAction;

private: //methods
	void createActions();

	// QWidget interface
};

#endif // FLCMODULEWIDGET_H
