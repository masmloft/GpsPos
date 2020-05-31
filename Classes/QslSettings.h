#pragma once

#include <QCoreApplication>
#include <QSettings>
#include <QVariant>
#include <QRect>
#include <QDataStream>
#ifdef QT_GUI_LIB
#	include <QMainWindow>
#	include <QWidget>
#	include <QSpinBox>
#	include <QDoubleSpinBox>
#	include <QComboBox>
#	include <QPushButton>
#	include <QHeaderView>
#endif //QT_GUI_LIB


/************************************************************************/
/*                                                                      */
/************************************************************************/
class QslSettings
{
public:
	enum Storage
	{
		STORAGE_USER = 0,
		STORAGE_SYSTEM,
		STORAGE_FILE,
	};

public:
	virtual ~QslSettings()
	{
	}

	bool contains(const QString& key) const
	{
		if (_settings == NULL)
			return false;
		return _settings->contains(key);
	}

	void setValue(const QString& key, const QVariant& value)
	{
		if (_settings == NULL)
			return;
		_settings->setValue(key, value);
	}

	QVariant value(const QString& key, const QVariant& defaultValue) const
	{
		if (_settings == NULL)
			return QVariant();
		return _settings->value(key, defaultValue);
	}

	bool readValue(const QString& key, qint8& val)
	{
		bool ret = true;
		val = value(key, QVariant(val)).toInt(&ret);
		return ret;
	}

	bool readValue(const QString& key, quint8& val)
	{
		bool ret = true;
		val = value(key, QVariant(val)).toUInt(&ret);
		return ret;
	}

	bool readValue(const QString& key, qint16& val)
	{
		bool ret = true;
		val = value(key, QVariant(val)).toInt(&ret);
		return ret;
	}

	bool readValue(const QString& key, quint16& val)
	{
		bool ret = true;
		val = value(key, QVariant(val)).toUInt(&ret);
		return ret;
	}

	bool readValue(const QString& key, int& val)
	{
		bool ret = true;
		val = value(key, QVariant(val)).toInt(&ret);
		return ret;
	}

	bool readValue(const QString& key, uint& val)
	{
		bool ret = true;
		val = value(key, QVariant(val)).toUInt(&ret);
		return ret;
	}

	bool readValue(const QString& key, QString& val)
	{
		bool ret = true;
		val = value(key, QVariant(val)).toString();
		return ret;
	}

	bool readValue(const QString& key, QByteArray& val)
	{
		bool ret = true;
		val = value(key, QVariant(val)).toByteArray();
		return ret;
	}

#ifdef QT_GUI_LIB
	void restoreWidgetGeometry(QWidget* widget)
	{
#ifdef _DEBUG
		Q_ASSERT(widget->objectName().isEmpty() == false);
#endif
		if(this->contains(_section + widget->objectName() + "/Geometry") == true)
			widget->setGeometry(this->value(_section + widget->objectName() + "/Geometry", widget->geometry()).toRect());
	}

	void restoreWidgetGeometry(QWidget* widget, const QRect& def)
	{
#ifdef _DEBUG
		Q_ASSERT(widget->objectName().isEmpty() == false);
#endif
		if(this->contains(_section + widget->objectName() + "/Geometry") == true)
			widget->setGeometry(this->value(_section + widget->objectName() + "/Geometry", def).toRect());
		else
			widget->setGeometry(def);
	}

	void saveWidgetGeometry(const QWidget* widget)
	{
#ifdef _DEBUG
		Q_ASSERT(widget->objectName().isEmpty() == false);
#endif
		this->setValue(_section + widget->objectName() + "/Geometry", widget->geometry());
	}

	void restoreWidgetState(QMainWindow* widget)
	{
#ifdef _DEBUG
		Q_ASSERT(widget->objectName().isEmpty() == false);
#endif
		widget->restoreState(this->value(_section + widget->objectName() + "/State", widget->saveState()).toByteArray());
	}

	void saveWidgetState(const QMainWindow* widget)
	{
#ifdef _DEBUG
		Q_ASSERT(widget->objectName().isEmpty() == false);
#endif
		this->setValue(_section + widget->objectName() + "/State", widget->saveState());
	}

