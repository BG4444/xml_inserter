#include "xml_inserter.h"

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

        posDelim=path.indexOf(delim,posStart);

        const int tagEnd=(posDelim == -1) ? path.length() : posDelim;

        const int tagLength=tagEnd - posStart;

        const QString tag=QStringRef(&path,posStart, tagLength).toString();


        auto child=node.firstChildElement(tag);

        const bool nullChild = child.isNull();

        node = (nullChild || alwaysAdd) ? node.appendChild(doc.createElement(tag)).toElement()
                              : child;
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

