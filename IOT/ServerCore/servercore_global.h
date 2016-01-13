#ifndef SERVERCORE_GLOBAL_H
#define SERVERCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef SERVERCORE_LIB
# define SERVERCORE_EXPORT Q_DECL_EXPORT
#else
# define SERVERCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // SERVERCORE_GLOBAL_H
