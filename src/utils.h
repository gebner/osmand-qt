#pragma once

#include <OsmAndCore.h>
#include <QPoint>

namespace {

using namespace OsmAnd;

inline QPoint toQPoint(const PointI& pt) {
    return QPoint(pt.x, pt.y);
}

static inline PointI toPointI(const QPoint& pt) {
    return PointI(pt.x(), pt.y());
}

static inline PointI toPointI(const QSize& size) {
    return PointI(size.width(), size.height());
}

}
