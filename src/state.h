#ifndef STATE_H
#define STATE_H

#include "argumenttablemodel.h"
#include "attachedmethodlistmodel.h"
#include "componentlistmodel.h"
#include "delegates/delegates.h"
//#include "delegates/exbuffer.h"
#include "delegates/exdoc.h"
//#include "delegates/exlight.h"
//#include "delegates/exmaterial.h"
//#include "delegates/exmesh.h"
//#include "delegates/exobject.h"
//#include "delegates/exsignal.h"
//#include "delegates/extable.h"
//#include "delegates/extexture.h"


#include <QEntity>
#include <QObject>
#include <QPointer>

#include <Qt3DCore/QEntity>

class QQmlContext;

namespace nooc {
class ClientConnection;
}

class TaggedNameObjectFilter;

class ExSignal;
class ExTable;
class ExBuffer;
class ExTexture;
class ExMaterial;
class ExLight;
class ExMesh;
class ExObject;

class State : public QObject {
    Q_OBJECT

    QPointer<nooc::ClientConnection> m_client_conn;

    Q_PROPERTY(int connection_state READ connection_state WRITE
                   set_connection_state NOTIFY connection_state_changed)

    int m_connection_state;

    UniqueQPtr<Qt3DCore::QEntity> m_root_entity;


    QPointer<ExMethod> m_current_doc_method;
    ArgumentTableModel m_argument_table_model;

    AttachedMethodListModel m_document_methods;

    ComponentListModel<ExMethod>*   m_method_list;
    ComponentListModel<ExSignal>*   m_signal_list;
    ComponentListModel<ExTable>*    m_table_list;
    ComponentListModel<ExBuffer>*   m_buffer_list;
    ComponentListModel<ExTexture>*  m_texture_list;
    ComponentListModel<ExMaterial>* m_material_list;
    ComponentListModel<ExLight>*    m_light_list;
    ComponentListModel<ExMesh>*     m_mesh_list;
    ComponentListModel<ExObject>*   m_object_list;
    QPointer<ExDoc>                 m_current_doc;

    TaggedNameObjectFilter* m_object_filter;


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
    void launch_chart_view(int);

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
