#include "chartviewer.h"

#include "delegates.h"
#include "tabledata.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

#include <QDataWidgetMapper>

using namespace QtCharts;

static const QStringList series_types = { "Line", "Scatter" };


void ChartSeriesPart::rebuild(ChartViewer& chart_view) {
    if (series) series->deleteLater();


    auto chart  = chart_view.chart();
    auto x_axis = chart_view.x_axis();
    auto y_axis = chart_view.y_axis();

    if (a_col < 0 or b_col < 0) return;

    auto& data_a = chart_view.current_data()->column(a_col);
    auto& data_b = chart_view.current_data()->column(b_col);

    if (data_a.is_string() or data_b.is_string()) return;

    auto span_a = data_a.as_doubles();
    auto span_b = data_b.as_doubles();

    auto common_size = std::min(span_a.size(), span_b.size());

    mins = glm::vec2(std::numeric_limits<float>::max());
    maxs = glm::vec2(std::numeric_limits<float>::lowest());

    QVector<QPointF> points;

    for (size_t i = 0; i < common_size; i++) {
        auto p = glm::vec2(span_a[i], span_b[i]);
        mins   = glm::min(mins, p);
        maxs   = glm::max(maxs, p);
        points << QPointF(span_a[i], span_b[i]);
    }


    if (type == series_types[0]) {
        auto* ns = new QLineSeries;
        series   = ns;
        ns->setName(name);

        chart->addSeries(ns);

        ns->attachAxis(x_axis);
        ns->attachAxis(y_axis);
        ns->setColor(color);
        ns->replace(points);
    } else if (type == series_types[1]) {
        auto ns = new QScatterSeries;
        series  = ns;
        ns->setName(name);
        ns->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        ns->setMarkerSize(2);

        chart->addSeries(ns);

        ns->attachAxis(x_axis);
        ns->attachAxis(y_axis);
        ns->setColor(color);
        ns->replace(points);
    }
}

// =============================================================================

static const QStringList series_table_header = { "Name",
                                                 "Type",
                                                 "X",
                                                 "Y",
                                                 "Color" };

QString SeriesTable::col_id_to_name(int i) const {
    auto* p = m_host->current_data();

    if (!p) return {};
    if (i < 0) return {};
    if (p->column_count() >= i) return {};

    auto l = p->column_names();

    return l[i];
}

SeriesTable::SeriesTable(ChartViewer* v) : m_host(v) { }

QVariant SeriesTable::headerData(int             section,
                                 Qt::Orientation orientation,
                                 int             role) const {
    if (role != Qt::DisplayRole) return {};

    if (orientation != Qt::Orientation::Horizontal) return {};
    // return m_active_series.at(section)->name;

    return series_table_header[section];
}


int SeriesTable::rowCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return m_active_series.size();
}

int SeriesTable::columnCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return series_table_header.size();
}


QVariant SeriesTable::data(QModelIndex const& index, int role) const {
    // qDebug() << index << m_columns.size();
    if (!index.isValid()) return {};
    if (index.row() >= m_active_series.size()) return {};

    if (role != Qt::DisplayRole and role != Qt::EditRole) return {};

    auto& item = m_active_series[index.row()];

    switch (index.column()) {
    case 0: return item.name; break;
    case 1: return item.type; break;
    case 2: return col_id_to_name(item.a_col); break;
    case 3: return col_id_to_name(item.b_col); break;
    case 4: return item.color; break;
    }

    return {};
}

