#ifndef FLCMODULEWIDGET_H
#define FLCMODULEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QIcon>
#include <QUuid>
#include <QDebug>
#include <QApplication>
#include <QMoveEvent>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;

class FLCModuleWidget : public QWidget
{
	Q_OBJECT
public:
	enum VALIDITY
	{
		VALID=1,
		INVALID = 2,
		UNDEFINED = 3
	};
	explicit FLCModuleWidget(QWidget *parent, const QString functionalName, const QIcon icon);

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

signals:

public slots:

protected:
	void mouseMoveEvent(QMouseEvent *event) override
	{
		qDebug() << "mouseMoveEvent MW";
		QApplication::sendEvent(parentWidget(), event);
	}

private:
	QHBoxLayout *m_pHorizontalLayot;
	QVBoxLayout *m_pVerticalLayout;
	QIcon m_pModuleIcon;
	QString m_sFunctionalName;
	QUuid m_uuid;
	QColor borderColor = QColor(100,100,100);


	// QWidget interface
protected:
	void paintEvent(QPaintEvent *e) override;
};

#endif // FLCMODULEWIDGET_H
