#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(TREEVIEWMODEL_LIB)
#  define TREEVIEWMODEL_EXPORT Q_DECL_EXPORT
# else
#  define TREEVIEWMODEL_EXPORT Q_DECL_IMPORT
# endif
#else
# define TREEVIEWMODEL_EXPORT
#endif