bool SeriesTable::setData(QModelIndex const& index,
                          QVariant const&    value,
                          int                role) {
    if (data(index, role) == value) return false;

    auto& item = m_active_series[index.row()];

    switch (index.column()) {
    case 0: item.name = value.toString(); break;
    case 1: item.type = value.toString(); break;
    case 2: item.a_col = value.toInt(); break;
    case 3: item.b_col = value.toInt(); break;
    case 4: item.color = QColor(value.toString()); break;
    }

    item.rebuild(*m_host);

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

Qt::ItemFlags SeriesTable::flags(QModelIndex const& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

void SeriesTable::clear() {
    beginResetModel();

    for (auto& s : m_active_series) {
        s.series->deleteLater();
    }

    m_active_series.clear();

    endResetModel();
}

void SeriesTable::rebuild_all() {
    for (auto& s : m_active_series) {
        s.rebuild(*m_host);
    }

    recompute_bounds();
}

void SeriesTable::recompute_bounds() {
    glm::vec2 mins(std::numeric_limits<float>::max());
    glm::vec2 maxs(std::numeric_limits<float>::lowest());

    for (auto const& a : m_active_series) {
        mins = glm::min(mins, a.mins);
        maxs = glm::max(maxs, a.maxs);
    }

    auto chart = m_host->chart();

    {
        auto ax = chart->axes(Qt::Horizontal);
        for (auto a : ax) {
            a->setRange(mins.x, maxs.x);
        }
    }

    {
        auto ax = chart->axes(Qt::Vertical);
        for (auto a : ax) {
            a->setRange(mins.y, maxs.y);
        }
    }
}


// =============================================================================


ChartViewer::ChartViewer(std::shared_ptr<ExTable> t, QObject* parent)
    : QObject(parent),
      m_attached_table(t),
      m_data(t->table_data()),
      m_series_table(this) {

    connect(t.get(), &ExTable::fetch_new_remote_table_data, [this]() {
        m_data = m_attached_table->table_data();
        connect_table();
    });

    m_widget = new QWidget;
    m_widget->setObjectName(t->get_name());
    m_widget->setWindowTitle(t->get_name());
    m_widget->setAttribute(Qt::WA_DeleteOnClose);

    m_ui_root = std::make_unique<Ui::ChartRoot>();
    m_ui_root->setupUi(m_widget);

    m_widget->setWindowTitle(t->get_name());

    connect(m_widget.data(), &QWidget::destroyed, [this](QObject*) {
        this->deleteLater();
    });

    m_ui_root->stackedWidget->setCurrentIndex(0);


    connect(m_ui_root->editButton, &QToolButton::toggled, [this](bool checked) {
        m_ui_root->stackedWidget->setCurrentIndex(checked);
    });

    auto* data_mapper = new QDataWidgetMapper(this);

    data_mapper->setModel(&m_series_table);

    connect(m_ui_root->seriesListView->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            mapper,
            &QDataWidgetMapper::setCurrentModelIndex);

    data_mapper->set

        // set up charts

        auto cview = new QChartView();

    m_ui_root->chartHolder->layout()->addWidget(cview);

    m_chart = new QChart();
    m_chart->setTheme(QChart::ChartThemeDark);
    m_chart->setBackgroundBrush(QColor(63, 63, 63));
    m_chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
    m_chart->setContentsMargins(1, 1, 1, 1);
    m_chart->setMargins(QMargins(2, 2, 2, 2));
    m_chart->setAcceptHoverEvents(true);

    {
        m_x_axis = new QValueAxis();
        m_x_axis->setRange(0, 2);
        // m_x_axis->setTickCount(10);
        m_chart->addAxis(m_x_axis, Qt::AlignBottom);
    }

    {
        m_y_axis = new QValueAxis();
        m_y_axis->setRange(0, 2);
        // m_y_axis->setTickCount(10);
        m_chart->addAxis(m_y_axis, Qt::AlignLeft);
    }

    cview->setChart(m_chart);
    cview->setRenderHint(QPainter::Antialiasing);

    connect_table();

    on_table_changed();


    m_widget->setVisible(true);
}

void ChartViewer::connect_table() {
    m_series_table.clear();

    for (auto c : m_table_obj_links) {
        disconnect(c);
    }

    m_table_obj_links.clear();

    auto add_link = [&](auto&& p) {
        m_table_obj_links.push_back(
            connect(m_data.get(), p, [this]() { on_table_changed(); }));
    };


    add_link(&RemoteTableData::rowsInserted);
    add_link(&RemoteTableData::rowsMoved);
    add_link(&RemoteTableData::rowsRemoved);
    add_link(&RemoteTableData::modelReset);
    add_link(&RemoteTableData::dataChanged);
}

void ChartViewer::on_table_changed() {
    qDebug() << Q_FUNC_INFO << m_series_table.rowCount();

    m_series_table.rebuild_all();

    //    glm::vec2 mins(std::numeric_limits<float>::max());
    //    glm::vec2 maxs(std::numeric_limits<float>::lowest());

    //    for (auto const& s : m_active_series) {
    //        qDebug() << s.a_col << s.b_col;
    //        if (s.a_col < 0 or s.b_col < 0) continue;

    //        auto& data_a = m_data->column(s.a_col);
    //        auto& data_b = m_data->column(s.b_col);

    //        qDebug() << data_a.is_string() << data_b.is_string();
    //        if (data_a.is_string() or data_b.is_string()) continue;

    //        auto span_a = data_a.as_doubles();
    //        auto span_b = data_b.as_doubles();

    //        auto common_size = std::min(span_a.size(), span_b.size());

    //        QVector<QPointF> points;

    //        for (size_t i = 0; i < common_size; i++) {
    //            auto p = glm::vec2(span_a[i], span_b[i]);
    //            mins   = glm::min(mins, p);
    //            maxs   = glm::max(maxs, p);

    //            points << QPointF(span_a[i], span_b[i]);
    //        }

    //        s.series->replace(points);
    //    }

    //    qDebug() << "Bounds";
    //    qDebug() << mins.x << maxs.x;
    //    qDebug() << mins.y << maxs.y;

    //    {
    //        auto ax = m_chart->axes(Qt::Horizontal);
    //        for (auto a : ax) {
    //            a->setRange(mins.x, maxs.x);
    //        }
    //    }

    //    {
    //        auto ax = m_chart->axes(Qt::Vertical);
    //        for (auto a : ax) {
    //            a->setRange(mins.y, maxs.y);
    //        }
    //    }
}
