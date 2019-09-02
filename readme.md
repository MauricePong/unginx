<Unginx_TemplateController_H>

#ifndef UNGINX_TEMPLATECONTROLLER_H
#define UNGINX_TEMPLATECONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class Unginx_TemplateController: public HttpRequestHandler {
    Q_OBJECT
public:
    Unginx_TemplateController(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
private:
    QList<QString> list;
    int count;
};

#endif // DATATEMPLATECONTROLLER_H
////////////////////////////////////////////////////////

<Unginx_TemplateController_CPP>
#include "requestmapper.h"
#include "unginx_templatecontroller.h"
#include "template.h"

Unginx_TemplateController::Unginx_TemplateController(QObject* parent)
    : HttpRequestHandler(parent) {
    list.append("Robert");
    list.append("Lisa");
    list.append("Hannah");
    list.append("Ludwig");
    list.append("Miranda");
    list.append("Fracesco");
    list.append("Kim");
    list.append("Jacko");
    count = 0;
}

void Unginx_TemplateController::service(HttpRequest &request, HttpResponse &response) {
    QByteArray language=request.getHeader("Accept-Language");
    qDebug("language=%s",qPrintable(language));
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    Template t=RequestMapper::templateCache->getTemplate("testdata",language);

    QByteArray username=request.getParameter("username");
    QByteArray password=request.getParameter("password");
    qDebug("username=%s",username.constData());
    qDebug("password=%s",password.constData());

    t.setVariable("username",username);
    t.setVariable("password",password);

    if(username.isEmpty()||password.isEmpty()){
        t.setCondition("logged-in",false);
    }else{
        t.setCondition("logged-in",true);
        if (username== "root" && password== "root") {
            t.setVariable("status",QString("OK"));
        }else {
            t.setVariable("status",QString("Fail"));
        }
    }


    t.loop("row",list.size());
    for(int i=0; i<list.size(); i++) {
        QString number=QString::number(i);
        QString name=list.at(i);
        t.setVariable("row"+number+".number",number);
        t.setVariable("row"+number+".name",name);
    }
    response.write("Hello World")
    response.write(t.toUtf8(),true);
}

login.html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Login</title>
    <link rel="stylesheet" type="text/css" href="Login.css"/>
</head>
<body>
    <div id="login">
        <h1>Login</h1>
        <form method="post">
            <input type="text" required="required" placeholder="用户名" name="u"></input>
            <input type="password" required="required" placeholder="密码" name="p"></input>
            <button class="but" type="submit">登录</button>
        </form>
    </div>
</body>
</html>


login.css

html{
    width: 100%;
    height: 100%;
    overflow: hidden;
    font-style: sans-serif;
}
body{
    width: 100%;
    height: 100%;
    font-family: 'Open Sans',sans-serif;
    margin: 0;
    background-color: #4A374A;
}
#login{
    position: absolute;
    top: 50%;
    left:50%;
    margin: -150px 0 0 -150px;
    width: 300px;
    height: 300px;
}
#login h1{
    color: #fff;
    text-shadow:0 0 10px;
    letter-spacing: 1px;
    text-align: center;
}
h1{
    font-size: 2em;
    margin: 0.67em 0;
}
input{
    width: 278px;
    height: 18px;
    margin-bottom: 10px;
    outline: none;
    padding: 10px;
    font-size: 13px;
    color: #fff;
    text-shadow:1px 1px 1px;
    border-top: 1px solid #312E3D;
    border-left: 1px solid #312E3D;
    border-right: 1px solid #312E3D;
    border-bottom: 1px solid #56536A;
    border-radius: 4px;
    background-color: #2D2D3F;
}
.but{
    width: 300px;
    min-height: 20px;
    display: block;
    background-color: #4a77d4;
    border: 1px solid #3762bc;
    color: #fff;
    padding: 9px 14px;
    font-size: 15px;
    line-height: normal;
    border-radius: 5px;
    margin: 0;
}