	void restoreHeaderViewState(const QString& key, QHeaderView* headerView)
	{
		QByteArray data = this->value(_section + key, QByteArray()).toByteArray();
		QDataStream stream(&data, QIODevice::ReadOnly);

		{
			int marker;
			stream >> marker;
			if (marker != 0x1a)
			{
				return;
			}
		}

		{
			int sortIndicatorOrder;
			stream >> sortIndicatorOrder;
			int sortIndicatorSection;
			stream >> sortIndicatorSection;

			headerView->setSortIndicator(sortIndicatorSection, Qt::SortOrder(sortIndicatorOrder));
		}

		for (int i = 0; i < headerView->count(); ++i)
		{
			int sectionSize;
			stream >> sectionSize;

			if (sectionSize < headerView->minimumSectionSize())
				sectionSize = headerView->minimumSectionSize();
			if (sectionSize > 1024)
				sectionSize = 1024;

			headerView->resizeSection(i, sectionSize);
		}
	}

	void saveHeaderViewState(const QString& key, const QHeaderView* headerView)
	{
		QByteArray data;
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream << int(0x1a);

		stream << int(headerView->sortIndicatorOrder());
		stream << headerView->sortIndicatorSection();

		for (int i = 0; i < headerView->count(); ++i)
		{
			stream << headerView->sectionSize(i);
		}

		this->setValue(_section + key, data);
	}

	void saveState(QComboBox* object)
	{
#ifdef _DEBUG
		Q_ASSERT(object->objectName().isEmpty() == false);
#endif
		this->setValue(_section + object->objectName(), object->currentIndex());
	}

	void restoreState(QComboBox* object)
	{
#ifdef _DEBUG
		Q_ASSERT(object->objectName().isEmpty() == false);
#endif
		object->setCurrentIndex(this->value(_section + object->objectName(), object->currentIndex()).toInt());
	}

	void saveState(QSpinBox* object)
	{
#ifdef _DEBUG
		Q_ASSERT(object->objectName().isEmpty() == false);
#endif
		this->setValue(_section + object->objectName(), object->value());
	}

	void restoreState(QSpinBox* object)
	{
#ifdef _DEBUG
		Q_ASSERT(object->objectName().isEmpty() == false);
#endif
		object->setValue(this->value(_section + object->objectName(), object->value()).toInt());
	}

	void saveState(QDoubleSpinBox* object)
	{
#ifdef _DEBUG
		Q_ASSERT(object->objectName().isEmpty() == false);
#endif
		this->setValue(_section + object->objectName(), object->value());
	}

	void restoreState(QDoubleSpinBox* object)
	{
#ifdef _DEBUG
		Q_ASSERT(object->objectName().isEmpty() == false);
#endif
		object->setValue(this->value(_section + object->objectName(), object->value()).toDouble());
	}

	void saveState(QPushButton* object)
	{
#ifdef _DEBUG
		Q_ASSERT(object->objectName().isEmpty() == false);
#endif
		this->setValue(_section + object->objectName(), object->isChecked());
	}

	void restoreState(QPushButton* object)
	{
#ifdef _DEBUG
		Q_ASSERT(object->objectName().isEmpty() == false);
#endif
		object->setChecked(this->value(_section + object->objectName(), object->isChecked()).toBool());
	}

#endif //QT_GUI_LIB

protected:
	QslSettings(QSettings*& settings, const QString& section) : _settings(settings), _section(section) 
	{
		if (_section.isEmpty() == false)
		{
			if (_section.at(_section.size() - 1) != QChar('/'))
				_section.append(QChar('/'));
		}
	}

