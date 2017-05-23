#ifndef QTRVINODE_GLOBAL_H
#define QTRVINODE_GLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_QTRVI_LIB)
#    define Q_QTRVI_EXPORT Q_DECL_EXPORT
#  else
#    define Q_QTRVI_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_QTRVI_EXPORT
#endif

QT_END_NAMESPACE

#endif // QTRVINODE_GLOBAL_H
