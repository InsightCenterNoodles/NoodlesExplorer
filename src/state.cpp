#include "state.h"


#include "delegates/exbuffer.h"
#include "delegates/exlight.h"
#include "delegates/exmaterial.h"
#include "delegates/exmesh.h"
#include "delegates/exobject.h"
#include "delegates/exsignal.h"
#include "delegates/extable.h"
#include "delegates/extexture.h"

#include "chartviewer.h"
#include "methodcalldialog.h"
#include "noo_client_interface.h"
#include "noo_common.h"
#include "tableviewer.h"
#include "variant_tools.h"

#include <QHostInfo>
#include <QQmlContext>
#include <QTreeWidgetItem>
#include <QUrl>

#include <QQmlEngine>

static State* current_state = nullptr;

State::State(QObject* parent) : QObject(parent) {
    current_state = this;

    set_connection_state(-1);

    m_method_list      = new ComponentListModel<ExMethod>(this);
    m_signal_list      = new ComponentListModel<ExSignal>(this);
    m_table_list       = new ComponentListModel<ExTable>(this);
    m_buffer_list      = new ComponentListModel<ExBuffer>(this);
    m_buffer_view_list = new ComponentListModel<ExBufferView>(this);
    m_texture_list     = new ComponentListModel<ExTexture>(this);
    m_sampler_list     = new ComponentListModel<ExSampler>(this);
    m_image_list       = new ComponentListModel<ExImage>(this);
    m_material_list    = new ComponentListModel<ExMaterial>(this);
    m_light_list       = new ComponentListModel<ExLight>(this);
    m_mesh_list        = new ComponentListModel<ExMesh>(this);
    m_object_list      = new ComponentListModel<ExObject>(this);

    m_ent_notifier = new EntityChangeNotifier(this);
    m_mat_notifier = new MaterialChangeNotifier(this);

    m_object_filter = new TaggedNameObjectFilter(this);

    m_object_filter->setSourceModel(m_object_list);

    connect(&m_document_methods,
            &AttachedMethodListModel::wishes_to_call,
            [this](ExMethod* ptr) {
                if (m_current_doc) {

                    auto* dialog = new MethodCallDialog(m_current_doc, ptr);

                    dialog->setVisible(true);
                }
            });
}

State::~State() {
    qDebug() << "Destroying state";
}

void State::link(QQmlContext* c) {
    c->setContextProperty("app_state", this);

    c->setContextProperty("argument_model", &m_argument_table_model);

    c->setContextProperty("method_list", m_method_list);
    c->setContextProperty("signal_list", m_signal_list);
    c->setContextProperty("table_list", m_table_list);
    c->setContextProperty("buffer_list", m_buffer_list);
    c->setContextProperty("texture_list", m_texture_list);
    c->setContextProperty("material_list", m_material_list);
    c->setContextProperty("light_list", m_light_list);
    c->setContextProperty("mesh_list", m_mesh_list);
    c->setContextProperty("object_list", m_object_list);

    c->setContextProperty("filtered_object_list", m_object_filter);

    c->setContextProperty("entity_notifier", m_ent_notifier);
    c->setContextProperty("material_notifier", m_mat_notifier);

    c->setContextProperty("document_methods", &m_document_methods);
}

