#pragma once

#ifdef QT_GUI_LIB

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>


class QslScreenGeometry
{
	QslScreenGeometry();
	QslScreenGeometry(const QslScreenGeometry&);

public:
	static void init()
	{
		geometry(true);
	}

	static const QRect& geometry(bool isInit = false)
	{
		static QRect screenGeometry;

		if ((isInit == true) || (screenGeometry.isNull() == true))
		{
			QDesktopWidget* desctop = QApplication::desktop();
			if (desctop == NULL)
			{
				screenGeometry.setRect(50, 50, 100, 100);
				return screenGeometry;
			}

			screenGeometry = desctop->geometry();
		}

		return screenGeometry;
	}

	static QRect geometryD2()
	{
		return QRect(geometry().width() / 4, geometry().height() / 4, geometry().width() / 2, geometry().height() / 2);
	}

    static QRect geometryD3()
	{
		return QRect(geometry().width() / 3, geometry().height() / 3, geometry().width() / 3, geometry().height() / 3);
	}

    static QRect geometry4D6()
	{
		return QRect(geometry().width() / 6, geometry().height() / 6, 4 * geometry().width() / 6,  4 * geometry().height() / 6);
	}

    static QRect geometry8D10()
	{
		return QRect(geometry().width() / 10, geometry().height() / 10, 8 * geometry().width() / 10,  8 * geometry().height() / 10);
	}
};

#endif //QT_GUI_LIB