#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H


#include <QSyntaxHighlighter>

class XMLHighlighter : public QSyntaxHighlighter
{
public:
    XMLHighlighter(QTextDocument *parent);

    virtual ~XMLHighlighter();

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat valueFormat;
    QRegExp valueStartExpression;
    QRegExp valueEndExpression;

    void highlightBlock(const QString & text);
};
#endif // XMLHIGHLIGHTER_H
