#ifndef FLCMODULE_H
#define FLCMODULE_H

#include <QObject>
#include <QWidget>
#include <QIcon>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;

class FLCModule : public QWidget
{
	Q_OBJECT
public:
	explicit FLCModule(QWidget *parent, const QString functionalName, const QIcon icon);

	QIcon moduleIcon() const
	{
		return  m_pModuleIcon;
	}
	QString functionalName() const
	{
		return  m_sFunctionalName;
	}

signals:

public slots:

private:
	QHBoxLayout *m_pHorizontalLayot;
	QVBoxLayout *m_pVerticalLayout;
	QIcon m_pModuleIcon;
	QString m_sFunctionalName;
};

#endif // FLCMODULE_H
