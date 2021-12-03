#include "xml_inserter.h"
#include <QTextCodec>
#include <SerException.h>

void appendTextChild(QDomDocument& doc, QDomElement& parent,const QString& name,const QString& value)
{

    QDomElement elm=doc.createElement(name);
    elm.appendChild(doc.createTextNode(value));

    parent.appendChild(elm);
}


QDomElement locate(QDomDocument& doc, QDomElement node,const QString& path,const bool alwaysAdd)
{
    if(path.isEmpty())
    {
        return node;
    }

    const QChar delim('/');

    int posDelim=-1;

    while(true)
    {
        const int posStart=posDelim+1;

        posDelim=path.indexOf(delim, posStart);

        const int tagEnd=(posDelim == -1) ? path.length() : posDelim;

        const int tagLength=tagEnd - posStart;

        const QString tag=QStringRef(&path, posStart, tagLength).toString();


        auto child=node.firstChildElement(tag);

        if(tag!=".")
        {

            const bool nullChild = child.isNull();
            node = (nullChild || alwaysAdd) ? node.appendChild(doc.createElement(tag)).toElement()
                                            : child;
        }

        if(posDelim==-1)
        {
            return node;
        }
    }
}


void insert(QDomDocument& doc, QDomElement node,const QString& path, const QString& value)
{
    const QChar delim('/');

    int posDelim=path.indexOf(delim,0);

    if(posDelim!=1)
    {
        throw InvalidInsertMode();
    }

    const char mode = path[0].toLatin1();

    while(true)
    {
        const int posStart=posDelim+1;

        posDelim=path.indexOf(delim,posStart);

        const int tagEnd=(posDelim == -1) ? path.length() : posDelim;

        const int tagLength=tagEnd - posStart;

        const QString tag=QStringRef(&path,posStart, tagLength).toString();

        if(posDelim==-1)
        {
            switch(mode)
            {
                case 'a':
                    node.setAttribute(tag,value);
                    return;
                case 'v':
                    appendTextChild(doc,node,tag,value);
                    return;
                default:
                    throw InvalidInsertMode();
            }
        }

        auto child=node.firstChildElement(tag);
        node = child.isNull() ? node.appendChild(doc.createElement(tag)).toElement()
                              : child;
    }
}

QDomElement findNode(const QString& tag, const QString& path, const int posStart, const QDomNodeList& children, const QTextCodec *codecConsole)
{
    QString nodesList;
    int i=children.size();
    while(--i>=0)
    {
        const auto& cur=children.at(i);
        nodesList += QString("[%1] '%2' %3\n").arg(i).arg(cur.nodeName()).arg(cur.isElement());
        if(cur.nodeName()==tag && cur.isElement())
        {
            break;
        }
    }

    if(i==-1)
    {
        const auto msgText=codecConsole->fromUnicode(QString("\nXML Path not found!\nPath: %1\ntag %2\nat pos %3\nExisting nodes is\n%4").arg(path).arg(tag).arg(posStart).arg(nodesList));

        throw SerException(__FILE__,__LINE__,msgText.data());
    }
    else
    {
        return children.at(i).toElement();
    }

}

QString readNode(QDomElement node, const QString &path, const QTextCodec *codecConsole)
{
    const QChar delim('/');

    int posDelim=path.indexOf(delim,0);

    if(posDelim!=1)
    {
        throw SerException(__FILE__,__LINE__);
    }

    const char mode = path[0].toLatin1();

    while(true)
    {
        const int posStart=posDelim+1;

        posDelim=path.indexOf(delim,posStart);

        const int tagEnd=(posDelim == -1) ? path.length() : posDelim;

        const int tagLength=tagEnd - posStart;

        const QString tag=QStringRef(&path,posStart, tagLength).toString();

        if(posDelim==-1)
        {
            switch(mode)
            {
              case 'a':
              {
                if(node.attributes().contains(tag))
                {
                    return node.attribute(tag);
                }
                else
                {
                    const auto msgText=codecConsole->fromUnicode(QString("\nXML Attribute not found!\nPath: %1\ntag %2\nat pos %3\n").arg(path).arg(tag).arg(posStart));
                    throw SerException(__FILE__,__LINE__,msgText.data());
                }
              }
              case 'v':
              {
                   return findNode(tag, path,posStart, node.childNodes(),codecConsole).text();
              }
              default:
              {
                   throw SerException(__FILE__,__LINE__);
              }
            }
        }
        node = findNode(tag, path,posStart, node.childNodes(),codecConsole);
    }
}

QDomElement harvestElement(QDomElement node, const QString &path, const QTextCodec *codecConsole)
{
    const QChar delim('/');

    int posDelim=-1;

    do
    {
        const int posStart=posDelim+1;

        posDelim=path.indexOf(delim,posStart);

        const int tagEnd=(posDelim == -1) ? path.length() : posDelim;

        const int tagLength=tagEnd - posStart;

        const QString tag=QStringRef(&path,posStart, tagLength).toString();

        node = findNode(tag, path,posStart, node.childNodes(),codecConsole);
    }
    while(posDelim!=-1);
    return node;
}
