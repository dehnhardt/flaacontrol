#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#define OSCPKT_OSTREAM_OUTPUT

#include <QThread>

class OscListener : public QThread
{

public:
	OscListener( int iPortNum);
	~OscListener(){}

protected:
	void run() override;

private:
	void runListener();

private:
	int m_iPortNum = 0;

};

#endif // OSCLISTENER_H
