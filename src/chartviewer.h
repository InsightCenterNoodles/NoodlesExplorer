#ifndef CHARTVIEWER_H
#define CHARTVIEWER_H

#include "ui_chartviewer.h"

#include "noo_include_glm.h"

#include <QAbstractTableModel>
#include <QPointer>
#include <QPushButton>
#include <QWidget>

class ExTable;
class RemoteTableData;
class ChartViewer;

class QAbstractSeries;
class QChart;
class QValueAxis;


class ColorWell : public QPushButton {
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged);

    QColor m_color;

public:
    ColorWell(QWidget* parent = nullptr);

    QColor color() const { return m_color; }
public slots:
    void setColor(QColor color);
signals:
    void colorChanged(QColor color);
};

struct ChartSeriesPart {
    QString name  = "Unnamed";
    int     type  = -1;
    int     a_col = -1;
    int     b_col = -1;
    QColor  color = Qt::black;

    QAbstractSeries* series;
    glm::vec2        mins;
    glm::vec2        maxs;
    void             rebuild(ChartViewer& chart_view);
};


class SeriesTable : public QAbstractTableModel {
    ChartViewer*           m_host;
    QList<ChartSeriesPart> m_active_series;

public:
    explicit SeriesTable(ChartViewer*);

public:
    QVariant headerData(int             section,
                        Qt::Orientation orientation,
                        int             role = Qt::DisplayRole) const override;

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;

    QVariant data(QModelIndex const& index,
                  int                role = Qt::DisplayRole) const override;

    bool setData(QModelIndex const& index,
                 QVariant const&    value,
                 int                role = Qt::EditRole) override;

    Qt::ItemFlags flags(QModelIndex const& index) const override;

public:
    void clear();
    void rebuild_all();
    void recompute_bounds();

    void add_new();
    void del_at(int);
};

class PlotChart;
class ChartPlotView;

class ChartViewer : public QObject {
    Q_OBJECT

    QPointer<QWidget>              m_widget;
    std::unique_ptr<Ui::ChartRoot> m_ui_root;

    PlotChart*     m_chart;
    ChartPlotView* m_chart_view;
    QValueAxis*    m_x_axis;
    QValueAxis*    m_y_axis;

    QPointer<ExTable>                m_attached_table;
    std::shared_ptr<RemoteTableData> m_data;

    std::vector<QMetaObject::Connection> m_table_obj_links;

    SeriesTable m_series_table;

    std::optional<std::string> m_editing_selection;

    void setup_root();
    void setup_edit_page();
    void setup_chart_page();

public:
    explicit ChartViewer(QPointer<ExTable>, QObject* parent = nullptr);

    PlotChart*       chart() const { return m_chart; }
    QValueAxis*      x_axis() const { return m_x_axis; }
    QValueAxis*      y_axis() const { return m_y_axis; }
    RemoteTableData* current_data() const { return m_data.get(); }

    void recompute_bounds() { m_series_table.recompute_bounds(); }

private slots:
    void on_table_changed();
    void on_table_data_changed();
    void on_mapper_changed(int);

    void on_edit_selection_clicked(bool);
    void add_selection();
    void del_selection();

signals:
    void selection_mode_changed(bool);
};

#endif // CHARTVIEWER_H
