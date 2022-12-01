#ifndef STATE_H
#define STATE_H

#include "argumenttablemodel.h"
#include "attachedmethodlistmodel.h"
#include "componentlistmodel.h"
#include "delegates/delegates.h"
#include "delegates/exdoc.h"

#include <QObject>
#include <QPointer>

class QQmlContext;

namespace nooc {
class Client;
}

class TaggedNameObjectFilter;

class ExSignal;
class ExTable;
class ExBuffer;
class ExBufferView;
class ExTexture;
class ExSampler;
class ExImage;
class ExMaterial;
class ExLight;
class ExMesh;
class ExObject;
class EntityChangeNotifier;
class MaterialChangeNotifier;
class TextureChangeNotifier;


class State : public QObject {
    Q_OBJECT

    QPointer<nooc::Client> m_client_conn;

    Q_PROPERTY(int connection_state READ connection_state WRITE
                   set_connection_state NOTIFY connection_state_changed)

    int m_connection_state;

    QPointer<ExMethod> m_current_doc_method;
    ArgumentTableModel m_argument_table_model;

    AttachedMethodListModel m_document_methods;

    EntityChangeNotifier*   m_ent_notifier;
    MaterialChangeNotifier* m_mat_notifier;
    TextureChangeNotifier*  m_tex_notifier;

    ComponentListModel<ExMethod>*     m_method_list;
    ComponentListModel<ExSignal>*     m_signal_list;
    ComponentListModel<ExTable>*      m_table_list;
    ComponentListModel<ExBuffer>*     m_buffer_list;
    ComponentListModel<ExBufferView>* m_buffer_view_list;
    ComponentListModel<ExTexture>*    m_texture_list;
    ComponentListModel<ExSampler>*    m_sampler_list;
    ComponentListModel<ExImage>*      m_image_list;
    ComponentListModel<ExMaterial>*   m_material_list;
    ComponentListModel<ExLight>*      m_light_list;
    ComponentListModel<ExMesh>*       m_mesh_list;
    ComponentListModel<ExObject>*     m_object_list;
    QPointer<ExDoc>                   m_current_doc;


    TaggedNameObjectFilter* m_object_filter;


public:
    explicit State(QObject* parent = nullptr);
    ~State();

    void link(QQmlContext*);

    int connection_state() const { return m_connection_state; }

public slots:
    bool start_connection(QString name, QString url);
    void disconnect();

    void set_connection_state(int connection_state);

    QString get_hostname();

    void exec_debug();

    void launch_table_view(int);
    void launch_chart_view(int);

private slots:
    void handle_connect();
    void handle_disconnect();
    void handle_document_updated();


signals:
    void connection_state_changed(int connection_state);

    void debug_tree();
};
#endif // STATE_H
