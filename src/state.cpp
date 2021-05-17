#include "state.h"

#include "chartviewer.h"
#include "noo_client_interface.h"
#include "noo_common.h"
#include "tableviewer.h"
#include "variant_tools.h"

#include <QHostInfo>
#include <QQmlContext>
#include <QTreeWidgetItem>
#include <QUrl>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QQmlEngine>
#include <Qt3DCore/QComponent>

static State* current_state = nullptr;

State::State(QObject* parent) : QObject(parent) {
    current_state = this;

    set_connection_state(-1);

    m_root_entity = new Qt3DCore::QEntity();

    m_method_list  = std::make_shared<ComponentListModel>(ExMethod::header());
    m_signal_list  = std::make_shared<ComponentListModel>(ExSignal::header());
    m_table_list   = std::make_shared<ComponentListModel>(ExTable::header());
    m_buffer_list  = std::make_shared<ComponentListModel>(ExBuffer::header());
    m_texture_list = std::make_shared<ComponentListModel>(ExTexture::header());
    m_material_list =
        std::make_shared<ComponentListModel>(ExMaterial::header());
    m_light_list  = std::make_shared<ComponentListModel>(ExLight::header());
    m_mesh_list   = std::make_shared<ComponentListModel>(ExMesh::header());
    m_object_list = std::make_shared<ComponentListModel>(ExObject::header());
}

State::~State() { }

void State::link(QQmlContext* c) {
    c->setContextProperty("app_state", this);

    c->setContextProperty("argument_model", &m_argument_table_model);

    c->setContextProperty("method_list", m_method_list.get());
    c->setContextProperty("signal_list", m_signal_list.get());
    c->setContextProperty("table_list", m_table_list.get());
    c->setContextProperty("buffer_list", m_buffer_list.get());
    c->setContextProperty("texture_list", m_texture_list.get());
    c->setContextProperty("material_list", m_material_list.get());
    c->setContextProperty("light_list", m_light_list.get());
    c->setContextProperty("mesh_list", m_mesh_list.get());
    c->setContextProperty("object_list", m_object_list.get());

    c->setContextProperty("document_methods", &m_document_methods);

    qmlRegisterType<EntityShim>("EntityShim", 1, 0, "EntityShim");
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

    nooc::ClientDelegates delegates;
    delegates.client_name = name;

    delegates.tex_maker = [this](noo::TextureID           id,
                                 nooc::TextureData const& md) {
        return std::make_shared<ExTexture>(id, md, m_texture_list);
    };
    delegates.buffer_maker = [this](noo::BufferID           id,
                                    nooc::BufferData const& md) {
        return std::make_shared<ExBuffer>(id, md, m_buffer_list, m_root_entity);
    };
    delegates.table_maker = [this](noo::TableID id, nooc::TableData const& md) {
        return std::make_shared<ExTable>(id, md, m_table_list);
    };
    delegates.light_maker = [this](noo::LightID id, nooc::LightData const& md) {
        return std::make_shared<ExLight>(id, md, m_light_list, m_root_entity);
    };
    delegates.mat_maker = [this](noo::MaterialID           id,
                                 nooc::MaterialData const& md) {
        return std::make_shared<ExMaterial>(
            id, md, m_material_list, m_root_entity);
    };
    delegates.mesh_maker = [this](noo::MeshID id, nooc::MeshData const& md) {
        return std::make_shared<ExMesh>(id, md, m_mesh_list, m_root_entity);
    };
    delegates.object_maker = [this](noo::ObjectID                 id,
                                    nooc::ObjectUpdateData const& md) {
        return std::make_shared<ExObject>(id, md, m_object_list, m_root_entity);
    };
    delegates.sig_maker = [this](noo::SignalID id, nooc::SignalData const& md) {
        return std::make_shared<ExSignal>(id, md, m_signal_list);
    };
    delegates.method_maker = [this](noo::MethodID           id,
                                    nooc::MethodData const& md) {
        return std::make_shared<ExMethod>(id, md, m_method_list);
    };
    delegates.doc_maker = [this]() {
        auto p = std::make_shared<ExDoc>();

        m_current_doc = p;

        connect(
            p.get(), &ExDoc::updated, this, &State::handle_document_updated);

        return p;
    };


    m_client_conn->open(proper_url, std::move(delegates));

    set_connection_state(0);

    return true;
}

void State::set_connection_state(int connection_state) {
    if (m_connection_state == connection_state) return;

    m_connection_state = connection_state;
    emit connection_state_changed(m_connection_state);
}

QString State::get_hostname() {
    return QHostInfo::localHostName();
}

static QString convert_string(std::string_view s) {
    return QString::fromLatin1(s.data(), s.size());
}

