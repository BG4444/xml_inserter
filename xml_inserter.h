#ifndef XML_INSERTER_H
#define XML_INSERTER_H

#include <QDomDocument>


class InvalidInsertMode
{

};

void appendTextChild(QDomDocument& doc, QDomElement& parent,const QString& name,const QString& value);

QDomElement locate(QDomDocument& doc, QDomElement node, const QString& path, const bool alwaysAdd);

//Insert attribure or value into DOM by path. 
//'a' insert("a/foo/bar","smth") for attribute (<foo bar="smth"/>)
//'v' insert("v/foo","smth") for attribute (<foo>smth</foo>)

void insert(QDomDocument& doc, QDomElement node,const QString& path, const QString& value);

QString readNode(QDomElement node, const QString &path, const QTextCodec *codecConsole);
QDomElement harvestElement(QDomElement node, const QString &path, const QTextCodec *codecConsole);

#endif // XML_INSERTER_H
