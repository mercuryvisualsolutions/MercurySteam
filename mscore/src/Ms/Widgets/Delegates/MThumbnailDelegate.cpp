#include "MThumbnailDelegate.h"

#include <Wt/WModelIndex>
#include <Wt/WContainerWidget>
#include <Wt/WAbstractItemModel>
#include <Wt/WImage>
#include <Wt/WVBoxLayout>

Ms::Widgets::Delegates::MThumbnailDelegate::MThumbnailDelegate() :
    MItemDelegate()
{
}

Ms::Widgets::Delegates::MThumbnailDelegate::MThumbnailDelegate(unsigned int width, unsigned int height, const std::string &defaultImagePath) :
    MThumbnailDelegate()
{
    _width = width;
    _height = height;
    _defaultImagePath = defaultImagePath;
}

unsigned int Ms::Widgets::Delegates::MThumbnailDelegate::width()
{
    return _width;
}

void Ms::Widgets::Delegates::MThumbnailDelegate::setWidth(unsigned int width)
{
    width = width;
}

unsigned int Ms::Widgets::Delegates::MThumbnailDelegate::height()
{
    return _height;
}

void Ms::Widgets::Delegates::MThumbnailDelegate::setHeight(unsigned int height)
{
    _height = height;
}

const std::string Ms::Widgets::Delegates::MThumbnailDelegate::defaultImagePath()
{
    return _defaultImagePath;
}

void Ms::Widgets::Delegates::MThumbnailDelegate::setDefaultImagePath(const std::string &defaultImagePath)
{
    _defaultImagePath = defaultImagePath;
}

Wt::WWidget *Ms::Widgets::Delegates::MThumbnailDelegate::update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag>)
{
    Wt::WContainerWidget *result;
    Wt::WImage *thumbnail;
    if(!widget)
    {
        result = new Wt::WContainerWidget();
        result->setLayout(new Wt::WVBoxLayout());
        thumbnail = new Wt::WImage();
        thumbnail->setAlternateText(_defaultImagePath);
        result->layout()->addWidget(thumbnail);
    }
    else
    {
        result = dynamic_cast<Wt::WContainerWidget*>(widget);
        thumbnail = dynamic_cast<Wt::WImage*>(result->widget(0));
    }

    result->setLineHeight(25);

    boost::any data = index.data(Wt::EditRole);
    if(!data.empty())
    {
        thumbnail->setImageLink(Wt::WLink(boost::any_cast<std::string>(data)));
        thumbnail->resize(_width, _height);
    }

    return result;
}