	QslSettings(const QslSettings& other, const QString& section) : _settings(other._settings), _section(section) 
	{
		if (_section.isEmpty() == false)
		{
			if (_section.at(_section.size() - 1) != QChar('/'))
				_section.append(QChar('/'));
		}
	}

private:
	QSettings*& _settings;
	QString _section;

};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class QslSystemSettings : public QslSettings
{
public:
	QslSystemSettings(const QslSystemSettings& other)
		: QslSettings(other)
	{
		staticData().ref++;
	}

	QslSystemSettings(const QString& section = QString())
		: QslSettings(staticData().settings, section)
	{
		staticData().ref++;

		if (staticData().settings == NULL)
		{
			if(staticData().storage == QslSettings::STORAGE_SYSTEM)
			{
				staticData().settings = new QSettings(QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
			}
			else if(staticData().storage == QslSettings::STORAGE_USER)
			{
				staticData().settings = new QSettings(QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
			}
			else if(staticData().storage == QslSettings::STORAGE_FILE)
			{
				QString filename;
				if (staticData().fileName.isEmpty() == false)
				{
					filename += staticData().fileName;
				}
				else
				{
					filename += QCoreApplication::applicationDirPath();
					filename += "/" + QCoreApplication::applicationName();
					filename += ".conf";
				}

				staticData().settings = new QSettings(filename, QSettings::IniFormat);
			}
			else
			{
				staticData().settings = new QSettings(QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
			}
		}
	}

	~QslSystemSettings()
	{
		staticData().ref--;

		if (staticData().ref <= 0)
		{
			delete staticData().settings;
			staticData().settings = NULL;
		}
	}

	static void setStorage(const Storage& storage) 
	{ 
		staticData().storage = storage; 
	}

	static void setFileName(const QString& fileName) 
	{ 
		staticData().fileName = fileName; 
		staticData().storage = STORAGE_FILE; 
	}

private:
	struct StaticData
	{
		int ref;
		QSettings* settings;
		Storage storage;
		QString fileName;

		StaticData()
			: ref(0)
			, settings(NULL)
			, storage(QslSettings::STORAGE_SYSTEM)
			, fileName()
		{
		}
	};

private:
	static StaticData& staticData()
	{
		static StaticData d;
		return d;
	}
};

inline QslSystemSettings qslSystemSettings() 
{ 
	return QslSystemSettings(); 
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
class QslUserSettings : public QslSettings
{
public:
	QslUserSettings(const QslUserSettings& other)
		: QslSettings(other)
	{
		staticData().ref++;
	}

	QslUserSettings(const QString& section = QString())
		: QslSettings(staticData().settings, section)
	{
		staticData().ref++;

		if (staticData().settings == NULL)
		{
			if(staticData().storage == QslSettings::STORAGE_SYSTEM)
			{
				staticData().settings = new QSettings(QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
			}
			else if(staticData().storage == QslSettings::STORAGE_USER)
			{
				staticData().settings = new QSettings(QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
			}
			else if(staticData().storage == QslSettings::STORAGE_FILE)
			{
				QString filename;
				if (staticData().fileName.isEmpty() == false)
				{
					filename += staticData().fileName;
				}
				else
				{
					filename += QCoreApplication::applicationDirPath();
					filename += "/" + QCoreApplication::applicationName();
					filename += ".conf";
				}

				staticData().settings = new QSettings(filename, QSettings::IniFormat);
			}
			else
			{
				staticData().settings = new QSettings(QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
			}
		}
	}

	~QslUserSettings()
	{
		staticData().ref--;

		if (staticData().ref <= 0)
		{
			delete staticData().settings;
			staticData().settings = NULL;
		}
	}

	static void setStorage(const Storage& storage) 
	{ 
		staticData().storage = storage; 
	}

	static void setFileName(const QString& fileName) 
	{ 
		staticData().fileName = fileName; 
		staticData().storage = STORAGE_FILE; 
	}

private:
	struct StaticData
	{
		int ref;
		QSettings* settings;
		Storage storage;
		QString fileName;

		StaticData()
			: ref(0)
			, settings(NULL)
			, storage(QslSettings::STORAGE_USER)
			, fileName()
		{
		}
	};

private:
	static StaticData& staticData()
	{
		static StaticData d;
		return d;
	}

};

inline QslUserSettings qslUserSettings() 
{ 
	return QslUserSettings(); 
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
class QslFileSettings : public QslSettings
{
public:
	QslFileSettings(const QString& fileName, const QString& section = QString())
		: QslSettings(_settings, section)
		, _settings(NULL)
		, _fileName(fileName)
	{
		open();
	}

	~QslFileSettings()
	{
		close();
	}

	bool isOpen() const 
	{ 
		return (_settings != NULL); 
	}

	bool open()
	{
		if (_settings != NULL)
			return true;
		_settings = new QSettings(_fileName, QSettings::IniFormat);
		return true;
	}

	void close()
	{
		if (_settings == NULL)
			return;
		delete _settings;
		_settings = NULL;
	}

private:
	QSettings* _settings;
	QString _fileName;
};