void State::ask_to_call_doc_method(int index) {
    auto ptr = m_document_methods.get_method_row(index);

    if (!ptr) return;

    m_current_doc_method = ptr;

    m_argument_table_model.set_method_name(
        convert_string(m_current_doc_method->name()));

    m_argument_table_model.set_method_documentation(
        m_current_doc_method->documentation());

    QStringList arg_names = m_current_doc_method->argument_names();
    QStringList arg_deets = m_current_doc_method->argument_details();

    QVector<Argument> args;

    for (int i = 0; i < std::min(arg_names.size(), arg_deets.size()); i++) {
        args << Argument { arg_names[i], arg_deets[i], "null", false };
    }

    m_argument_table_model.reset(args);

    emit open_call_method_page();
}


// Forward
static noo::AnyVar from_json(QJsonValue const& s);

static noo::AnyVar from_json(QJsonArray const& arr) {
    // TODO: we can do better
    bool all_reals = std::all_of(
        arr.begin(), arr.end(), [](auto const& v) { return v.isDouble(); });

    if (all_reals) {
        std::vector<double> reals;
        reals.reserve(arr.size());

        for (auto const& v : arr) {
            reals.push_back(v.toDouble());
        }

        return noo::AnyVar(std::move(reals));
    }

    noo::AnyVarList ret;

    ret.reserve(arr.size());

    for (auto const& v : arr) {
        ret.push_back(from_json(v));
    }

    return std::move(ret);
}

static noo::AnyVar from_json(QJsonObject const& obj) {
    noo::AnyVarMap ret;

    for (auto iter = obj.begin(); iter != obj.end(); ++iter) {
        ret.try_emplace(iter.key().toStdString(), from_json(iter.value()));
    }

    return ret;
}

static noo::AnyVar from_json(QJsonValue const& s) {
    switch (s.type()) {
    case QJsonValue::Null: return std::monostate();
    case QJsonValue::Bool: return (int64_t)s.toBool();
    case QJsonValue::Double: return s.toDouble();
    case QJsonValue::String: return s.toString().toStdString();
    case QJsonValue::Array: return from_json(s.toArray());
    case QJsonValue::Object: return from_json(s.toObject());
    case QJsonValue::Undefined: return std::monostate();
    }

    return std::monostate();
}

static std::variant<noo::AnyVar, QString> from_raw_string(QString const& s) {

    auto ls = QString("[ %1 ]").arg(s);

    // to save us effort, we are going to hijack the document reader

    QJsonParseError error;

    auto doc = QJsonDocument::fromJson(ls.toLocal8Bit(), &error);

    if (error.error == QJsonParseError::NoError) {
        return from_json(doc.array().at(0));
    }

    return error.errorString();
}

void State::invoke_doc_method() {

    noo::AnyVarList avlist;

    for (auto const& d : m_argument_table_model.vector()) {
        auto ret = from_raw_string(d.current_value);

        VMATCH(
            ret,
            VCASE(noo::AnyVar & a) { avlist.push_back(std::move(a)); },
            VCASE(QString) { avlist.push_back({}); });
    }

    auto* r =
        m_current_doc->attached_methods()
            .new_call_by_delegate<NormalizeStringReply>(m_current_doc_method);

    if (!r) return;

    connect(
        r, &NormalizeStringReply::recv, this, &State::method_result_recieved);

    connect(r,
            &NormalizeStringReply::recv_fail,
            this,
            &State::method_error_recieved);


    r->call_direct(std::move(avlist));
}

Qt3DCore::QEntity* State::scene_root() {
    Q_ASSERT(m_root_entity);
    return m_root_entity;
}

void State::exec_debug() {
    emit debug_tree();
}

void State::handle_connect() {
    set_connection_state(1);
}

void State::handle_disconnect() {
    if (m_client_conn) delete m_client_conn.data();
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

    auto del_ptr = std::dynamic_pointer_cast<ExTable>(tbl_ptr);

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

    auto del_ptr = std::dynamic_pointer_cast<ExTable>(tbl_ptr);

    if (!del_ptr) return;


    new ChartViewer(del_ptr, this);
}


// =============================================================================

static void node_debug(Qt3DCore::QNode* n, int d) {
    {
        auto deb = qDebug();

        QString r;

        for (int i = 0; i < d; i++) {
            r += QString("-");
        }

        auto ent = dynamic_cast<Qt3DCore::QEntity*>(n);

        QStringList comp;

        deb << r << n->objectName() << n;

        if (ent) {
            for (auto comp : ent->components()) {
                deb << comp;
            }
        }
    }

    auto c = n->childNodes();

    for (auto cn : c) {
        node_debug(cn, d + 1);
    }
}

EntityShim::EntityShim(Qt3DCore::QNode* n) : Qt3DCore::QEntity(n) {
    qDebug() << Q_FUNC_INFO << n;

    connect(
        current_state, &State::debug_tree, [this]() { node_debug(this, 0); });

    current_state->scene_root()->setParent(this);

    this->setObjectName("Test");
}

EntityShim::~EntityShim() {
    qDebug() << Q_FUNC_INFO;
}

// =============================================================================

void NormalizeStringReply::interpret() {
    auto str = m_var.dump_string();

    emit recv(noo::to_qstring(str));
}
