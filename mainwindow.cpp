#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <open62541.h>

static void beforeProgressVariableRead(UA_Server *server,
                                       const UA_NodeId *sessionId, void *sessionContext,
                                       const UA_NodeId *nodeId, void *nodeContext,
                                       const UA_NumericRange *range, const UA_DataValue *data)
{ }

static void afterProgressVariableWrite(UA_Server *server,
                                       const UA_NodeId *sessionId, void *sessionContext,
                                       const UA_NodeId *nodeId, void *nodeContext,
                                       const UA_NumericRange *range, const UA_DataValue *data)
{
    auto* mainWindow = reinterpret_cast<MainWindow*>(nodeContext);
    if (UA_Variant_hasScalarType(&data->value, &UA_TYPES[UA_TYPES_INT32])) {
        int32_t newValue = *reinterpret_cast<int32_t*>(data->value.data);
        mainWindow->updateProgressBarValue(newValue);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    config(nullptr),
    server(nullptr)
{
    ui->setupUi(this);

    blinkTimer.setInterval(500);
    blinkTimer.setSingleShot(false);
    QObject::connect(&blinkTimer, &QTimer::timeout,
                     this, &MainWindow::blink);

    messagePumpTimer.setInterval(10);
    messagePumpTimer.setSingleShot(false);
    QObject::connect(&messagePumpTimer, &QTimer::timeout,
                     this, &MainWindow::opcMessagePump);

    config = UA_ServerConfig_new_default();
    server = UA_Server_new(config);

    addBoolVariable();
    addCallbackVariable();
    addObject();

    UA_Server_run_startup(server);

    messagePumpTimer.start();
}

MainWindow::~MainWindow()
{
    messagePumpTimer.stop();
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    delete ui;
}

void MainWindow::blink()
{
    auto checked = ui->led->isChecked();
    ui->led->setChecked(!checked);
}

void MainWindow::updateProgressBarValue(int value)
{
    ui->variableProgress->setValue(value);
}

void MainWindow::opcMessagePump()
{
    UA_Server_run_iterate(server, 0);
}

void MainWindow::addBoolVariable()
{
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Boolean theVariable = false;
    UA_Variant_setScalar(&attr.value, &theVariable, &UA_TYPES[UA_TYPES_BOOLEAN]);
    attr.description = UA_LOCALIZEDTEXT("en-US", "The variable");
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "The description of the variable");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId theVariableNodeId = UA_NODEID_STRING(1, "theVariable");
    UA_QualifiedName theVariableName = UA_QUALIFIEDNAME(1, "the variable");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_Server_addVariableNode(server, theVariableNodeId, parentNodeId,
                              parentReferenceNodeId, theVariableName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, nullptr, nullptr);
}

void MainWindow::setBoolVariableValue(bool value)
{
    UA_NodeId theVariableNodeId = UA_NODEID_STRING(1, "theVariable");
    UA_Boolean theNewValue = value;
    UA_Variant var;
    UA_Variant_init(&var);
    UA_Variant_setScalar(&var, &theNewValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_writeValue(server, theVariableNodeId, var);
}

void MainWindow::addCallbackVariable()
{
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_Int32 theVariable = 0;
    UA_Variant_setScalar(&attr.value, &theVariable, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT("en-US", "Progress Value");
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "A progress value shown as progress bar");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId theVariableNodeId = UA_NODEID_STRING(1, "progressValue");
    UA_QualifiedName theVariableName = UA_QUALIFIEDNAME(1, "progress variable");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);

    UA_Server_addVariableNode(server, theVariableNodeId, parentNodeId,
                              parentReferenceNodeId, theVariableName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, this, nullptr);

    UA_ValueCallback callback;
    callback.onWrite = afterProgressVariableWrite;
    callback.onRead = beforeProgressVariableRead;
    UA_Server_setVariableNode_valueCallback(server, theVariableNodeId, callback);
}

void MainWindow::addObject()
{
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "The Object");
    UA_Server_addObjectNode(server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, "The Object"),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            attr, nullptr, nullptr);
}

void MainWindow::on_statusCheckbox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        setBoolVariableValue(true);
    else if (arg1 == Qt::Unchecked)
        setBoolVariableValue(false);
}
