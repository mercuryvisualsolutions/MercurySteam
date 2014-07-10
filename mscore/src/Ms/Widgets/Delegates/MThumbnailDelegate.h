#ifndef MTHUMBNAILDELEGATE_H
#define MTHUMBNAILDELEGATE_H

#include "MItemDelegate.h"

#include <iostream>

namespace Ms
{
    namespace Widgets
    {
        namespace Delegates
        {
            class MThumbnailDelegate : public MItemDelegate
            {
            public:
                MThumbnailDelegate();
                MThumbnailDelegate(unsigned int width, unsigned int height, const std::string &defaultImagePath);

                unsigned int width();
                void setWidth(unsigned int width);
                unsigned int height();
                void setHeight(unsigned int height);
                const std::string defaultImagePath();
                void setDefaultImagePath(const std::string &defaultImagePath);

                //functions
                Wt::WWidget *update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag>) override;

            private:
                unsigned int _width;
                unsigned int _height;
                std::string _defaultImagePath;
            };
        }
    }
}

#endif // MTHUMBNAILDELEGATE_H
