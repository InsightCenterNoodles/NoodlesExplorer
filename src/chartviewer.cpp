#include "chartviewer.h"

#include "chartplotview.h"
#include "delegates/extable.h"
#include "tabledata.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

#include <QColorDialog>
#include <QDataWidgetMapper>
#include <QDateTime>
#include <QDebug>
#include <QInputDialog>

#include <random>

using namespace QtCharts;

static const QStringList default_color_list = {
    "#8dd3c7", "#ffffb3", "#bebada", "#fb8072", "#80b1d3", "#fdb462",
    "#b3de69", "#fccde5", "#d9d9d9", "#bc80bd", "#ccebc5", "#ffed6f",
};

QColor random_color() {
    static auto gen = std::mt19937 { std::random_device {}() };
    static auto b =
        std::uniform_int_distribution(0, default_color_list.size() - 1);
    return default_color_list.value(b(gen));
}


ColorWell::ColorWell(QWidget* parent) : QPushButton(parent) {
    this->setFlat(true);
    setColor(Qt::black);

    connect(this, &QPushButton::clicked, [this]() {
        auto c = QColorDialog::getColor(color(), this, "Pick Color");

        if (c.isValid()) this->setColor(c);
    });
}

void ColorWell::setColor(QColor color) {
    if (m_color == color) return;

    static const QString style_sheet =
        "QPushButton { color: black; background-color: %1; border-style: "
        "solid; border-color: black; border-width: 1px; border-radius:5px; }";

    this->setStyleSheet(style_sheet.arg(color.name()));
    m_color = color;
    emit colorChanged(m_color);
}

// =============================================================================

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

    qDebug() << "Bound for" << name << mins.x << maxs.x << mins.y << maxs.y;


    if (type == 0) { // ick
        auto* ns = new QLineSeries;
        series   = ns;
        ns->setName(name);

        chart->addSeries(ns);

        ns->attachAxis(x_axis);
        ns->attachAxis(y_axis);
        ns->setColor(color);
        ns->replace(points);
    } else if (type == 1) {
        auto ns = new QScatterSeries;
        series  = ns;
        ns->setName(name);
        ns->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        ns->setMarkerSize(4);

        chart->addSeries(ns);

        ns->attachAxis(x_axis);
        ns->attachAxis(y_axis);
        ns->setColor(color);
        ns->replace(points);
    }

    chart_view.recompute_bounds();
}

// =============================================================================

static const QStringList series_table_header = { "Name",
                                                 "Type",
                                                 "X",
                                                 "Y",
                                                 "Color" };

SeriesTable::SeriesTable(ChartViewer* v) : m_host(v) { }

QVariant SeriesTable::headerData(int             section,
                                 Qt::Orientation orientation,
                                 int             role) const {
    if (role != Qt::DisplayRole) return {};

    if (orientation != Qt::Orientation::Horizontal) return {};

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
    // qDebug() << Q_FUNC_INFO << index << role;
    if (!index.isValid()) return {};
    if (index.row() >= m_active_series.size()) return {};

    if (role != Qt::DisplayRole and role != Qt::EditRole) return {};


    auto& item = m_active_series[index.row()];

    switch (index.column()) {
    case 0: return item.name;
    case 1: return item.type;
    case 2: return item.a_col;
    case 3: return item.b_col;
    case 4: return item.color;
    }

    return {};
}

