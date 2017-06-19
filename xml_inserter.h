#ifndef XML_INSERTER_H
#define XML_INSERTER_H

#include <QDomDocument>


class InvalidInsertMode
{

};

void appendTextChild(QDomDocument& doc, QDomElement& parent,const QString& name,const QString& value);
QDomElement locate(QDomDocument& doc, QDomElement node, const QString& path, const bool alwaysAdd);
void insert(QDomDocument& doc, QDomElement node,const QString& path, const QString& value);

#endif // XML_INSERTER_H
