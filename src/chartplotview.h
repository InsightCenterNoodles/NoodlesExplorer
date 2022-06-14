#ifndef CHARTPLOTVIEW_H
#define CHARTPLOTVIEW_H

#include <QChartView>

QT_BEGIN_NAMESPACE
class QGestureEvent;
class QGraphicsProxyWidget;
QT_END_NAMESPACE

class ChartPlotView : public QChartView {
    bool m_in_touch_event = false;
    bool m_selection_mode = false;

    bool m_selection_move = false;

    QRubberBand* m_rubber_band;
    QPoint       m_band_origin;

    QGraphicsProxyWidget* m_proxy = nullptr;

protected:
    bool viewportEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

public:
    ChartPlotView();
    ~ChartPlotView();

public slots:
    void set_selection_mode(bool);

    void save_selection();

signals:
    void selection_made(QRectF);
};


class PlotChart : public QChart {
    bool m_selection_mode = false;

    bool gesture_event(QGestureEvent* event);

protected:
    bool sceneEvent(QEvent* event);

public:
    explicit PlotChart(QGraphicsItem*  parent = nullptr,
                       Qt::WindowFlags wFlags = {});
    ~PlotChart();

public slots:
    void set_selection_mode(bool);
};

#endif // CHARTPLOTVIEW_H
