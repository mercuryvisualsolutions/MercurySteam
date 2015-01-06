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
                unsigned int m_width;
                unsigned int m_height;
                std::string m_defaultImagePath;
            };
        }
    }
}

#endif // MTHUMBNAILDELEGATE_H
