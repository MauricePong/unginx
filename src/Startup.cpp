#include <QCoreApplication>
#include <QDir>
#include "httplistener.h"
#include "templatecache.h"
#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"
#include "requestmapper.h"

#include "Startup.h"
using namespace stefanfrings;

/** Cache for template files */
TemplateCache* templateCache = nullptr;

/** Storage for session cookies */
HttpSessionStore* sessionStore = nullptr;

/** Controller for static files */
StaticFileController* staticFileController = nullptr;

/** Redirects log messages to a file */
FileLogger* logger = nullptr;

/**
 * Search the configuration file.
 * Aborts the application if not found.
 * @return The valid filename
 */
QString searchConfigFile() {
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QFile file;
    file.setFileName(binDir+"/webapp.ini");
    if (!file.exists()) {
        file.setFileName(binDir+"/etc/webapp.ini");
        if (!file.exists()) {
            file.setFileName(binDir+"/../etc/webapp.ini");
            if (!file.exists()) {
                file.setFileName(binDir+"/../"+appName+"/etc/webapp.ini");
                if (!file.exists()) {
                    file.setFileName(binDir+"/../../"+appName+"/etc/webapp.ini");
                    if (!file.exists()) {
                        file.setFileName(binDir+"/../../../../../"+appName+"/etc/webapp.ini");
                        if (!file.exists()) {
                            file.setFileName(QDir::rootPath()+"etc/webapp.ini");
                        }
                    }
                }
            }
        }
    }
    if (file.exists()) {
        QString configFileName=QDir(file.fileName()).canonicalPath();
        qDebug("using config file %s", qPrintable(configFileName));
        return configFileName;
    }
    else {
        qFatal("config file not found");
    }

    return QString();
}

void Startup::start() {
    QCoreApplication* app = application();
    app->setApplicationName("unginx");
    app->setOrganizationName("TBS");
    // Log the library version
    qDebug("unginx has version %s",getQtWebAppLibVersion());

    QString configFileName=searchConfigFile();
    // Configure logging
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat,app);
    logSettings->beginGroup("logging");
    logger=new FileLogger(logSettings,10000,app);
    logger->installMsgHandler();

    // Static file controller
    QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat,app);
    fileSettings->beginGroup("files");
    staticFileController=new StaticFileController(fileSettings,app);

    // Session store
    QSettings* sessionSettings=new QSettings(configFileName,QSettings::IniFormat,app);
    sessionSettings->beginGroup("sessions");
    sessionStore=new HttpSessionStore(sessionSettings,app);

    // Configure template cache
    QSettings* templateSettings=new QSettings(configFileName,QSettings::IniFormat,app);
    templateSettings->beginGroup("templates");
    templateCache=new TemplateCache(templateSettings,app);

    // HTTP server
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,app);
    listenerSettings->beginGroup("listener");
    listener=new HttpListener(listenerSettings,new RequestMapper(app),app);
}

void Startup::stop() {
    if(!listener){
        delete listener;
    }
    qWarning("Webserver has been stopped");
}

Startup::Startup(int argc, char *argv[])
    : QtService<QCoreApplication>(argc, argv, "unginx") {
    listener = NULL;
    setServiceDescription("Welcome to unginx");
    setStartupType(QtServiceController::AutoStartup);
}
