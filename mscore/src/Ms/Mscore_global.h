#ifndef MSCORE_GLOBAL_H
#define MSCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MSCORE_LIBRARY)
#  define MSCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MSCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MSCORE_GLOBAL_H
