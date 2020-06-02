#include "MapWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QGridLayout>
#include <QToolButton>
#include <QDebug>


MapWidget::MapWidget(QWidget* parent)
	: QWidget(parent)
	, _tileManager(NULL)
	, _statusWidget(NULL)
	, _zoomWidget(NULL)
	, _centerMapPoint()
	, _mouseMapPoint()
{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);
	setMouseTracking(true);

	QGridLayout* gridLayout = new QGridLayout(this);
	gridLayout->setContentsMargins(3, 3, 3, 3);
	this->setLayout(gridLayout);

	QToolButton* zoomOutToolButton = new QToolButton(this);
    //zoomOutToolButton->setArrowType(Qt::UpArrow);
    zoomOutToolButton->setText("-");
	zoomOutToolButton->setAutoRaise(true);
	connect(zoomOutToolButton, SIGNAL(clicked()), this, SLOT(on_zoomOutPushButton_clicked()));
	gridLayout->addWidget(zoomOutToolButton, 0, 0);
	zoomOutToolButton->setMinimumSize(64, 64);

	_zoomWidget = new QLabel(this);
	_zoomWidget->setAlignment(Qt::AlignHCenter);
	gridLayout->addWidget(_zoomWidget, 1, 0);

	QToolButton* zoomInToolButton = new QToolButton(this);
    //zoomInToolButton->setArrowType(Qt::DownArrow);
    zoomInToolButton->setText("+");
	zoomInToolButton->setAutoRaise(true);
	connect(zoomInToolButton, SIGNAL(clicked()), this, SLOT(on_zoomInPushButton_clicked()));
	gridLayout->addWidget(zoomInToolButton, 2, 0);
	zoomInToolButton->setMinimumSize(64, 64);

	gridLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding), 3,0);
	gridLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum), 0,1);

	_statusWidget = new QLabel(this);
	gridLayout->addWidget(_statusWidget, 4, 0, 2, 1);
}

MapWidget::~MapWidget()
{
	if (_tileManager != NULL)
		delete _tileManager;
}

void MapWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
}

void MapWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	_centerMapPoint.addPoint(event->pos() - QPoint(this->width() / 2, this->height() / 2));

	updateStatusWidget();
	update();

	QWidget::mouseDoubleClickEvent(event);
}

void MapWidget::mouseMoveEvent(QMouseEvent* event)
{
	static QPoint lastPos(0xffff, 0);

	if (event->buttons() & Qt::LeftButton)
	{
		if (lastPos.x() < 0xffff)
		{
			_centerMapPoint.addPoint(lastPos - event->pos());
			updateStatusWidget();
			update();
		}
		lastPos = event->pos();
	}
	else
	{
		lastPos.setX(0xffff);
	}

//	updateMPusePos(event->pos());

	QWidget::mouseMoveEvent(event);
}

void MapWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
}

void MapWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mouseReleaseEvent(event);
}

void MapWidget::wheelEvent(QWheelEvent* event)
{
	QPoint p = event->pos() - QPoint(this->width() / 2, this->height() / 2);
	_centerMapPoint.addPoint(p);
	_centerMapPoint.addZoom(event->delta() / abs(event->delta()));
	_centerMapPoint.addPoint(-p);

	updateMPusePos(event->pos());
	updateStatusWidget();
	update();
}

void MapWidget::on_zoomInPushButton_clicked()
{
	_centerMapPoint.addZoom(1);
	updateStatusWidget();
	update();
}

void MapWidget::on_zoomOutPushButton_clicked()
{
	_centerMapPoint.addZoom(-1);
	updateStatusWidget();
	update();
}

void MapWidget::paintEvent(QPaintEvent* event)
{
	if (_tileManager != NULL)
		_tileManager->beginReqTile();

	QPainter painter(this);

	painter.fillRect(this->rect(), Qt::gray);

	for (int y = 0; y < this->height(); y += 256)
	{
		for (int x = 0; x < this->width(); x += 256)
		{
			int mapX = x - (this->width() / 2) + _centerMapPoint.x();
			int mapY = y - (this->height() / 2) + _centerMapPoint.y();

			if ((mapX >= 0) && (mapY >= 0) && (mapX < MapPoint::PIX_PER_ZOOM[_centerMapPoint.zoom()]) && (mapY < MapPoint::PIX_PER_ZOOM[_centerMapPoint.zoom()]))
			{
				x = x - abs(mapX % 256);
				y = y - abs(mapY % 256);
				mapX = mapX - abs(mapX % 256);
				mapY = mapY - abs(mapY % 256);

				Tile tile(mapX / 256, mapY / 256, _centerMapPoint.zoom());

				if (_tileManager != NULL)
				{
					const TileImage* tileImage = _tileManager->reqTile(tile);
					if (tileImage != NULL)
					{
						painter.drawPixmap(x, y, tileImage->image());
					}
				}
				
/*
				const QRect tileRect(x, y, 256, 256);
				painter.setPen(Qt::black);
				painter.setBrush(QBrush(Qt::NoBrush));
				painter.drawRect(tileRect);
				{
					QString str = QString::number(tile.z()) + "-" + QString::number(tile.x()) + "-" + QString::number(tile.y());
					painter.drawText(tileRect, Qt::AlignCenter, str);
				}
*/
			}
		}
	}

	if (_tileManager != NULL)
		_tileManager->endReqTile();

//    {
//        QSize c = this->size() / 2;
//        painter.drawLine(c.width() - 3, c.height(), c.width() + 3, c.height());
//        painter.drawLine(c.width(), c.height() - 3, c.width(), c.height() + 3);
//    }

	QWidget::paintEvent(event);
}

void MapWidget::updateStatusWidget()
{
	QString str;

	str += tr(" z:%1 ").arg(_centerMapPoint.zoom());
	str += tr(" lat:%1 ").arg(MATH_RADTODEG(_centerMapPoint.lat()));
	str += tr(" lon:%1 ").arg(MATH_RADTODEG(_centerMapPoint.lon()));
	str += tr(" x:%1 ").arg(_centerMapPoint.x());
	str += tr(" y:%1 ").arg(_centerMapPoint.y());

/*
	str += "\t";

	str += tr(" z:%1 ").arg(_mouseMapPoint.zoom());
	str += tr(" lat:%1 ").arg(MATH_RADTODEG(_mouseMapPoint.lat()));
	str += tr(" lon:%1 ").arg(MATH_RADTODEG(_mouseMapPoint.lon()));
	str += tr(" x:%1 ").arg(_mouseMapPoint.x());
	str += tr(" y:%1 ").arg(_mouseMapPoint.y());
*/

	_statusWidget->setText(str);
}

void MapWidget::updateMPusePos(const QPoint& pos)
{
	return;
	_mouseMapPoint.setZoom(_centerMapPoint.zoom());
	_mouseMapPoint.setX(_centerMapPoint.x() + pos.x() - width() / 2);
	_mouseMapPoint.setY(_centerMapPoint.y() + pos.y() - height() / 2);
	updateStatusWidget();
}

void MapWidget::setTileManager(TileManager* tileManager)
{
	if (_tileManager != NULL)
	{
		delete _tileManager;
	}

	_tileManager = tileManager;

	if (_tileManager != NULL)
	{
		connect(_tileManager, SIGNAL(tileImageChanged()), this, SLOT(tileImageChanged()));
	}
}

void MapWidget::tileImageChanged()
{
	update();
}
