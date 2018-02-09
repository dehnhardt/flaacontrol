#ifndef FLCMODULEWIDGET_H
#define FLCMODULEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QIcon>
#include <QUuid>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;

class FLCModuleWidget : public QWidget
{
	Q_OBJECT
public:
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

signals:

public slots:

private:
	QHBoxLayout *m_pHorizontalLayot;
	QVBoxLayout *m_pVerticalLayout;
	QIcon m_pModuleIcon;
	QString m_sFunctionalName;
	QUuid m_uuid;
};

#endif // FLCMODULEWIDGET_H
