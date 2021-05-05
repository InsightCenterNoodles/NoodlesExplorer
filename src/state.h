#ifndef STATE_H
#define STATE_H

#include "argumenttablemodel.h"
#include "attachedmethodlistmodel.h"
#include "componentlistmodel.h"
#include "delegates.h"

#include <QObject>
#include <QPointer>

#include <Qt3DCore/QEntity>

class QQmlContext;

namespace nooc {
class ClientConnection;
}

class State : public QObject {
    Q_OBJECT

    QPointer<nooc::ClientConnection> m_client_conn;

    Q_PROPERTY(int connection_state READ connection_state WRITE
                   set_connection_state NOTIFY connection_state_changed)

    int m_connection_state;

    UniqueQPtr<Qt3DCore::QEntity> m_root_entity;


    std::shared_ptr<ExMethod> m_current_doc_method;
    ArgumentTableModel        m_argument_table_model;

    AttachedMethodListModel m_document_methods;

    std::shared_ptr<ComponentListModel> m_method_list;
    std::shared_ptr<ComponentListModel> m_signal_list;
    std::shared_ptr<ComponentListModel> m_table_list;
    std::shared_ptr<ComponentListModel> m_buffer_list;
    std::shared_ptr<ComponentListModel> m_texture_list;
    std::shared_ptr<ComponentListModel> m_material_list;
    std::shared_ptr<ComponentListModel> m_light_list;
    std::shared_ptr<ComponentListModel> m_mesh_list;
    std::shared_ptr<ComponentListModel> m_object_list;
    std::shared_ptr<ExDoc>              m_current_doc;


public:
    explicit State(QObject* parent = nullptr);
    ~State();

    void link(QQmlContext*);

    int connection_state() const { return m_connection_state; }

public slots:
    bool start_connection(QString name, QString url);

    void set_connection_state(int connection_state);

    QString get_hostname();

    void ask_to_call_doc_method(int index);
    void invoke_doc_method();

    Qt3DCore::QEntity* scene_root();

    void exec_debug();

    void launch_table_view(int);

private slots:
    void handle_connect();
    void handle_disconnect();
    void handle_document_updated();


signals:
    void connection_state_changed(int connection_state);

    void open_call_method_page();

    void method_result_recieved(QString);
    void method_error_recieved(QString);

    void debug_tree();
};


class EntityShim : public Qt3DCore::QEntity {
public:
    EntityShim(Qt3DCore::QNode* n = nullptr);
    ~EntityShim();
};

class NormalizeStringReply : public nooc::PendingMethodReply {
    Q_OBJECT

public:
    using PendingMethodReply::PendingMethodReply;

    void interpret() override;
signals:
    void recv(QString);
};

#endif // STATE_H
