#ifndef FLCMODULE_H
#define FLCMODULE_H

#include <QObject>
#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;
class QIcon;
class QLabel;

class FLCModule : public QWidget
{
	Q_OBJECT
public:
	explicit FLCModule(QWidget *parent, const QString functionalName, const QIcon *icon);

signals:

public slots:

private:
	QHBoxLayout *m_pHorizontalLayot;
	QVBoxLayout *m_pVerticalLayout;
	const QIcon *m_pModuleIcon;
	QLabel *m_pFunctionalLabel;
};

#endif // FLCMODULE_H