bool SeriesTable::setData(QModelIndex const& index,
                          QVariant const&    value,
                          int                role) {
    // qDebug() << Q_FUNC_INFO << index << value;
    if (data(index, role) == value) return false;

    auto& item = m_active_series[index.row()];

    switch (index.column()) {
    case 0: item.name = value.toString(); break;
    case 1: item.type = value.toInt(); break;
    case 2: item.a_col = value.toInt(); break;
    case 3: item.b_col = value.toInt(); break;
    case 4: item.color = value.value<QColor>(); break;
    }

    item.rebuild(*m_host);

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags SeriesTable::flags(QModelIndex const& index) const {
    //    qDebug() << Q_FUNC_INFO << index;
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
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

    // add some margin?

    qDebug() << "Totals bounds" << mins.x << maxs.x << mins.y << maxs.y;

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

void SeriesTable::add_new() {
    beginInsertRows({}, rowCount(), rowCount());
    auto& n = m_active_series.emplace_back();
    n.color = random_color();
    endInsertRows();
}
void SeriesTable::del_at(int i) {
    if (i < 0 or i >= m_active_series.size()) return;

    beginRemoveRows({}, i, i);
    m_active_series[i].series->deleteLater();
    m_active_series.erase(m_active_series.begin() + i);
    endRemoveRows();

    recompute_bounds();
}


// =============================================================================


void ChartViewer::setup_root() {

    m_widget = new QWidget;
    m_widget->setAttribute(Qt::WA_DeleteOnClose);

    m_ui_root = std::make_unique<Ui::ChartRoot>();
    m_ui_root->setupUi(m_widget);

    // set this up after the setupUI call, as it would be overwritten
    m_widget->setWindowTitle(m_attached_table->get_name());

    connect(m_widget.data(), &QWidget::destroyed, [this](QObject*) {
        this->deleteLater();
    });
}

void ChartViewer::setup_edit_page() {

    auto* color_b_layout = new QVBoxLayout();
    color_b_layout->setMargin(0);

    auto* color_well = new ColorWell();

    color_b_layout->addWidget(color_well);

    m_ui_root->colorWidget->setLayout(color_b_layout);

    m_ui_root->seriesListView->setModel(&m_series_table);
    m_ui_root->seriesListView->setModelColumn(0);

    auto* data_mapper = new QDataWidgetMapper(this);

    data_mapper->setModel(&m_series_table);

    connect(m_ui_root->seriesListView->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            data_mapper,
            &QDataWidgetMapper::setCurrentModelIndex);

    m_ui_root->typeComboBox->addItems(series_types);

    // Bug fix for OS X platforms and focus
    m_ui_root->typeComboBox->setFocusPolicy(Qt::StrongFocus);
    m_ui_root->xColumn->setFocusPolicy(Qt::StrongFocus);
    m_ui_root->yColumn->setFocusPolicy(Qt::StrongFocus);

    data_mapper->addMapping(m_ui_root->nameLineEdit, 0);
    data_mapper->addMapping(m_ui_root->typeComboBox, 1, "currentIndex");
    data_mapper->addMapping(m_ui_root->xColumn, 2, "currentIndex");
    data_mapper->addMapping(m_ui_root->yColumn, 3, "currentIndex");
    data_mapper->addMapping(color_well, 4, "color");

    on_mapper_changed(-1);

    connect(data_mapper,
            &QDataWidgetMapper::currentIndexChanged,
            this,
            &ChartViewer::on_mapper_changed);


    connect(color_well, &ColorWell::colorChanged, [data_mapper](auto) {
        data_mapper->submit();
    });


    connect(m_ui_root->addSeries, &QToolButton::clicked, [this]() {
        m_series_table.add_new();

        m_ui_root->seriesListView->selectionModel()->setCurrentIndex(
            m_series_table.index(m_series_table.rowCount() - 1, 0),
            QItemSelectionModel::Select);
    });

    connect(m_ui_root->delSeries, &QToolButton::clicked, [this]() {
        auto* sel_m = m_ui_root->seriesListView->selectionModel();
        auto  index = sel_m->currentIndex();
        if (!index.isValid()) return;
        m_series_table.del_at(index.row());
    });
}

void ChartViewer::setup_chart_page() {
    // set up charts

    m_chart_view = new ChartPlotView();

    m_ui_root->chartHolder->layout()->addWidget(m_chart_view);

    m_chart = new PlotChart();
    m_chart->setTheme(QChart::ChartThemeDark);
    m_chart->setBackgroundBrush(QColor(63, 63, 63));
    m_chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
    m_chart->setContentsMargins(0, 0, 0, 0);
    m_chart->setMargins(QMargins(1, 1, 1, 1));
    m_chart->setBackgroundRoundness(0);
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

    m_chart_view->setChart(m_chart);
    m_chart_view->setRenderHint(QPainter::Antialiasing);

    connect(this,
            &ChartViewer::selection_mode_changed,
            m_chart,
            &PlotChart::set_selection_mode);

    connect(this,
            &ChartViewer::selection_mode_changed,
            m_chart_view,
            &ChartPlotView::set_selection_mode);
}

ChartViewer::ChartViewer(QPointer<ExTable> t, QObject* parent)
    : QObject(parent),
      m_attached_table(t),
      m_data(t->table_data()),
      m_series_table(this) {

    setup_root();
    setup_edit_page();
    setup_chart_page();


    connect(t,
            &ExTable::fetch_new_remote_table_data,
            this,
            &ChartViewer::on_table_changed);

    if (!m_attached_table->is_subscribed()) {
        m_attached_table->subscribe();
    } else {
        on_table_changed();
    }

    m_widget->setVisible(true);
}

void ChartViewer::on_table_changed() {
    qDebug() << Q_FUNC_INFO << m_series_table.rowCount();

    m_data = m_attached_table->table_data();

    m_ui_root->xColumn->clear();
    m_ui_root->yColumn->clear();

    m_ui_root->xColumn->addItems(m_data->column_names());
    m_ui_root->yColumn->addItems(m_data->column_names());

    m_series_table.clear();

    for (auto c : m_table_obj_links) {
        disconnect(c);
    }

    // reconnect table changed to re-plot functions
    m_table_obj_links.clear();

    auto add_link = [&](auto&& p) {
        m_table_obj_links.push_back(connect(
            m_data.get(), p, this, &ChartViewer::on_table_data_changed));
    };


    add_link(&RemoteTableData::rowsInserted);
    add_link(&RemoteTableData::rowsMoved);
    add_link(&RemoteTableData::rowsRemoved);
    add_link(&RemoteTableData::modelReset);
    add_link(&RemoteTableData::dataChanged);

    m_series_table.rebuild_all();

    // selections

    if (m_data->selections()) {
        m_ui_root->selectionsListView->setModel(m_data->selections());
    }
}

void ChartViewer::on_table_data_changed() {
    m_series_table.rebuild_all();
}

void ChartViewer::on_mapper_changed(int i) {
    bool e = i >= 0;

    m_ui_root->nameLineEdit->setEnabled(e);
    m_ui_root->typeComboBox->setEnabled(e);
    m_ui_root->xColumn->setEnabled(e);
    m_ui_root->yColumn->setEnabled(e);
}

void ChartViewer::on_edit_selection_clicked(bool down) {
    if (down) {
        // enter selection mode
        auto rows =
            m_ui_root->selectionsListView->selectionModel()->selectedRows();

        if (rows.empty()) {
            m_ui_root->editSelection->setChecked(false);
            return;
        }

        auto row = rows.value(0);

        auto name = m_attached_table->table_data()
                        ->selections()
                        ->slot_at(row.row())
                        ->name;

        m_editing_selection = name;

        emit selection_mode_changed(true);

        return;
    }


    emit selection_mode_changed(false);
}

void ChartViewer::add_selection() {
    // auto populate with the first row
    auto key = m_attached_table->table_data()->key_for_row(0);

    if (key < 0) return;

    noo::Selection sel;

    sel.rows.push_back(key);


    auto name = QInputDialog::getText(m_widget,
                                      "New selection name",
                                      "New selection name:",
                                      QLineEdit::Normal);

    if (name.isEmpty()) { name = QDateTime::currentDateTime().toString(); }

    auto local_str = name.toStdString();

    m_attached_table->request_selection_update(local_str, std::move(sel));
}

void ChartViewer::del_selection() {
    auto rows = m_ui_root->selectionsListView->selectionModel()->selectedRows();

    if (rows.empty()) return;

    auto row = rows.value(0);

    auto name =
        m_attached_table->table_data()->selections()->slot_at(row.row())->name;

    m_attached_table->request_selection_update(name, {});
}
