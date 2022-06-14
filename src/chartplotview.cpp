#include "chartplotview.h"

#include <QDebug>
#include <QGestureEvent>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QRubberBand>
#include <QtGui/QMouseEvent>

// Following the examples of ZoomlineCharts in the Qt Examples

ChartPlotView::ChartPlotView() {
    // setRubberBand(QChartView::RectangleRubberBand);

    m_rubber_band = new QRubberBand(QRubberBand::Rectangle, this);
}

ChartPlotView::~ChartPlotView() = default;

bool ChartPlotView::viewportEvent(QEvent* event) {
    if (event->type() == QEvent::TouchBegin and !m_selection_mode) {
        qDebug() << "TOUCH EVENT";
        // block mouse events from touch
        m_in_touch_event = true;

        // disable animations with touch
        chart()->setAnimationOptions(QChart::NoAnimation);
    }

    return QChartView::viewportEvent(event);
}

void ChartPlotView::mousePressEvent(QMouseEvent* event) {
    if (m_proxy) {
        bool b = this->scene()->sendEvent(m_proxy, event);
        qDebug() << Q_FUNC_INFO << b;
        // return QChartView::mousePressEvent(event);
        return;
    }

    qDebug() << Q_FUNC_INFO;
    if (m_selection_mode) {
        m_band_origin = event->pos();
        m_rubber_band->setGeometry(QRect(m_band_origin, QSize()));
        m_rubber_band->show();
        QChartView::mousePressEvent(event);
        m_selection_move = true;
        return;
    }

    if (m_in_touch_event) return;
    QChartView::mousePressEvent(event);
}
void ChartPlotView::mouseMoveEvent(QMouseEvent* event) {
    if (m_selection_mode and m_rubber_band->isVisible() and m_selection_move) {
        m_rubber_band->setGeometry(
            QRect(m_band_origin, event->pos()).normalized());

        return;
    }

    if (m_in_touch_event) return;
    QChartView::mouseMoveEvent(event);
}
void ChartPlotView::mouseReleaseEvent(QMouseEvent* event) {
    if (m_proxy) {
        bool b = this->scene()->sendEvent(m_proxy, event);
        qDebug() << Q_FUNC_INFO << b;
        if (!b) return QChartView::mouseReleaseEvent(event);
    }

    qDebug() << Q_FUNC_INFO;
    if (m_selection_mode and m_rubber_band->isVisible()) {
        qDebug() << "SELECT";
        m_selection_move = false;

        if (m_proxy) delete m_proxy;

        auto save_button = new QPushButton("Save");

        connect(save_button,
                &QPushButton::clicked,
                this,
                &ChartPlotView::save_selection);

        m_proxy = this->scene()->addWidget(save_button);

        QPointF p(m_proxy->size().width() / 2, m_proxy->size().height() / 2);

        m_proxy->setPos(event->pos() - p);
        m_proxy->setZValue(100);
    }

    if (m_in_touch_event) m_in_touch_event = false;

    // Restore animations
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    if (rubberBand()) QChartView::mouseReleaseEvent(event);
}
void ChartPlotView::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Plus: chart()->zoomIn(); break;
    case Qt::Key_Minus: chart()->zoomOut(); break;
    case Qt::Key_Left: chart()->scroll(-10, 0); break;
    case Qt::Key_Right: chart()->scroll(10, 0); break;
    case Qt::Key_Up: chart()->scroll(0, 10); break;
    case Qt::Key_Down: chart()->scroll(0, -10); break;
    case Qt::Key_R: chart()->zoomReset(); break;
    default: QGraphicsView::keyPressEvent(event); break;
    }
}

void ChartPlotView::set_selection_mode(bool state) {
    m_selection_mode = state;
    m_rubber_band->hide();
}

void ChartPlotView::save_selection() {
    qDebug() << Q_FUNC_INFO;

    m_rubber_band->hide();

    delete m_proxy;
}


// =============================================================================

bool PlotChart::gesture_event(QGestureEvent* event) {
    if (QGesture* gesture = event->gesture(Qt::PanGesture)) {
        QPanGesture* pan = static_cast<QPanGesture*>(gesture);
        QChart::scroll(-(pan->delta().x()), pan->delta().y());
    }

    if (QGesture* gesture = event->gesture(Qt::PinchGesture)) {
        QPinchGesture* pinch = static_cast<QPinchGesture*>(gesture);
        if (pinch->changeFlags() & QPinchGesture::ScaleFactorChanged) {
            QChart::zoom(pinch->scaleFactor());
        }
    }

    return true;
}

bool PlotChart::sceneEvent(QEvent* event) {
    if (event->type() == QEvent::Gesture) {
        qDebug() << "GESTURE" << event->type();
        return gesture_event(static_cast<QGestureEvent*>(event));
    }
    return QChart::event(event);
}

PlotChart::PlotChart(QGraphicsItem* parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags) {
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    setAcceptHoverEvents(true);
}

PlotChart::~PlotChart() = default;

void PlotChart::set_selection_mode(bool state) {
    m_selection_mode = state;

    if (state) {
        ungrabGesture(Qt::PanGesture);
        ungrabGesture(Qt::PinchGesture);
    } else {
        grabGesture(Qt::PanGesture);
        grabGesture(Qt::PinchGesture);
    }
}
