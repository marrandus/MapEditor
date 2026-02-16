#include "CMainWindow.h"

#include <QApplication>
#include <QIcon>

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/icon.png"));

	CMainWindow w;
	w.show();

	return app.exec();
}