bool State::start_connection(QString name, QString url) {
    qDebug() << Q_FUNC_INFO << name << url;
    handle_disconnect();

    if (name.isEmpty()) { name = QHostInfo::localHostName(); }

    QUrl proper_url(url);

    if (!proper_url.isValid()) {
        qWarning() << "Bad url!";
        return false;
    }

    m_client_conn = new nooc::ClientConnection(this);

    connect(m_client_conn,
            &nooc::ClientConnection::disconnected,
            m_client_conn,
            &QObject::deleteLater);

    connect(m_client_conn,
            &nooc::ClientConnection::disconnected,
            this,
            &State::handle_disconnect);

    connect(m_client_conn,
            &nooc::ClientConnection::connected,
            this,
            &State::handle_connect);

    connect(m_client_conn, &nooc::ClientConnection::initialized, []() {
        qDebug() << "ALL SCENE READY";
    });

    nooc::ClientDelegates delegates;
    delegates.client_name = name;

    delegates.tex_maker = [this](noo::TextureID           id,
                                 nooc::TextureInit const& md) {
        return m_texture_list->add_item(id, md);
    };
    delegates.buffer_maker = [this](noo::BufferID           id,
                                    nooc::BufferInit const& md) {
        return m_buffer_list->add_item(id, md);
    };
    delegates.buffer_view_maker = [this](noo::BufferViewID           id,
                                         nooc::BufferViewInit const& md) {
        return m_buffer_view_list->add_item(id, md);
    };
    delegates.table_maker = [this](noo::TableID id, nooc::TableInit const& md) {
        return m_table_list->add_item(id, md);
    };
    delegates.light_maker = [this](noo::LightID id, nooc::LightInit const& md) {
        return m_light_list->add_item(id, md);
    };
    delegates.mat_maker = [this](noo::MaterialID           id,
                                 nooc::MaterialInit const& md) {
        return m_material_list->add_item(id, md, m_mat_notifier);
    };
    delegates.mesh_maker = [this](noo::GeometryID       id,
                                  nooc::MeshInit const& md) {
        return m_mesh_list->add_item(id, md, nullptr);
    };
    delegates.object_maker = [this](noo::EntityID           id,
                                    nooc::EntityInit const& md) {
        return m_object_list->add_item(id, md, m_ent_notifier);
    };
    delegates.sig_maker = [this](noo::SignalID id, nooc::SignalInit const& md) {
        return m_signal_list->add_item(id, md);
    };
    delegates.method_maker = [this](noo::MethodID           id,
                                    nooc::MethodInit const& md) {
        return m_method_list->add_item(id, md);
    };
    delegates.doc_maker = [this]() {
        auto p = std::make_unique<ExDoc>(&this->m_document_methods);

        m_current_doc = p.get();

        connect(
            p.get(), &ExDoc::updated, this, &State::handle_document_updated);

        return p;
    };


    m_client_conn->open(proper_url, std::move(delegates));

    set_connection_state(0);

    return true;
}

void State::disconnect() {
    handle_disconnect();
}

void State::set_connection_state(int connection_state) {
    if (m_connection_state == connection_state) return;

    m_connection_state = connection_state;
    emit connection_state_changed(m_connection_state);
}

QString State::get_hostname() {
    return QHostInfo::localHostName();
}

void State::exec_debug() {
    emit debug_tree();
}

void State::handle_connect() {
    set_connection_state(1);
}

void State::handle_disconnect() {
    if (m_client_conn) m_client_conn.data()->deleteLater();
    set_connection_state(-1);
}

void State::handle_document_updated() {
    ExDoc* d = qobject_cast<ExDoc*>(sender());

    if (!d) return;

    m_document_methods.set(d->attached_methods().list());
}

void State::launch_table_view(int i) {
    auto* p = m_table_list->get_item(i);

    if (!p) return;

    auto slot = p->get_id();
    auto gen  = p->get_id_gen();

    auto id = noo::TableID(slot, gen);

    auto tbl_ptr = m_client_conn->get(id);

    if (!tbl_ptr) return;

    auto del_ptr = dynamic_cast<ExTable*>(tbl_ptr);

    if (!del_ptr) return;


    new TableViewer(del_ptr, this);
}

void State::launch_chart_view(int i) {
    auto* p = m_table_list->get_item(i);

    if (!p) return;

    auto slot = p->get_id();
    auto gen  = p->get_id_gen();

    auto id = noo::TableID(slot, gen);

    auto tbl_ptr = m_client_conn->get(id);

    if (!tbl_ptr) return;

    auto del_ptr = dynamic_cast<ExTable*>(tbl_ptr);

    if (!del_ptr) return;


    new ChartViewer(del_ptr, this);
}
